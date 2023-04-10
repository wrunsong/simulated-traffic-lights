#ifndef __S5PV210_IRQ_H__
#define __S5PV210_IRQ_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

typedef void (*interrupt_function_t)(void * data);

void s5pv210_irq_initial(void);
bool_t request_irq(const char * name, interrupt_function_t func, void * data);
bool_t free_irq(const char * name);

#ifdef __cplusplus
}
#endif

#endif /* __S5PV210_IRQ_H__ */
