#include <types.h>
#include <io.h>
#include <stddef.h>
#include <s5pv210/reg-gpio.h>
#include <hw-key.h>

/*
 * POWER -> EINT1   -> GPH0_1
 * LEFT  -> EINT2   -> GPH0_2
 * DOWN  -> EINT3   -> GPH0_3
 * UP    -> KP_COL0 -> GPH2_0
 * RIGHT -> KP_COL1 -> GPH2_1
 * BACK  -> KP_COL2 -> GPH2_2
 * MENU  -> KP_COL3 -> GPH2_3
 */
void key_initial(void)
{
	/* POWER */
	writel(S5PV210_GPH0CON, (readl(S5PV210_GPH0CON) & ~(0xf<<8)) | (0x0<<4));
	writel(S5PV210_GPH0PUD, (readl(S5PV210_GPH0PUD) & ~(0x3<<4)) | (0x2<<2));

	/* LEFT */
	writel(S5PV210_GPH0CON, (readl(S5PV210_GPH0CON) & ~(0xf<<8)) | (0x0<<8));
	writel(S5PV210_GPH0PUD, (readl(S5PV210_GPH0PUD) & ~(0x3<<4)) | (0x2<<4));

	/* DOWN */
	writel(S5PV210_GPH0CON, (readl(S5PV210_GPH0CON) & ~(0xf<<12)) | (0x0<<12));
	writel(S5PV210_GPH0PUD, (readl(S5PV210_GPH0PUD) & ~(0x3<<6)) | (0x2<<6));

	/* UP */
	writel(S5PV210_GPH2CON, (readl(S5PV210_GPH2CON) & ~(0xf<<0)) | (0x0<<0));
	writel(S5PV210_GPH2PUD, (readl(S5PV210_GPH2PUD) & ~(0x3<<0)) | (0x2<<0));

	/* RIGHT */
	writel(S5PV210_GPH2CON, (readl(S5PV210_GPH2CON) & ~(0xf<<4))| (0x0<<4));
	writel(S5PV210_GPH2PUD, (readl(S5PV210_GPH2PUD) & ~(0x3<<2)) | (0x2<<2));

	/* BACK */
	writel(S5PV210_GPH2CON, (readl(S5PV210_GPH2CON) & ~(0xf<<8))| (0x0<<8));
	writel(S5PV210_GPH2PUD, (readl(S5PV210_GPH2PUD) & ~(0x3<<4)) | (0x2<<4));

	/* MENU */
	writel(S5PV210_GPH2CON, (readl(S5PV210_GPH2CON) & ~(0xf<<12))| (0x0<<12));
	writel(S5PV210_GPH2PUD, (readl(S5PV210_GPH2PUD) & ~(0x3<<6)) | (0x2<<6));
}

static u32_t __get_key_status(void)
{
	u32_t key = 0;

	if((readl(S5PV210_GPH0DAT) & (0x1<<1)) == 0)
		key |= KEY_NAME_POWER;

	if((readl(S5PV210_GPH0DAT) & (0x1<<2)) == 0)
		key |= KEY_NAME_LEFT;

	if((readl(S5PV210_GPH0DAT) & (0x1<<3)) == 0)
		key |= KEY_NAME_DOWN;

	if((readl(S5PV210_GPH2DAT) & (0x1<<0)) == 0)
		key |= KEY_NAME_UP;

	if((readl(S5PV210_GPH2DAT) & (0x1<<1)) == 0)
		key |= KEY_NAME_RIGHT;

	if((readl(S5PV210_GPH2DAT) & (0x1<<2)) == 0)
		key |= KEY_NAME_BACK;

	if((readl(S5PV210_GPH2DAT) & (0x1<<3)) == 0)
		key |= KEY_NAME_MENU;

	return key;
}

bool_t get_key_status(u32_t * key)
{
	static u32_t a = 0, b = 0, c = 0;

	a = __get_key_status();
	b = __get_key_status();
	c = __get_key_status();

	if((a == b) && (a == c))
	{
		*key = a;
		return TRUE;
	}

	return FALSE;
}

bool_t get_key_event(u32_t * keyup, u32_t * keydown)
{
	static u32_t key_old = 0x0;
	u32_t key;

	if(!get_key_status(&key))
		return FALSE;

	if(key != key_old)
	{
		*keyup = (key ^ key_old) & key_old;
		*keydown = (key ^ key_old) & key;
		key_old = key;

		return TRUE;
	}

	return FALSE;
}

