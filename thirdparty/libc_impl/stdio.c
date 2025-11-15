#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

extern void __io_putchar(char ch); 

static void print_num(int value, int base, bool is_signed) {
    char buffer[32];
    char *ptr = &buffer[31];
    *ptr = '\0';

    unsigned int num = (unsigned int)value;
    if (is_signed && value < 0) {
        num = (unsigned int)(-value);
    }

    do {
        int digit = num % base;
        *--ptr = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        num /= base;
    } while (num > 0);

    if (is_signed && value < 0) {
        *--ptr = '-';
    }

    while (*ptr) {
        __io_putchar(*ptr++);
    }
}

//Own implementation as libc is not linked
int printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'c': {
                    char c = (char)va_arg(args, int);
                    __io_putchar(c);
                    break;
                }
                case 's': {
                    char *s = va_arg(args, char*);
                    while (*s) __io_putchar(*s++);
                    break;
                }
                case 'd': {
                    int val = va_arg(args, int);
                    print_num(val, 10, true);
                    break;
                }
                case 'x': {
                    int val = va_arg(args, int);
                    print_num(val, 16, false);
                    break;
                }
                case '%': {
                    __io_putchar('%');
                    break;
                }
                default:
                    __io_putchar('?');  // unknown specifier
            }
        } else {
            __io_putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
    return 0;
}

int fflush (FILE *__stream)
{
    //Do nothing in our implementation
    (void) __stream;
    return 0;
}