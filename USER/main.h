#ifndef _MAIN_H
#define _MAIN_H
#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "string.h"
#include "stdlib.h"
#include "LED.h"
#include "KEY.h"
#include "DH11.h"
#include "OLED.h"
#include "bsp_usart.h"
#include "function.h"
#include "Timer.h"
#include "u8g2.h"
#include "u8x8.h"

extern uint8_t Flag;// 数据包是否发送
extern uint8_t num[];
extern uint8_t cha;
extern uint8_t DH_Data[4];

#endif
