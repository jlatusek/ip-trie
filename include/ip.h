#pragma once

#include <stdint.h>

constexpr int IP_OK = 0;
constexpr int IP_ERROR = -1;
constexpr unsigned int IP_LEN = 32;

uint32_t str_to_ip(const char *ip_str);
uint32_t numb_to_ip(const uint8_t ip1, const uint8_t ip2, const uint8_t ip3, const uint8_t ip4);
