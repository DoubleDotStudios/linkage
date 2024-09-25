#ifndef LK_UTILS_H
#define LK_UTILS_H
#include "list.h"
char *str_to_hex(const char *str);
char *mkstr(const char *str);
list_T *str_to_hex_chunks(const char *str);
char str_to_esc_seq(const char *str);
char *format_str(char *str);
void assert_not_null(void *x);
#endif // !LK_UTILS_H
