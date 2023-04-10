#ifndef __S5PV210_TICK_H__
#define __S5PV210_TICK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

/*
 * When HZ = 1000, The Max delay is 24 days
 */
#define time_after(a, b)		(((s32_t)(b) - (s32_t)(a) < 0))
#define time_before(a, b)		time_after(b, a)
#define time_after_eq(a,b)		(((s32_t)(a) - (s32_t)(b) >= 0))
#define time_before_eq(a,b)		time_after_eq(b, a)

extern volatile u32_t jiffies;

void s5pv210_tick_initial(void);
u32_t get_system_hz(void);
u64_t clock_gettime(void);

#ifdef __cplusplus
}
#endif

#endif /* __S5PV210_TICK_H__ */
