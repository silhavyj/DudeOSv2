#ifndef _STRING_H_
#define _STRING_H_

#include <stdlib/stdint.h>

uint32_t strlen(const char *str);
int strcmp(const char *str1, const char *str2);
void strcpy(char *dst, const char *src);
void reverse(char *str);
void append(char *str, char c);
int atoi(const char *str);
int find_first_not_of(const char *str, char c);

void int_to_str(char *str, int num, int base);
void int_to_str_10(char *str, int num);
void int_to_str_16(char *str, int num);
void int_to_str_02(char *str, int num);
void double_to_ascii(double num, char *str, uint8_t afterpoint);

#endif