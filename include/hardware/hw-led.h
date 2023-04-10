#ifndef __HW_LED_H__
#define __HW_LED_H__

#ifdef __cplusplus
extern "C" {
#endif

enum led_name {
	LED_NAME_LED1		= 1,
	LED_NAME_LED2		= 2,
	LED_NAME_LED3		= 3,
	LED_NAME_LED4		= 4,
};

enum led_status {
	LED_STATUS_OFF		= 0,
	LED_STATUS_ON		= 1,
};

void led_initial(void);
void led_set_status(enum led_name name, enum led_status status);

#ifdef __cplusplus
}
#endif

#endif /* __HW_LED_H__ */
