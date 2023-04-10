#include <malloc.h>
#include <stdio.h>
#include <sizes.h>
#include <s5pv210-serial.h>
#include <s5pv210-serial-stdio.h>

int serial_printf(int ch, const char * fmt, ...)
{
	va_list ap;
	char * buf;
	int len;
	int rv;

	va_start(ap, fmt);
	len = vsnprintf(NULL, 0, fmt, ap);
	if(len < 0)
		return 0;
	buf = malloc(len + 1);
	if(!buf)
		return 0;
	rv = vsnprintf(buf, len + 1, fmt, ap);
	va_end(ap);

	rv = (s5pv210_serial_write_string(ch, buf) < 0) ? 0 : rv;
	free(buf);

	return rv;
}
