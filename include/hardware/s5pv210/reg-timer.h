#ifndef __S5PV210_REG_TIMER_H__
#define __S5PV210_REG_TIMER_H__

/*
 * timer 0 ~ 4
 */
#define S5PV210_TCFG0	  			(0xe2500000)
#define S5PV210_TCFG1	  			(0xe2500004)
#define S5PV210_TCON	  			(0xe2500008)

#define S5PV210_TCNTB0	  			(0xe250000c)
#define S5PV210_TCMPB0	  			(0xe2500010)
#define S5PV210_TCNTO0	  			(0xe2500014)

#define S5PV210_TCNTB1	  			(0xe2500018)
#define S5PV210_TCMPB1	  			(0xe250001c)
#define S5PV210_TCNTO1	  			(0xe2500020)

#define S5PV210_TCNTB2	  			(0xe2500024)
#define S5PV210_TCMPB2	  			(0xe2500028)
#define S5PV210_TCNTO2	  			(0xe250002c)

#define S5PV210_TCNTB3	  			(0xe2500030)
#define S5PV210_TCMPB3	  			(0xe2500034)
#define S5PV210_TCNTO3	  			(0xe2500038)

#define S5PV210_TCNTB4	  			(0xe250003c)
#define S5PV210_TCNTO4	  			(0xe2500040)

#define S5PV210_TINT_CSTAT	  		(0xe2500044)

/*
 * system timer
 */
#define S5PV210_SYSTIMER_TCFG		(0xe2600000)
#define S5PV210_SYSTIMER_TCON		(0xe2600004)

#define S5PV210_SYSTIMER_TICNTB		(0xe2600008)
#define S5PV210_SYSTIMER_TICNTO		(0xe260000c)

#define S5PV210_SYSTIMER_TFCNTB		(0xe2600010)

#define S5PV210_SYSTIMER_ICNTB		(0xe2600018)
#define S5PV210_SYSTIMER_ICNTO		(0xe260001c)

#define S5PV210_SYSTIMER_TINT_CSTAT	(0xe2600020)


#endif /* __S5PV210_REG_TIMER_H__ */
