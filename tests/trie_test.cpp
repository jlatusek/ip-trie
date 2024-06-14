#include "CppUTest/TestHarness.h"

TEST_GROUP(FirstTestGroup){};

TEST(FirstTestGroup, FirstTest)
{
    FAIL("NO FAIL ME!");
}