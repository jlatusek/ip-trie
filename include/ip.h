#pragma once

#include <stdint.h>

constexpr int IP_OK = 0;
constexpr int IP_ERROR = -1;
constexpr unsigned int IP_LEN = 32;

uint32_t str_to_ip(const char *ip_str);
uint32_t numb_to_ip(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4);
