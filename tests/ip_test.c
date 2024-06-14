#include "unity.h"

#include "ip.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_str_to_ip(void)
{
    TEST_ASSERT_EQUAL(0x0A010A01, str_to_ip("10.01.10.1"));
    TEST_ASSERT_EQUAL(0xFF01FF01, str_to_ip("255.1.255.1"));
}

void test_numb_to_ip(void)
{
    TEST_ASSERT_EQUAL(0x0A010A01, numb_to_ip(10, 01, 10, 1));
    TEST_ASSERT_EQUAL(0xFF01FF01, numb_to_ip(255, 01, 255, 1));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_str_to_ip);
    RUN_TEST(test_numb_to_ip);
    return UNITY_END();
}