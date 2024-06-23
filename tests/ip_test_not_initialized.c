#include "unity.h"

#include "ip.h"
#include "ip_tool.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_not_initialized(void)
{
    TEST_ASSERT_EQUAL(IP_ERROR, del(numb_to_ip(255, 0, 0, 0), 8));
    TEST_ASSERT_EQUAL(IP_ERROR, check(numb_to_ip(255, 0, 0, 0)));
    TEST_ASSERT_EQUAL(IP_ERROR, print_all_ip_addr());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_not_initialized);
    return UNITY_END();
}
