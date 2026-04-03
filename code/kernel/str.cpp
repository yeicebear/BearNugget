#include "../include/str.hpp"

int k_strcmp(const char* a, const char* b) 
{
    while (*a && *a == *b) { a++; b++; }
    return *a - *b;
}

int k_strncmp(const char* a, const char* b, size_t n) {
    while (n-- && *a && *a == *b) { a++; b++; }
    return n == (size_t)-1 ? 0 : *a - *b;
}

size_t k_strlen(const char* s) 
{
    size_t n = 0;
    while (*s++) n++;
    return n;
}
char* k_strcpy(char* dst, const char* src) 
{
    char* d = dst;
    while ((*d++ = *src++));
    return dst;
}