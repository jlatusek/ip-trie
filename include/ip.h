#pragma once

#include <stdint.h>

constexpr int IP_OK = 0;
constexpr int IP_ERROR = -1;
constexpr int IP_STR_TOO_SHORT = -2;
constexpr unsigned int IP_LEN = 32;
constexpr unsigned int IP_STR_EXPECTED_LEN = 16;

int str_to_ip(const char *ip_str, uint32_t *ip);
uint32_t numb_to_ip(const uint8_t ip1, const uint8_t ip2, const uint8_t ip3, const uint8_t ip4);
int ip_to_str(const uint32_t ip, char *ip_str, int len);
