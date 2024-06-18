#include "Trie.h"
#include "ip.h"

#include "unity.h"

#include <stdint.h>
#include <string.h>

constexpr unsigned int MAX_EXPECTED_IP = 32;
static uint32_t expected_ip[MAX_EXPECTED_IP];
static uint8_t expected_mask[MAX_EXPECTED_IP];
static bool ip_found[MAX_EXPECTED_IP];
static Trie *trie;

void setUp(void)
{
    memset(expected_ip, 0, sizeof(expected_ip));
    memset(expected_mask, 0, sizeof(expected_mask));
    memset(ip_found, 0, sizeof(ip_found));
    trie = trie_init();
}

void tearDown(void)
{
    TEST_ASSERT_EQUAL(TRIE_OK,trie_deinit(trie));
    trie = NULL;
}

void ip_callback(uint32_t ip, uint8_t mask)
{
    for (unsigned int i = 0; i < MAX_EXPECTED_IP; ++i)
    {
        if (expected_ip[i] == ip && expected_mask[i] == mask)
        {
            TEST_ASSERT(!ip_found[i]);
            ip_found[i] = true;
            return;
        }
    }

    char buff[256] = {};
    char ip_str[IP_STR_EXPECTED_LEN] = {};
    ip_to_str(ip, ip_str, IP_STR_EXPECTED_LEN);
    snprintf(buff, sizeof(buff), "IP not found in expected list: %s/%d", ip_str, mask);
    TEST_FAIL_MESSAGE(buff);
}

void test_add_ip(void)
{
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("255.0.0.0", &expected_ip[0]));
    expected_mask[0] = 8;
    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, expected_ip[0], expected_mask[0]));
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("10.10.0.0", &expected_ip[1]));
    expected_mask[1] = 16;
    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, expected_ip[1], expected_mask[1]));
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("10.10.16.0", &expected_ip[2]));
    expected_mask[2] = 24;
    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, expected_ip[2], expected_mask[2]));
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("10.10.16.192", &expected_ip[3]));
    expected_mask[3] = 32;
    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, expected_ip[3], expected_mask[3]));
    TEST_ASSERT_EQUAL(TRIE_OK, trie_foreach(trie, ip_callback));
    TEST_ASSERT_EACH_EQUAL_CHAR(1, ip_found, 4);
}

void test_add_ip_2(void)
{
    for (unsigned int i = 0; i < MAX_EXPECTED_IP; i++)
    {
        expected_mask[i] = i;
        expected_ip[i] = (i * 0xAAAAAAAA) & (0xFFFFFFFF << (IP_LEN - expected_mask[i]));
        TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, expected_ip[i], expected_mask[i]));
        TEST_ASSERT_EQUAL(TRIE_OK, trie_foreach(trie, ip_callback));
        TEST_ASSERT_EACH_EQUAL_CHAR(1, ip_found, i + 1);
        memset(ip_found, 0, sizeof(ip_found));
    }
}

void test_del_ip(void)
{
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("255.0.0.0", &expected_ip[0]));
    expected_mask[0] = 8;
    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, expected_ip[0], expected_mask[0]));
    TEST_ASSERT_EQUAL(TRIE_OK, trie_foreach(trie, ip_callback));

    TEST_ASSERT_EQUAL(TRIE_OK, trie_del(trie, expected_ip[0], expected_mask[0]));
    expected_ip[0] = 0;
    expected_mask[0] = 0;

    TEST_ASSERT_EQUAL(TRIE_OK, trie_foreach(trie, ip_callback));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_add_ip);
    RUN_TEST(test_add_ip_2);
    RUN_TEST(test_del_ip);
    return UNITY_END();
}
