#include "unity.h"

#include "ip.h"
#include "ip_tool.h"

void setUp(void)
{
    init();
}

void tearDown(void)
{
    deinit();
}


void test_add_ip(void)
{
    TEST_ASSERT_EQUAL(IP_OK, add(numb_to_ip(255, 0, 0, 0), 8));
    TEST_ASSERT_EQUAL(IP_OK, add(numb_to_ip(10, 10, 0, 0), 16));
    TEST_ASSERT_EQUAL(IP_OK, add(numb_to_ip(10, 10, 10, 0), 24));
    TEST_ASSERT_EQUAL(IP_OK, add(numb_to_ip(10, 10, 10, 10), 32));
}

void test_del_ip(void)
{
    TEST_ASSERT_EQUAL(IP_OK, add(numb_to_ip(255, 0, 0, 0), 8));
    TEST_ASSERT_EQUAL(IP_OK, add(numb_to_ip(10, 10, 0, 0), 16));
    TEST_ASSERT_EQUAL(IP_OK, add(numb_to_ip(10, 10, 10, 0), 24));
    TEST_ASSERT_EQUAL(IP_OK, add(numb_to_ip(10, 10, 10, 10), 32));
    TEST_ASSERT_EQUAL(IP_OK, del(numb_to_ip(255, 0, 0, 0), 8));
    TEST_ASSERT_EQUAL(IP_OK, del(numb_to_ip(10, 10, 0, 0), 16));
    TEST_ASSERT_EQUAL(IP_OK, del(numb_to_ip(10, 10, 10, 0), 24));
    TEST_ASSERT_EQUAL(IP_OK, del(numb_to_ip(10, 10, 10, 10), 32));
    TEST_ASSERT_EQUAL(IP_ERROR, del(numb_to_ip(10, 10, 10, 10), 32));
}

void test_check_ip(void)
{
    TEST_ASSERT_EQUAL(IP_OK, add(numb_to_ip(255, 0, 0, 0), 8));
    TEST_ASSERT_EQUAL(IP_OK, add(numb_to_ip(10, 10, 0, 0), 16));
    TEST_ASSERT_EQUAL(IP_OK, add(numb_to_ip(10, 10, 10, 0), 24));
    TEST_ASSERT_EQUAL(IP_OK, add(numb_to_ip(10, 10, 10, 10), 32));

    TEST_ASSERT_EQUAL(8, check(numb_to_ip(255, 0, 0, 0)));
    TEST_ASSERT_EQUAL(16, check(numb_to_ip(10, 10, 0, 0)));
    TEST_ASSERT_EQUAL(24, check(numb_to_ip(10, 10, 10, 0)));
    TEST_ASSERT_EQUAL(32, check(numb_to_ip(10, 10, 10, 10)));
    TEST_ASSERT_EQUAL(24, check(numb_to_ip(10, 10, 10, 11)));
    TEST_ASSERT_EQUAL(16, check(numb_to_ip(10, 10, 11, 11)));
    TEST_ASSERT_EQUAL(IP_ERROR, check(numb_to_ip(10, 16, 11, 11)));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_add_ip);
    RUN_TEST(test_del_ip);
    RUN_TEST(test_check_ip);
    return UNITY_END();
}
