#include "Trie.h"
#include "ip.h"
#include "ip_tool.h"

#include "unity.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

constexpr unsigned int MAX_EXPECTED_IP = 256;
static uint32_t expected_ip[MAX_EXPECTED_IP];
static uint8_t expected_mask[MAX_EXPECTED_IP];
static bool ip_found[MAX_EXPECTED_IP];
static Trie *trie;
static uint callback_function_call_count;

void setUp(void)
{
    srand(44);
    callback_function_call_count = 0;
    memset(expected_ip, 0, sizeof(expected_ip));
    memset(expected_mask, 0, sizeof(expected_mask));
    memset(ip_found, 0, sizeof(ip_found));
    trie = trie_init();
}

void tearDown(void)
{
    TEST_ASSERT_EQUAL(TRIE_OK, trie_deinit(trie));
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
            callback_function_call_count++;
            return;
        }
    }

    char buff[256] = {};
    char ip_str[IP_STR_EXPECTED_LEN] = {};
    ip_to_str(ip, ip_str, IP_STR_EXPECTED_LEN);
    snprintf(buff, sizeof(buff), "IP not found in expected list: %s/%d, function call count: %d", ip_str, mask,
             callback_function_call_count);
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

    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("128.0.0.0", &expected_ip[4]));
    expected_mask[4] = 1;
    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, expected_ip[4], expected_mask[4]));

    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("255.255.255.255", &expected_ip[5]));
    expected_mask[5] = 32;
    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, expected_ip[5], expected_mask[5]));

    TEST_ASSERT_EQUAL(TRIE_OK, trie_foreach(trie, ip_callback));
    TEST_ASSERT_EACH_EQUAL_CHAR(1, ip_found, 6);
}

void test_add_ip_loop(void)
{
    for (unsigned int i = 0; i < 32; i++)
    {
        expected_mask[i] = i;
        expected_ip[i] = (i * 0xFFAAAAAA) & (0xFFFFFFFF << (IP_LEN - expected_mask[i]));
        TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, expected_ip[i], expected_mask[i]));
        TEST_ASSERT_EQUAL(TRIE_OK, trie_foreach(trie, ip_callback));
        TEST_ASSERT_EACH_EQUAL_CHAR(1, ip_found, i + 1);
        TEST_ASSERT_EACH_EQUAL_CHAR(0, ip_found + i + 1, MAX_EXPECTED_IP - i - 1);
        memset(ip_found, 0, sizeof(ip_found[0]) * MAX_EXPECTED_IP);
    }
    for (unsigned int i = 1; i < 32; i++)
    {
        int exp_idx = (int)i + 31;
        expected_mask[exp_idx] = i;
        expected_ip[exp_idx] = (i * 0x5BBBBBBB) & (0xFFFFFFFF << (IP_LEN - expected_mask[exp_idx]));
        TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, expected_ip[exp_idx], expected_mask[exp_idx]));
        TEST_ASSERT_EQUAL(TRIE_OK, trie_foreach(trie, ip_callback));
        TEST_ASSERT_EACH_EQUAL_CHAR(1, ip_found, exp_idx + 1);
        TEST_ASSERT_EACH_EQUAL_CHAR(0, ip_found + exp_idx + 1, MAX_EXPECTED_IP - exp_idx - 1);
        memset(ip_found, 0, sizeof(ip_found[0]) * MAX_EXPECTED_IP);
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

void test_del_ip_sub_net(void)
{
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("255.0.0.0", &expected_ip[0]));
    expected_mask[0] = 8;
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("255.255.0.0", &expected_ip[1]));
    expected_mask[1] = 16;
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("255.255.255.0", &expected_ip[2]));
    expected_mask[2] = 24;

    for (unsigned int i = 0; i < 3; i++)
    {
        TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, expected_ip[i], expected_mask[i]));
    }

    TEST_ASSERT_EQUAL(TRIE_OK, trie_foreach(trie, ip_callback));
    TEST_ASSERT_EACH_EQUAL_CHAR(1, ip_found, 3);
    TEST_ASSERT_EACH_EQUAL_CHAR(0, ip_found + 3, MAX_EXPECTED_IP - 3);

    TEST_ASSERT_EQUAL(TRIE_OK, trie_del(trie, expected_ip[1], expected_mask[1]));
    memset(ip_found, 0, sizeof(ip_found[0]) * MAX_EXPECTED_IP);
    TEST_ASSERT_EQUAL(TRIE_OK, trie_foreach(trie, ip_callback));
    TEST_ASSERT_TRUE(ip_found[0]);
    TEST_ASSERT_FALSE(ip_found[1]);
    TEST_ASSERT_TRUE(ip_found[2]);
    TEST_ASSERT_EACH_EQUAL_CHAR(0, ip_found + 3, MAX_EXPECTED_IP - 3);
}

void test_check_ip(void)
{
    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, numb_to_ip(192, 168, 1, 0), 24));
    TEST_ASSERT_EQUAL(24, trie_check(trie, numb_to_ip(192, 168, 1, 0)));
    TEST_ASSERT_EQUAL(24, trie_check(trie, numb_to_ip(192, 168, 1, 1)));
    TEST_ASSERT_EQUAL(-1, trie_check(trie, numb_to_ip(192, 168, 0, 0)));
    TEST_ASSERT_EQUAL(-1, trie_check(trie, numb_to_ip(192, 168, 0, 1)));
    TEST_ASSERT_EQUAL(-1, trie_check(trie, numb_to_ip(192, 160, 0, 0)));

    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, numb_to_ip(192, 168, 1, 1), 24));
    TEST_ASSERT_EQUAL(24, trie_check(trie, numb_to_ip(192, 168, 1, 1)));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_add_ip);
    RUN_TEST(test_add_ip_loop);
    RUN_TEST(test_del_ip);
    RUN_TEST(test_check_ip);
    RUN_TEST(test_del_ip_sub_net);
    return UNITY_END();
}
