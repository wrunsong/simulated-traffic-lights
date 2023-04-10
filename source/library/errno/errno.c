/*
 * libc/errno/errno.c
 */

#include <errno.h>

/* error number */
static int __errno;

/*
 * Internal function returning the address of the thread-specific errno
 */
volatile int * __runtime_errno_location(void)
{
	return &(__errno);
}
