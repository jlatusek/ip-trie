#pragma once

constexpr int IP_OK = 0;
constexpr int IP_ERROR = -1;
constexpr int IP_STR_TOO_SHORT = -2;
constexpr unsigned int IP_LEN = 32;
constexpr unsigned int IP_STR_EXPECTED_LEN = 16;

/*
 * init() - Initialize the IP trie
 *
 * @return: IP_OK(0) on success, IP_ERROR(-1) on error
 */
int init();

/*
 * deinit() - Deinitialize the IP trie
 *
 * @return: IP_OK(0) on success, IP_ERROR(-1) on error
 */
int deinit();

/*
 * add() - Save the IP address and mask in the structure
 *
 * @base: IP address
 * @mask: Mask
 *
 * @return: IP_OK(0) on success, IP_ERROR(-1) on error
 */
int add(unsigned int base, char mask);

/*
 * del() - Remove the IP address and mask from the structure
 *
 * @base: IP address
 * @mask: Mask
 *
 * @return: IP_OK(0) on success, IP_ERROR(-1) on error
 */
int del(unsigned int base, char mask);

/*
 * check() - Check if the IP address is in the structure
 *
 * @ip: IP address
 *
 * @return: IP mask on success, -1 on error
 */
int check(unsigned int ip);

/*
 * print_all_ip_addr() - Print all IP addresses and masks
 *
 * @return: IP_OK(0) on success, IP_ERROR(-1) on error
 */
int print_all_ip_addr();