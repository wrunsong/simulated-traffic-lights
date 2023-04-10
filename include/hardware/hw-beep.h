#ifndef __HW_BEEP_H__
#define __HW_BEEP_H__

#ifdef __cplusplus
extern "C" {
#endif

enum beep_status {
	BEEP_STATUS_OFF		= 0,
	BEEP_STATUS_ON		= 1,
};

void beep_initial(void);
void beep_set_status(enum beep_status status);

#ifdef __cplusplus
}
#endif

#endif /* __HW_BEEP_H__ */
