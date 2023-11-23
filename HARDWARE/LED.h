#ifndef _LED_H
#include "stm32f10x.h"
#define _LED_H
    void LED_Init(void);
    void LED_On(void);
    void LED_Off(void);
    void LED_Turn(void);
    void LED_Shine(uint8_t);
#endif
