#ifndef __FUNCTION_H
#define __FUNCTION_H

#include "stm32f10x.h"
#include "bsp_usart.h"
#include "function.h"
#include "u8g2.h"
#include "u8x8.h"

typedef struct
{
	int num;
	char c;
} INPUT;

INPUT DATARecv(void);
void BL_Send(USART_TypeDef *pUSARTx, uint8_t send_ok);
char *ChaRecv(const char *cha);
void BL_SendArr(USART_TypeDef *pUSARTx, uint8_t *array, uint8_t size, uint8_t isCheck);
void CharToAscll(char a[],int b[]);

#endif
