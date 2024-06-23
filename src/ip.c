#include "ip.h"
#include "CodeUtils.h"
#include "Trie.h"


static Trie *trie;

int init()
{
    trie = trie_init();
    VerifyOrReturnError(trie != NULL, TRIE_ERROR);
    return TRIE_OK;
}

int deinit()
{
    return trie_deinit(trie);
}

int add(unsigned int base, char mask)
{
    return trie_add(trie, base, mask);
}

int del(unsigned int base, char mask)
{
    return trie_del(trie, base, mask);
}

int check(unsigned int ip)
{
    return trie_check(trie, ip);
}

int print_all_ip_addr()
{
    VerifyOrReturnError(trie != NULL, TRIE_ERROR);
    return trie_print(trie);
}
