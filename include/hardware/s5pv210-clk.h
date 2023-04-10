#ifndef __S5PV210_CLK_H__
#define __S5PV210_CLK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

void s5pv210_clk_initial(void);
bool_t clk_get_rate(const char * name, u64_t * rate);

#ifdef __cplusplus
}
#endif

#endif /* __S5PV210_CLK_H__ */
