#include "unity.h"

#include "ip.h"

void setUp(void) {};

void tearDown(void) {};

void test(void)
{
    TEST_FAIL();
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test);
    return UNITY_END();
}