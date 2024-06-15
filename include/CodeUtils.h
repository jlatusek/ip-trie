#include <stdio.h>

#define VerifyOrDie(condition)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(condition))                                                                                              \
        {                                                                                                              \
            fprintf(stderr, "%s:%d Program died", __FILE__, __LINE__);                                                 \
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
            fprintf(stderr, "%s:%d ", __FILE__, __LINE__);                                                              \
            fprintf(stderr, error_msg "\n", ##__VA_ARGS__);                                                            \
            return return_value;                                                                                       \
        }                                                                                                              \
    } while (0)
