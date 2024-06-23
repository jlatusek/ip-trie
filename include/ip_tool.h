#pragma once

#include <stdint.h>

int str_to_ip(const char *ip_str, uint32_t *ip);
uint32_t numb_to_ip(const uint8_t ip1, const uint8_t ip2, const uint8_t ip3, const uint8_t ip4);
int ip_to_str(const uint32_t ip, char *ip_str, int len);
