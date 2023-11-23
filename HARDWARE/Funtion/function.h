#ifndef __FUNCTION_H
#define __FUNCTION_H

#include "stm32f10x.h"
#include "bsp_usart.h"
#include "function.h"

typedef struct
{
	int num;
	char c;
} INPUT;

INPUT DATARecv(void);
void BL_Send(USART_TypeDef *pUSARTx, u8 send_ok); 
char *ChaRecv(const char *cha);
void BL_SendArr(USART_TypeDef *pUSARTx, u8 *array, u8 size, u8 isCheck);

#endif
