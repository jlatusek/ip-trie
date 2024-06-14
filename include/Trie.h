#pragma once

#include <stdint.h>

struct Trie_t
{
    struct Trie_t *children[2];
    bool used;
};

typedef struct Trie_t Trie;

Trie *trie_init();

int trie_add(Trie *trie, uint32_t base, char mask);
int trie_del(Trie *trie, uint32_t base, char mask);
char trie_check(const Trie *trie, uint32_t ip);
void trie_print(const Trie *trie);
int trie_clean(Trie *trie);
