#include "unity.h"

#include "Trie.h"
#include "ip.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_add_ip(void)
{
    Trie *trie = trie_init();
    uint32_t ip;
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("255.0.0.0", &ip));
    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, ip, 8));
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("10.10.0.0", &ip));
    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, ip, 16));
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("10.10.16.0", &ip));
    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, ip, 24));
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("10.10.16.192", &ip));
    TEST_ASSERT_EQUAL(TRIE_OK, trie_add(trie, ip, 32));
    TEST_ASSERT_EQUAL(TRIE_OK,trie_print(trie));
}


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_add_ip);
    return UNITY_END();
}
