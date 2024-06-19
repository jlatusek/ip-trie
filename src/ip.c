#include "ip.h"
#include "CodeUtils.h"
#include "Trie.h"

#include <stdio.h>
#include <string.h>

int str_to_ip(const char *ip_str, uint32_t *ip)
{
    int c[4] = {};
    sscanf(ip_str, "%d.%d.%d.%d", c, c + 1, c + 2, c + 3);
    for (int i = 0; i < 4; ++i)
    {
        if (c[i] < 0 || c[i] > 255)
        {
            *ip = 0;
            return IP_ERROR;
        }
    }
    *ip = numb_to_ip(c[0], c[1], c[2], c[3]);
    return IP_OK;
}

uint32_t numb_to_ip(const uint8_t ip1, const uint8_t ip2, const uint8_t ip3, const uint8_t ip4)
{
    return ip1 << 24 | ip2 << 16 | ip3 << 8 | ip4;
}

int ip_to_str(const uint32_t ip, char *ip_str, int len)
{
    uint8_t c[4] = {
        (ip >> 24) & 0xFF,
        (ip >> 16) & 0xFF,
        (ip >> 8) & 0xFF,
        ip & 0xFF,
    };
    int written_len = snprintf(ip_str, len, "%d.%d.%d.%d", c[0], c[1], c[2], c[3]);
    if (written_len > len)
    {
        bzero(ip_str, len);
        return IP_STR_TOO_SHORT;
    }
    return IP_OK;
}

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
    if (trie == NULL)
    {
        trie = trie_init();
    }
    return trie_add(trie, base, mask);
}

int del(unsigned int base, char mask)
{
    if (trie == NULL)
    {
        return TRIE_ERROR;
    }
    return trie_del(trie, base, mask);
}

int check(unsigned int ip)
{
    if (trie == NULL)
    {
        return TRIE_ERROR;
    }
    return trie_check(trie, ip);
}
