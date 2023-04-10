/*
 * s5pv210-tick-delay.c
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
#include <s5pv210-tick.h>
#include <s5pv210-tick-delay.h>

static volatile u32_t loops_per_jiffy = 0;

void __attribute__ ((noinline)) __delay(volatile u32_t loop)
{
	for(; loop > 0; loop--);
}

void udelay(u32_t us)
{
	u32_t hz = get_system_hz();

	if(hz)
		__delay(us * loops_per_jiffy / (1000000 / hz));
	else
		__delay(us);
}

void mdelay(u32_t ms)
{
	u32_t hz = get_system_hz();

	if(hz)
		__delay(ms * loops_per_jiffy / (1000 / hz));
	else
		__delay(ms * 1000);
}

static void calibrate_delay(void)
{
	u32_t ticks, loopbit;
	s32_t lps_precision = 8;
	u32_t hz = get_system_hz();

	if(hz > 0)
	{
		loops_per_jiffy = (1<<12);

		while((loops_per_jiffy <<= 1) != 0)
		{
			/* wait for "start of" clock tick */
			ticks = jiffies;
			while (ticks == jiffies);

			/* go ... */
			ticks = jiffies;
			__delay(loops_per_jiffy);
			ticks = jiffies - ticks;

			if(ticks)
				break;
		}

		loops_per_jiffy >>= 1;
		loopbit = loops_per_jiffy;

		while(lps_precision-- && (loopbit >>= 1))
		{
			loops_per_jiffy |= loopbit;
			ticks = jiffies;
			while(ticks == jiffies);

			ticks = jiffies;
			__delay(loops_per_jiffy);

			/* longer than 1 tick */
			if(jiffies != ticks)
				loops_per_jiffy &= ~loopbit;
		}
	}
	else
	{
		loops_per_jiffy = 0;
	}
}

void s5pv210_tick_delay_initial(void)
{
	calibrate_delay();
}
