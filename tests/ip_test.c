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
    uint32_t ip;
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("10.01.10.1", &ip));
    TEST_ASSERT_EQUAL(0x0A010A01, ip);
    TEST_ASSERT_EQUAL(IP_OK, str_to_ip("255.1.255.1", &ip));
    TEST_ASSERT_EQUAL(0xFF01FF01, ip);
}

void test_str_to_ip_wrong_input(void)
{
    uint32_t ip;
    TEST_ASSERT_EQUAL(IP_ERROR, str_to_ip("256.01.10.1", &ip));
    TEST_ASSERT_EQUAL(IP_ERROR, str_to_ip("255.-1.255.1", &ip));
    TEST_ASSERT_EQUAL(IP_ERROR, str_to_ip("255.-1.255.1", &ip));
}

void test_numb_to_ip(void)
{
    TEST_ASSERT_EQUAL(0x0A010A01, numb_to_ip(10, 01, 10, 1));
    TEST_ASSERT_EQUAL(0xFF01FF01, numb_to_ip(255, 01, 255, 1));
}

void test_ip_to_str(void)
{
    char buff[32] = {};
    TEST_ASSERT_EQUAL(IP_OK, ip_to_str(0xFF0AF00A, buff, 32));
    TEST_ASSERT_EQUAL_STRING("255.10.240.10", buff);

    TEST_ASSERT_EQUAL(IP_OK, ip_to_str(0x0A010A01, buff, 32));
    TEST_ASSERT_EQUAL_STRING("10.1.10.1", buff);

    TEST_ASSERT_EQUAL(IP_STR_TOO_SHORT, ip_to_str(0x0A010A01, buff, 8));
    TEST_ASSERT_EACH_EQUAL_CHAR(0, buff, 8);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_str_to_ip);
    RUN_TEST(test_str_to_ip_wrong_input);
    RUN_TEST(test_numb_to_ip);
    RUN_TEST(test_ip_to_str);
    return UNITY_END();
}