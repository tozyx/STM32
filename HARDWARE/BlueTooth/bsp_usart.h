#ifndef __BSP_USART_H
#define __BSP_USART_H
#include "stm32f10x.h"
#include <stdio.h>

#define REC_BUF_SIZE 8    //数据包大小

#define DEBUG_USARTx                     USART2   //蓝牙所用串口2
#define DEBUG_USART_CLK                  RCC_APB1Periph_USART2  //串口时钟
#define DEBUG_USART_APBxClkCmd           RCC_APB1PeriphClockCmd //串口时钟使能
#define DEBUG_USART_BAUDRATE             9600 //波特率

#define DEBUG_USART_GPIO_CLK             RCC_APB2Periph_GPIOA   

#define DEBUG_USART_GPIO_APBxClkCmd       RCC_APB2PeriphClockCmd  

#define DEBUG_USART_TX_GPIO_PORT         GPIOA              //串口2
#define DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_2
#define DEBUG_USART_RX_GPIO_PORT         GPIOA
#define DEBUG_USART_RX_GPIO_PIN          GPIO_Pin_3
#define DEBUG_BULETOOTH_STATE_PORT       GPIOA              //蓝牙状态口
#define DEBUG_BULETOOTH_STATE_PIN        GPIO_Pin_1


#define DEBUG_USART_IRQ                  USART2_IRQn
#define DEBUG_USART_IRQHandler           USART2_IRQHandler   //中断函数

static void NVIC_Config(void);
void USART_Config(void);
void Usart_SendByte(USART_TypeDef*pUSARTx,uint8_t data);

#endif 

