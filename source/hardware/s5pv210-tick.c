/*
 * s5pv210-tick.c
 *
 * Copyright(c) 2007-2013 jianjun jiang <jerryjianjun@gmail.com>
 * official site: http://xboot.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <types.h>
#include <stddef.h>
#include <io.h>
#include <sizes.h>
#include <string.h>
#include <s5pv210-clk.h>
#include <s5pv210-irq.h>
#include <s5pv210/reg-timer.h>
#include <s5pv210-tick.h>

volatile u32_t jiffies = 0;
static u32_t tick_hz = 0;

static void timer_interrupt(void * data)
{
	/* tick count */
	jiffies++;

	/* clear interrupt status bit */
	writel(S5PV210_TINT_CSTAT, (readl(S5PV210_TINT_CSTAT) & ~(0x1f<<5)) | (0x01<<9));
}

void s5pv210_tick_initial(void)
{
	u64_t pclk;

	if(!clk_get_rate("psys-pclk", &pclk))
		return;

	if(!request_irq("TIMER4", timer_interrupt, 0))
		return;

	/* use pwm timer 4, prescaler for timer 4 is 16 */
	writel(S5PV210_TCFG0, (readl(S5PV210_TCFG0) & ~(0xff<<8)) | (0x0f<<8));

	/* select mux input for pwm timer4 is 1/2 */
	writel(S5PV210_TCFG1, (readl(S5PV210_TCFG1) & ~(0xf<<16)) | (0x01<<16));

	/* load value for 10 ms timeout */
	writel(S5PV210_TCNTB4, (u32_t)(pclk / (2 * 16 * 100)));

	/* auto load, manaual update of timer 4 and stop timer4 */
	writel(S5PV210_TCON, (readl(S5PV210_TCON) & ~(0x7<<20)) | (0x06<<20));

	/* enable timer4 interrupt and clear interrupt status bit */
	writel(S5PV210_TINT_CSTAT, (readl(S5PV210_TINT_CSTAT) & ~(0x1<<4)) | (0x01<<4) | (0x01<<9));

	/* start timer4 */
	writel(S5PV210_TCON, (readl(S5PV210_TCON) & ~(0x7<<20)) | (0x05<<20));

	/* initial system tick */
	tick_hz = 100;
	jiffies = 0;
}

u32_t get_system_hz(void)
{
	return tick_hz;
}

u64_t clock_gettime(void)
{
	if(get_system_hz() > 0)
		return (u64_t)jiffies * 1000000 / get_system_hz();

	return 0;
}
