#include "Trie.h"

#include "CodeUtils.h"
#include "ip.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const static Trie default_trie = {
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

int trie_free(Trie *trie)
{
    VerifyOrReturn(trie == nullptr || trie_has_child(trie), TRIE_ERROR);
    free(trie);
    trie = nullptr;
    return 0;
}

int trie_add(Trie *trie, uint32_t base, char mask)
{
    Trie *root = trie;
    for (int i = 0; i < mask; ++i)
    {
        uint bit = (base << i) & 0x01;
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
    Trie *trie_path[IP_LEN] = {};
    Trie *root = trie;
    for (int i = IP_LEN - 1; i >= 0; --i)
    {
        uint bit = (base >> i) & 0x01;
        VerifyOrReturn(root->children[bit] == nullptr, TRIE_ERROR);
        root = root->children[bit];
        trie_path[i] = root;
    }
    root->used = false;
    for (int i = mask - 1; i >= 0; --i)
    {
        if (!trie_has_child(trie_path[i]) && !trie_path[i]->used)
        {
            trie_free(trie_path[i]);
        }
        else
        {
            break;
        }
    }
    return 0;
}

char trie_check(const Trie *trie, uint32_t ip)
{
}

int trie_clean(Trie *trie)
{
    for (uint i = 0; i < 2; ++i)
    {
        if (trie->children[i])
        {
            VerifyOrReturn(trie_clean(trie->children[i]) != 0, TRIE_ERROR);
        }
    }
    VerifyOrReturnWithMsg(trie_free(trie) != 0, -1, "Failed to clean trie :(");
    return TRIE_OK;
}
