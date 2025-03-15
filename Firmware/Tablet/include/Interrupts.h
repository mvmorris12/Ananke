#include "stdint.h"

#ifndef __EXTINT_H 
#define __EXTINT_H

#define EINT0		0x00000001
#define EINT1		0x00000002
#define EINT2		0x00000004
#define EINT3		0x00000008

#define EINT0_EDGE	0x00000001
#define EINT1_EDGE	0x00000002
#define EINT2_EDGE	0x00000004
#define EINT3_EDGE	0x00000008

#define EINT0_RISING	0x00000001
#define EINT1_RISING	0x00000002
#define EINT2_RISING	0x00000004
#define EINT3_RISING	0x00000008

extern void EINT0_IRQHandler(void);
extern uint32_t EINTInit( void );
extern void interrupt_test(void);
extern void lcd_touch_interrupt_enable(void);
extern void lcd_touch_interrupt_disable(void);
extern void esp32_slave_interrupt_enable(void);
void rtc_interrupt_enable(void);
void lcd_vfp_interrupt_enable(void);
void timer0_interrupt_enable(void);
void timer1_interrupt_enable(void);
#endif /* end __EXTINT_H */