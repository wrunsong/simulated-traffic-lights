#include <types.h>
#include <string.h>
#include <io.h>
#include <stddef.h>
#include <s5pv210/reg-gpio.h>
#include <s5pv210/reg-serial.h>
#include <s5pv210-clk.h>
#include <s5pv210-serial.h>

static void s5pv210_serial0_init(void)
{
	/* configure GPA01, GPA00 for TXD0, RXD0 */
	writel(S5PV210_GPA0CON, (readl(S5PV210_GPA0CON) & ~(0xf<<0 | 0x0f<<4)) | (0x2<<0 | 0x2<<4));

	/* pull up GPA01 and GPA00 */
	writel(S5PV210_GPA0PUD, (readl(S5PV210_GPA0PUD) & ~(0x3<<0 | 0x03<<2)) | (0x2<<0 | 0x2<<2));

	/* configure clk source (pclk), mode, etc */
	writel(S5PV210_UCON0, 0x00000005);
	writel(S5PV210_UFCON0, 0x00000000);
	writel(S5PV210_UMON0, 0x00000000);

	/* configure uart */
	s5pv210_serial_setup(0, B115200, DATA_BITS_8, PARITY_NONE, STOP_BITS_1);
}

static void s5pv210_serial1_init(void)
{
	/* configure GPA05, GPA04 for TXD1, RXD1 */
	writel(S5PV210_GPA0CON, (readl(S5PV210_GPA0CON) & ~(0xf<<16 | 0x0f<<20)) | (0x2<<16 | 0x2<<20));

	/* pull up GPA05 and GPA04 */
	writel(S5PV210_GPA0PUD, (readl(S5PV210_GPA0PUD) & ~(0x3<<8 | 0x03<<10)) | (0x2<<8 | 0x2<<10));

	/* configure clk source (pclk), mode, etc */
	writel(S5PV210_UCON1, 0x00000005);
	writel(S5PV210_UFCON1, 0x00000000);
	writel(S5PV210_UMON1, 0x00000000);

	/* configure uart */
	s5pv210_serial_setup(1, B115200, DATA_BITS_8, PARITY_NONE, STOP_BITS_1);
}

static void s5pv210_serial2_init(void)
{
	/* configure GPA11, GPA10 for TXD2, RXD2 */
	writel(S5PV210_GPA1CON, (readl(S5PV210_GPA1CON) & ~(0xf<<0 | 0x0f<<4)) | (0x2<<0 | 0x2<<4));

	/* pull up GPA11 and GPA10 */
	writel(S5PV210_GPA1PUD, (readl(S5PV210_GPA1PUD) & ~(0x3<<0 | 0x03<<2)) | (0x2<<0 | 0x2<<2));

	/* configure clk source (pclk), etc */
	writel(S5PV210_UCON2, 0x00000005);
	writel(S5PV210_UFCON2, 0x00000000);

	/* configure uart */
	s5pv210_serial_setup(2, B115200, DATA_BITS_8, PARITY_NONE, STOP_BITS_1);
}

static void s5pv210_serial3_init(void)
{
	/* configure GPA13, GPA12 for TXD3, RXD3 */
	writel(S5PV210_GPA1CON, (readl(S5PV210_GPA1CON) & ~(0xf<<8 | 0x0f<<12)) | (0x2<<8 | 0x2<<12));

	/* pull up GPA13 and GPA12 */
	writel(S5PV210_GPA1PUD, (readl(S5PV210_GPA1PUD) & ~(0x3<<4 | 0x03<<6)) | (0x2<<4 | 0x2<<6));

	/* configure clk source (pclk), etc */
	writel(S5PV210_UCON3, 0x00000005);
	writel(S5PV210_UFCON3, 0x00000000);

	/* configure uart */
	s5pv210_serial_setup(3, B115200, DATA_BITS_8, PARITY_NONE, STOP_BITS_1);
}

static ssize_t s5pv210_serial0_read(u8_t * buf, size_t count)
{
	ssize_t i;

	for(i = 0; i < count; i++)
	{
		if( (readl(S5PV210_UTRSTAT0) & S5PV210_UTRSTAT_RXDR) )
			buf[i] = readb(S5PV210_URXH0);
		else
			break;
	}
	return i;
}

static ssize_t s5pv210_serial1_read(u8_t * buf, size_t count)
{
	ssize_t i;

	for(i = 0; i < count; i++)
	{
		if( (readl(S5PV210_UTRSTAT1) & S5PV210_UTRSTAT_RXDR) )
			buf[i] = readb(S5PV210_URXH1);
		else
			break;
	}
	return i;
}

static ssize_t s5pv210_serial2_read(u8_t * buf, size_t count)
{
	ssize_t i;

	for(i = 0; i < count; i++)
	{
		if( (readl(S5PV210_UTRSTAT2) & S5PV210_UTRSTAT_RXDR) )
			buf[i] = readb(S5PV210_URXH2);
		else
			break;
	}
	return i;
}

static ssize_t s5pv210_serial3_read(u8_t * buf, size_t count)
{
	ssize_t i;

	for(i = 0; i < count; i++)
	{
		if( (readl(S5PV210_UTRSTAT3) & S5PV210_UTRSTAT_RXDR) )
			buf[i] = readb(S5PV210_URXH3);
		else
			break;
	}
	return i;
}

static ssize_t s5pv210_serial0_write(const u8_t * buf, size_t count)
{
	ssize_t i;

	for(i = 0; i < count; i++)
	{
		/* wait for transmit buffer & shifter register empty */
		while( !(readl(S5PV210_UTRSTAT0) & S5PV210_UTRSTAT_TXE) );

		/* transmit a character */
		writeb(S5PV210_UTXH0, buf[i]);
	}
	return i;
}

static ssize_t s5pv210_serial1_write(const u8_t * buf, size_t count)
{
	ssize_t i;

	for(i = 0; i < count; i++)
	{
		/* wait for transmit buffer & shifter register empty */
		while( !(readl(S5PV210_UTRSTAT1) & S5PV210_UTRSTAT_TXE) );

		/* transmit a character */
		writeb(S5PV210_UTXH1, buf[i]);
	}
	return i;
}

static ssize_t s5pv210_serial2_write(const u8_t * buf, size_t count)
{
	ssize_t i;

	for(i = 0; i < count; i++)
	{
		/* wait for transmit buffer & shifter register empty */
		while( !(readl(S5PV210_UTRSTAT2) & S5PV210_UTRSTAT_TXE) );

		/* transmit a character */
		writeb(S5PV210_UTXH2, buf[i]);
	}
	return i;
}

static ssize_t s5pv210_serial3_write(const u8_t * buf, size_t count)
{
	ssize_t i;

	for(i = 0; i < count; i++)
	{
		/* wait for transmit buffer & shifter register empty */
		while( !(readl(S5PV210_UTRSTAT3) & S5PV210_UTRSTAT_TXE) );

		/* transmit a character */
		writeb(S5PV210_UTXH3, buf[i]);
	}
	return i;
}

void s5pv210_serial_initial(void)
{
	s5pv210_serial0_init();
	s5pv210_serial1_init();
	s5pv210_serial2_init();
	s5pv210_serial3_init();
}

bool_t s5pv210_serial_setup(int ch, enum baud_rate_t baud, enum data_bits_t data, enum parity_bits_t parity, enum stop_bits_t stop)
{
	const u32_t udivslot_code[16] = {0x0000, 0x0080, 0x0808, 0x0888,
									 0x2222, 0x4924, 0x4a52, 0x54aa,
									 0x5555, 0xd555, 0xd5d5, 0xddd5,
									 0xdddd, 0xdfdd, 0xdfdf, 0xffdf};
	u32_t ibaud, baud_div_reg, baud_divslot_reg;
	u8_t data_bit_reg, parity_reg, stop_bit_reg;
	u64_t pclk;

	if((ch < 0) || (ch > 3))
		return FALSE;

	switch(baud)
	{
	case B50:
		ibaud = 50;
		break;
	case B75:
		ibaud = 75;
		break;
	case B110:
		ibaud = 110;
		break;
	case B134:
		ibaud = 134;
		break;
	case B200:
		ibaud = 200;
		break;
	case B300:
		ibaud = 300;
		break;
	case B600:
		ibaud = 600;
		break;
	case B1200:
		ibaud = 1200;
		break;
	case B1800:
		ibaud = 1800;
		break;
	case B2400:
		ibaud = 2400;
		break;
	case B4800:
		ibaud = 4800;
		break;
	case B9600:
		ibaud = 9600;
		break;
	case B19200:
		ibaud = 19200;
		break;
	case B38400:
		ibaud = 38400;
		break;
	case B57600:
		ibaud = 57600;
		break;
	case B76800:
		ibaud = 76800;
		break;
	case B115200:
		ibaud = 115200;
		break;
	case B230400:
		ibaud = 230400;
		break;
	case B380400:
		ibaud = 380400;
		break;
	case B460800:
		ibaud = 460800;
		break;
	case B921600:
		ibaud = 921600;
		break;
	default:
		return FALSE;
	}

	switch(data)
	{
	case DATA_BITS_5:
		data_bit_reg = 0x0;		break;
	case DATA_BITS_6:
		data_bit_reg = 0x1;		break;
	case DATA_BITS_7:
		data_bit_reg = 0x2;		break;
	case DATA_BITS_8:
		data_bit_reg = 0x3;		break;
	default:
		return FALSE;
	}

	switch(parity)
	{
	case PARITY_NONE:
		parity_reg = 0x0;		break;
	case PARITY_EVEN:
		parity_reg = 0x5;		break;
	case PARITY_ODD:
		parity_reg = 0x4;		break;
	default:
		return FALSE;
	}

	switch(stop)
	{
	case STOP_BITS_1:
		stop_bit_reg = 0;		break;
	case STOP_BITS_1_5:
		return -1;
	case STOP_BITS_2:
		stop_bit_reg = 1;		break;
	default:
		return FALSE;
	}

	if(clk_get_rate("psys-pclk", &pclk))
	{
		baud_div_reg = (u32_t)((pclk / (ibaud * 16)) ) - 1;
		baud_divslot_reg = udivslot_code[( (u32_t)((pclk % (ibaud*16)) / ibaud) ) & 0xf];
	}
	else
	{
		return FALSE;
	}

	switch(ch)
	{
	case 0:
		writel(S5PV210_UBRDIV0, baud_div_reg);
		writel(S5PV210_UDIVSLOT0, baud_divslot_reg);
		writel(S5PV210_ULCON0, (data_bit_reg<<0 | stop_bit_reg <<2 | parity_reg<<3));
		break;
	case 1:
		writel(S5PV210_UBRDIV1, baud_div_reg);
		writel(S5PV210_UDIVSLOT1, baud_divslot_reg);
		writel(S5PV210_ULCON1, (data_bit_reg<<0 | stop_bit_reg <<2 | parity_reg<<3));
		break;
	case 2:
		writel(S5PV210_UBRDIV2, baud_div_reg);
		writel(S5PV210_UDIVSLOT2, baud_divslot_reg);
		writel(S5PV210_ULCON2, (data_bit_reg<<0 | stop_bit_reg <<2 | parity_reg<<3));
		break;
	case 3:
		writel(S5PV210_UBRDIV3, baud_div_reg);
		writel(S5PV210_UDIVSLOT3, baud_divslot_reg);
		writel(S5PV210_ULCON3, (data_bit_reg<<0 | stop_bit_reg <<2 | parity_reg<<3));
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

ssize_t s5pv210_serial_read(int ch, u8_t * buf, size_t count)
{
	switch(ch)
	{
	case 0:
		return s5pv210_serial0_read(buf, count);
	case 1:
		return s5pv210_serial1_read(buf, count);
	case 2:
		return s5pv210_serial2_read(buf, count);
	case 3:
		return s5pv210_serial3_read(buf, count);
	default:
		break;
	}

	return 0;
}

ssize_t s5pv210_serial_write(int ch, u8_t * buf, size_t count)
{
	switch(ch)
	{
	case 0:
		return s5pv210_serial0_write(buf, count);
	case 1:
		return s5pv210_serial1_write(buf, count);
	case 2:
		return s5pv210_serial2_write(buf, count);
	case 3:
		return s5pv210_serial3_write(buf, count);
	default:
		break;
	}

	return 0;
}

ssize_t s5pv210_serial_write_string(int ch, const char * buf)
{
	return s5pv210_serial_write(ch, (u8_t *)buf, strlen(buf));
}
