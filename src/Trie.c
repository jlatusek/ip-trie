#include "Trie.h"

#include "CodeUtils.h"
#include "ip.h"

#include <stdio.h>
#include <stdlib.h>

static bool trie_has_child(Trie *trie)
{
    return trie->children[0] != NULL || trie->children[1] != NULL;
}

Trie *trie_init()
{
    Trie *trie = calloc(1, sizeof(Trie));
    VerifyOrDie(trie != NULL);
    return trie;
}

int trie_free(Trie *trie)
{
    VerifyOrReturnErrorWithMsg(trie != NULL, TRIE_ERROR, "Provided trie is NULL");
    VerifyOrReturnErrorWithMsg(!trie_has_child(trie), TRIE_HAS_CHILDREN, "Cannot remove trie, there are children");
    free(trie);
    return TRIE_OK;
}

int trie_add(Trie *trie, uint32_t base, uint8_t mask)
{
    Trie *root = trie;
    for (int i = 0; i < mask; ++i)
    {
        int bit_index = (IP_LEN - 1 - i);
        uint bit = (base >> bit_index) & 0x01;
        if (root->children[bit] == NULL)
        {
            root->children[bit] = trie_init();
        }
        root = root->children[bit];
    }
    root->used = true;
    return TRIE_OK;
}

int trie_del(Trie *trie, uint32_t base, uint8_t mask)
{
    VerifyOrReturnErrorWithMsg(mask <= IP_LEN, TRIE_ERROR, "Value passed as mask is too high %d > 32", mask);

    int ret = TRIE_OK;

    Trie *trie_path[IP_LEN + 1] = {trie};
    Trie *root = trie;
    for (int i = 0; i < mask; ++i)
    {
        uint bit = (base >> (IP_LEN - 1 - i)) & 0x01;
        Trie *child = root->children[bit];
        VerifyOrReturnErrorWithMsg(child != NULL, TRIE_ERROR, "Provided ip do not exist in trie structure");
        root = child;
        trie_path[i + 1] = root;
    }
    VerifyOrReturnErrorWithMsg(root->used, TRIE_ERROR, "Provided ip wasn't saved in trie structure");
    root->used = false;
    for (int i = mask; i >= 1; --i)
    {
        if (!trie_has_child(trie_path[i]) && !(trie_path[i]->used))
        {
            for (uint j = 0; j < MAX_CHILD_COUNT; ++j)
            {
                if (trie_path[i - 1]->children[j] == trie_path[i])
                {
                    trie_path[i - 1]->children[j] = NULL;
                }
            }
            ret = trie_free(trie_path[i]);
            VerifyOrReturnError(ret == 0, ret);
        }
        else
        {
            break;
        }
    }
    return ret;
}

int trie_check(const Trie *trie, uint32_t ip)
{
    const Trie *root = trie;
    uint i;
    int ret = TRIE_ERROR;
    for (i = 0; i <= IP_LEN; ++i)
    {
        uint bit = (ip >> (IP_LEN - 1 - i)) & 0x01;
        Trie *child = root->children[bit];
        if (root->used)
        {
            ret = (int)i;
        }
        VerifyOrReturnError(child != NULL, ret);
        root = child;
    }
    return ret;
}

static int _trie_deinit(Trie *trie, uint depth)
{
    VerifyOrReturnErrorWithMsg(trie != NULL, TRIE_ERROR, "Provided trie is NULL");
    VerifyOrReturnErrorWithMsg(depth <= IP_LEN, TRIE_ERROR, "Trie deinit gone to deep: %d", depth);
    for (uint i = 0; i < MAX_CHILD_COUNT; ++i)
    {
        if (trie->children[i] != NULL)
        {
            VerifyOrReturnError(_trie_deinit(trie->children[i], depth + 1) == TRIE_OK, TRIE_ERROR);
            trie->children[i] = NULL;
        }
    }
    int ret = trie_free(trie);
    VerifyOrReturnErrorWithMsg(ret == TRIE_OK, TRIE_ERROR, "Failed to clean trie :(");
    return TRIE_OK;
}

int trie_deinit(Trie *trie)
{
    return _trie_deinit(trie, 0);
}

static int _trie_foreach_node(const Trie *trie, void (*callback)(uint32_t ip_addr, uint8_t mask), uint32_t ip_addr,
                              uint8_t depth)
{
    VerifyOrReturnErrorWithMsg(trie != NULL, TRIE_ERROR, "Provided trie is NULL");
    VerifyOrReturnErrorWithMsg(depth <= IP_LEN, TRIE_ERROR, "Trie foreach gone to deep: %d", depth);
    if (trie->used)
    {
        callback(ip_addr << (IP_LEN - depth), depth);
    }
    for (uint i = 0; i < MAX_CHILD_COUNT; ++i)
    {
        if (trie->children[i] != NULL)
        {
            VerifyOrReturnError(_trie_foreach_node(trie->children[i], callback, ((ip_addr << 1) | i), depth + 1) ==
                                    TRIE_OK,
                                TRIE_ERROR);
        }
    }
    return TRIE_OK;
}

int trie_foreach(const Trie *trie, void (*callback)(uint32_t ip_addr, uint8_t mask))
{
    return _trie_foreach_node(trie, callback, 0, 0);
}

static void _print_ip(uint32_t ip_addr, uint8_t mask)
{
    char ip_str[IP_STR_EXPECTED_LEN] = {};
    VerifyOrReturnWithMsg(ip_to_str(ip_addr, ip_str, IP_STR_EXPECTED_LEN) == IP_OK,
                          "Error during generating string from ip");
    printf("ip: %s/%d\n", ip_str, mask);
}

int trie_print(const Trie *trie)
{
    return trie_foreach(trie, _print_ip);
}
