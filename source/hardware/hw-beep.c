#include <types.h>
#include <io.h>
#include <s5pv210/reg-gpio.h>
#include <hw-beep.h>

/*
 * Beep Pin -> GPD0_2
 */
void beep_initial(void)
{
	writel(S5PV210_GPD0CON, (readl(S5PV210_GPD0CON) & ~(0xf<<8)) | (0x1<<8));
	writel(S5PV210_GPD0PUD, (readl(S5PV210_GPD0PUD) & ~(0x3<<4)) | (0x2<<4));
	writel(S5PV210_GPD0DAT, (readl(S5PV210_GPD0DAT) & ~(0x1<<2)) | (0x0<<2));
}

void beep_set_status(enum beep_status status)
{
	if(status == BEEP_STATUS_ON)
		writel(S5PV210_GPD0DAT, (readl(S5PV210_GPD0DAT) & ~(0x1<<2)) | (0x1<<2));
	else if(status == BEEP_STATUS_OFF)
		writel(S5PV210_GPD0DAT, (readl(S5PV210_GPD0DAT) & ~(0x1<<2)) | (0x0<<2));
}
