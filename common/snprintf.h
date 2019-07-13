#include <stdlib.h>
#include <stdarg.h>

int snprintf(char *str, size_t count, const char *fmt, ...);
int vsnprintf(char *str, size_t count, const char *fmt, va_list args);