#ifndef __S5PV210_TICK_DELAY_H__
#define __S5PV210_TICK_DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

void s5pv210_tick_delay_initial(void);
void udelay(u32_t us);
void mdelay(u32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* __S5PV210_TICK_DELAY_H__ */
