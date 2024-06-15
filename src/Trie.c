#include "Trie.h"

#include "CodeUtils.h"
#include "ip.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const Trie default_trie = {
    .children = {},
    .used = false,
};

static bool trie_has_child(Trie *trie)
{
    return trie->children[0] != nullptr || trie->children[1] != nullptr;
}

Trie *trie_init()
{
    Trie *trie = malloc(sizeof(Trie));
    VerifyOrDie(trie != nullptr);
    memcpy(trie, &default_trie, sizeof(Trie));
    return trie;
}

int trie_free(Trie **trie)
{
    VerifyOrReturnWithMsg(trie != nullptr, TRIE_ERROR, "Provided trie is nullptr");
    VerifyOrReturnWithMsg(!trie_has_child(*trie), TRIE_HAS_CHILDREN, "Cannot remove trie, there are children");
    free(*trie);
    *trie = nullptr;
    return 0;
}

int trie_add(Trie *trie, uint32_t base, char mask)
{
    Trie *root = trie;
    for (int i = 0; i < mask; ++i)
    {
        int bit_index = (IP_LEN - 1 - i);
        uint bit = (base >> bit_index) & 0x01;
        if (root->children[bit] == nullptr)
        {
            root->children[bit] = trie_init();
        }
        root = root->children[bit];
    }
    root->used = true;
    return TRIE_OK;
}

int trie_del(Trie *trie, uint32_t base, char mask)
{
    VerifyOrReturnWithMsg((uint) mask < IP_LEN, TRIE_ERROR, "Value passed as mask is too high");

    Trie *trie_path[IP_LEN + 1] = {trie};
    Trie *root = trie;
    for (int i = 0; i < mask; ++i)
    {
        uint bit = (base >> (IP_LEN - 1 - i)) & 0x01;
        Trie *child = root->children[bit];
        VerifyOrReturnWithMsg(child != nullptr, TRIE_ERROR, "Provided ip do not exist in trie structure");
        root = child;
        trie_path[i + 1] = root;
    }
    root->used = false;
    for (int i = mask; i >= 0; --i)
    {
        if (!trie_has_child(trie_path[i]) && !(trie_path[i]->used))
        {
            if (i > 0)
            {
                for (uint j = 0; j < MAX_CHILD_COUNT; ++j)
                {
                    printf("%p == %p\n", (void *)trie_path[i - 1]->children[j], (void *)trie_path[i]);
                    if (trie_path[i - 1]->children[j] == trie_path[i])
                    {
                        trie_path[i - 1]->children[j] = nullptr;
                    }
                }
            }
            trie_free(&trie_path[i]);
        }
        else
        {
            //            break;
        }
    }
    return 0;
}

char trie_check(const Trie *trie, uint32_t ip)
{
    (void)trie;
    (void)ip;
    return 0;
}

int trie_deinit(Trie **trie)
{
    for (uint i = 0; i < MAX_CHILD_COUNT; ++i)
    {
        if ((*trie)->children[i])
        {
            VerifyOrReturn(trie_deinit(&(*trie)->children[i]) != TRIE_OK, TRIE_ERROR);
        }
    }
    VerifyOrReturnWithMsg(trie_free(trie) != TRIE_OK, TRIE_ERROR, "Failed to clean trie :(");
    return TRIE_OK;
}

static int node_print(const Trie *trie, uint32_t ip_addr, uint8_t depth)
{
    VerifyOrReturnWithMsg(depth <= IP_LEN, TRIE_ERROR, "Trie print gone to deep: %d", depth);
    if (trie->used)
    {
        char ip_str[IP_STR_EXPECTED_LEN] = {};
        VerifyOrReturnWithMsg(ip_to_str(ip_addr << (IP_LEN - depth), ip_str, IP_STR_EXPECTED_LEN) == IP_OK, TRIE_ERROR,
                              "Error during printing trie :((");
        printf("ip: %s/%d\n", ip_str, depth);
    }
    for (uint i = 0; i < MAX_CHILD_COUNT; ++i)
    {
        if (trie->children[i] != nullptr)
        {
            VerifyOrReturn(node_print(trie->children[i], ((ip_addr << 1) | i), depth + 1) == TRIE_OK, TRIE_ERROR);
        }
    }
    return TRIE_OK;
}

int trie_print(const Trie *trie)
{
    return node_print(trie, 0, 0);
}
