#include <stdio.h>

#define VerifyOrDie(condition)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(condition))                                                                                              \
        {                                                                                                              \
            abort();                                                                                                   \
        }                                                                                                              \
    } while (0)

#define VerifyOrReturn(condition, return_value)                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(condition))                                                                                              \
        {                                                                                                              \
            return return_value;                                                                                       \
        }                                                                                                              \
    } while (0)

#define VerifyOrReturnWithMsg(condition, return_value, error_msg, ...)                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(condition))                                                                                              \
        {                                                                                                              \
            fprintf(stderr, error_msg, ##__VA_ARGS__);                                                                 \
            return return_value;                                                                                       \
        }                                                                                                              \
    } while (0)
