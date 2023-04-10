#ifndef __S5PV210_FB_H__
#define __S5PV210_FB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>
#include <graphic/surface.h>

void s5pv210_fb_initial(void);
struct surface_t * s5pv210_screen_surface(void);
void s5pv210_screen_swap(void);
void s5pv210_screen_flush(void);
void s5pv210_screen_backlight(u8_t brightness);

#ifdef __cplusplus
}
#endif

#endif /* __S5PV210_FB_H__ */
