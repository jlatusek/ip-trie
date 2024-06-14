#include "ip.h"

#include "stdio.h"

uint32_t str_to_ip(const char *ip_str)
{
    int c[4] = {};
    sscanf(ip_str, "%d.%d.%d.%d", c, c + 1, c + 2, c + 3);
    return numb_to_ip(c[0], c[1], c[2], c[3]);
}

uint32_t numb_to_ip(const uint8_t ip1, const uint8_t ip2, const uint8_t ip3, const uint8_t ip4)
{
    return ip1 << 24 | ip2 << 16 | ip3 << 8 | ip4;
}

void ip_to_str(const uint32_t ip, char * ip_str, int len)
{
    uint8_t c[4] = {
        (ip >> 24) & 0xFF,
        (ip >> 16) & 0xFF,
        (ip >> 8) & 0xFF,
        ip & 0xFF,
    };
    snprintf(ip_str, len, "%d.%d.%d.%d", c[0], c[1], c[2], c[3]);
}
