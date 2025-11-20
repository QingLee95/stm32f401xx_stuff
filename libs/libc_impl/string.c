#include <string.h>

//This function is marked with attribute nonnull-compare
size_t strlen(const char *s) {
    size_t size = 0;
    while(s[size++] != '\0');
    return size - 1;
}

