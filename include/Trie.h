#pragma once

#include <stdint.h>

constexpr unsigned int MAX_CHILD_COUNT = 2;
constexpr int TRIE_OK = 0;
constexpr int TRIE_ERROR = -1;

struct Trie_t
{
    struct Trie_t *children[MAX_CHILD_COUNT];
    bool used;
};

typedef struct Trie_t Trie;

Trie *trie_init();
int trie_deinit(Trie *trie);

int trie_add(Trie *trie, uint32_t base, char mask);
int trie_del(Trie *trie, uint32_t base, char mask);
char trie_check(const Trie *trie, uint32_t ip);
int trie_print(const Trie *trie);
