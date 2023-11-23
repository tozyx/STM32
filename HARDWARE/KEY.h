#include "stm32f10x.h"
#ifndef _KEY_H
#define _KEY_H

void KEY_Init(void);
uint8_t KEY_GetNum(void);
uint8_t KEY_Turn(void);
uint8_t KEY_State(void);

#endif
