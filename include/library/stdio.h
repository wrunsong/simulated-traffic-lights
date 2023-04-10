#ifndef __STDIO_H__
#define __STDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

int vsnprintf(char * buf, size_t n, const char * fmt, va_list ap);
int vsscanf(const char * buf, const char * fmt, va_list ap);
int sprintf(char * buf, const char * fmt, ...);
int snprintf(char * buf, size_t n, const char * fmt, ...);
int sscanf(const char * buf, const char * fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* __STDIO_H__ */
