#include <types.h>
#include <io.h>
#include <s5pv210/reg-gpio.h>
#include <hw-led.h>

/*
 * LED1 -> D22 -> GPJ0_3
 * LED2 -> D23 -> GPJ0_4
 * LED3 -> D24 -> GPJ0_5
 * LED4 -> D25 -> GPD0_1
 */
void led_initial(void)
{
	/* LED1 */
	writel(S5PV210_GPJ0CON, (readl(S5PV210_GPJ0CON) & ~(0xf<<12)) | (0x1<<12));
	writel(S5PV210_GPJ0PUD, (readl(S5PV210_GPJ0PUD) & ~(0x3<<6)) | (0x2<<6));
	writel(S5PV210_GPJ0DAT, (readl(S5PV210_GPJ0DAT) & ~(0x1<<3)) | (0x1<<3));

	/* LED2 */
	writel(S5PV210_GPJ0CON, (readl(S5PV210_GPJ0CON) & ~(0xf<<16)) | (0x1<<16));
	writel(S5PV210_GPJ0PUD, (readl(S5PV210_GPJ0PUD) & ~(0x3<<8)) | (0x2<<8));
	writel(S5PV210_GPJ0DAT, (readl(S5PV210_GPJ0DAT) & ~(0x1<<4)) | (0x1<<4));

	/* LED3 */
	writel(S5PV210_GPJ0CON, (readl(S5PV210_GPJ0CON) & ~(0xf<<20)) | (0x1<<20));
	writel(S5PV210_GPJ0PUD, (readl(S5PV210_GPJ0PUD) & ~(0x3<<10)) | (0x2<<10));
	writel(S5PV210_GPJ0DAT, (readl(S5PV210_GPJ0DAT) & ~(0x1<<5)) | (0x1<<5));

	/* LED4 */
	writel(S5PV210_GPD0CON, (readl(S5PV210_GPD0CON) & ~(0xf<<4)) | (0x1<<4));
	writel(S5PV210_GPD0PUD, (readl(S5PV210_GPD0PUD) & ~(0x3<<2)) | (0x2<<2));
	writel(S5PV210_GPD0DAT, (readl(S5PV210_GPD0DAT) & ~(0x1<<1)) | (0x1<<1));
}

void led_set_status(enum led_name name, enum led_status status)
{
	switch(name)
	{
	case LED_NAME_LED1:
		if(status == LED_STATUS_ON)
			writel(S5PV210_GPJ0DAT, (readl(S5PV210_GPJ0DAT) & ~(0x1<<3)) | (0x0<<3));
		else if(status == LED_STATUS_OFF)
			writel(S5PV210_GPJ0DAT, (readl(S5PV210_GPJ0DAT) & ~(0x1<<3)) | (0x1<<3));
		break;

	case LED_NAME_LED2:
		if(status == LED_STATUS_ON)
			writel(S5PV210_GPJ0DAT, (readl(S5PV210_GPJ0DAT) & ~(0x1<<4)) | (0x0<<4));
		else if(status == LED_STATUS_OFF)
			writel(S5PV210_GPJ0DAT, (readl(S5PV210_GPJ0DAT) & ~(0x1<<4)) | (0x1<<4));
		break;

	case LED_NAME_LED3:
		if(status == LED_STATUS_ON)
			writel(S5PV210_GPJ0DAT, (readl(S5PV210_GPJ0DAT) & ~(0x1<<5)) | (0x0<<5));
		else if(status == LED_STATUS_OFF)
			writel(S5PV210_GPJ0DAT, (readl(S5PV210_GPJ0DAT) & ~(0x1<<5)) | (0x1<<5));
		break;

	case LED_NAME_LED4:
		if(status == LED_STATUS_ON)
			writel(S5PV210_GPD0DAT, (readl(S5PV210_GPD0DAT) & ~(0x1<<1)) | (0x0<<1));
		else if(status == LED_STATUS_OFF)
			writel(S5PV210_GPD0DAT, (readl(S5PV210_GPD0DAT) & ~(0x1<<1)) | (0x1<<1));
		break;

	default:
		break;
	}
}
