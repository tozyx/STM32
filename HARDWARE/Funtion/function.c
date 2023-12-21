#include "function.h"
#include "bsp_usart.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "stm32f10x_it.h"
#include "Delay.h"

extern uint8_t Flag;			  // 数据包是否发送
extern uint8_t num[REC_BUF_SIZE]; // 存储上位机发出的数据包

/**************************************************************************
函数名：DATARecv

作用：  将中断接收的数据包导出到INPUT结构体，结构体和DATARccv可根据实际情况进行更改

返回值：INPUT类型的结构体

使用：BL_Send(DEBUG_USARTx,mode,quan)
***************************************************************************/
INPUT DATARecv()
{
	int a = 0;
	uint8_t i;
	INPUT structure;

	if (num[0] != 0x8f)
	{
	}
	else
	{

		// 接收一个char类型的数据
		structure.c = num[1];
		// 接收一个int整形数据
		for (i = 6; i >= 2; i--)
		{
			a |= (num[i] << ((i - 2) * 8));
		}
		structure.num = a;
	}
	a = 0;
	Flag = 0; // 接收完成
	return structure;
}

/// @brief  返回输入的非常规数据包的数据部分，即剔除首尾字节
/// @param cha 原始数据包 含有首尾标志字节
/// @return char数组
char *ChaRecv(const char *cha)
{
	if (strlen(cha) <= 2)
	{
		// 如果输入为空或长度小于等于2，无法删除首尾字符，直接返回原字符串的拷贝
		return (char *)cha;
	}
	uint8_t newlength = strlen(cha) - 2;
	char *ChaRecv = (char *)malloc((newlength + 1) * sizeof(char));
	if (ChaRecv != NULL)
	{
		strncpy(ChaRecv, cha + 1, newlength);
	}
	Flag = 0;
	return ChaRecv;
}

/**************************************************************************
函数名：BL_Send

作用：  上位机数据显示，板子发送上位机，根据要发送的数据字节，在调试器上设置接收数据包
				参数1代表串口，后面代表发送的数据，可根据实际情况进行更改

参数：(串口类型，要发送的参数1，参数2，参数3)可修改个数，同时也要修改发送的字节就是下面注释掉的部分

使用：BL_Send(DEBUG_USARTx,mode,quan)
***************************************************************************/
void BL_Send(USART_TypeDef *pUSARTx, uint8_t send_ok)
{
	uint8_t sum = 0;			   // 校验位--数据字节之和的低八位
	Usart_SendByte(pUSARTx, 0x8F); // 头
	///////////////////////////////////////发送模式
	Usart_SendByte(pUSARTx, send_ok);
	sum += send_ok;
	Usart_SendByte(pUSARTx, sum);  // 校验位
	Usart_SendByte(pUSARTx, 0xF8); // 尾
}

/// @brief 发送数组数据 非正常数据包
/// @param pUSARTx 所使用串口号 如USART2 USART1
/// @param send_arr 传输数组首地址 传入数组名即可，数据包自动封装头，尾
/// @param size 数组长度
/// @param isCheck 是否添加校验位 0 不添加校验及首尾 1 添加校验及首尾
void BL_SendArr(USART_TypeDef *pUSARTx, uint8_t *send_arr, uint8_t size, uint8_t isCheck)
{
	uint8_t i = 0;
	uint8_t sum = 0; // 校验位--数组字节之和的低八位
	if (isCheck)
	{
		Usart_SendByte(pUSARTx, 0x8F); // 头
		Usart_SendByte(pUSARTx, 0x00); // 默认字符 0x32
	}

	for (i = 0; i < size; i++, send_arr++)
	{
		if (*send_arr != 0xf8 && *send_arr != 0x8f) // 除去头尾
		{
			Usart_SendByte(pUSARTx, *send_arr);
			sum += *send_arr;
		}
		else if (*send_arr == 0x8f)
		{
			continue;
		}
		else if (*send_arr == 0xf8)
		{
			break;
		}
	}
	if (isCheck)
	{
		Usart_SendByte(pUSARTx, sum);  // 校验位
		Usart_SendByte(pUSARTx, 0xF8); // 尾
	}
}


uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
								  U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
								  U8X8_UNUSED void *arg_ptr)
{
	uint16_t n;
	switch (msg)
	{
	case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
		__NOP();
		break;
	case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds

		for (n = 0; n < 320; n++)
		{
			__NOP();
		}
		break;
	case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
		Delay_ms(1);
		break;
	case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
		Delay_us(5);
		break; // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
	case U8X8_MSG_GPIO_SPI_DATA:
		if (arg_int == 1) // arg_int=1: Input dir with pullup high for I2C clock pin
			GPIO_SetBits(GPIOB, GPIO_Pin_6);
		else if (arg_int == 0)
			GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		break;
	case U8X8_MSG_GPIO_SPI_CLOCK:
		if (arg_int == 1) // arg_int=1: Input dir with pullup high for I2C clock pin
			GPIO_SetBits(GPIOB, GPIO_Pin_5);
		else if (arg_int == 0)
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		break;
	case U8X8_MSG_GPIO_CS:
		if (arg_int == 1) // arg_int=1: Input dir with pullup high for I2C clock pin
			GPIO_SetBits(GPIOB, GPIO_Pin_9);
		else if (arg_int == 0)
			GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		break;
	case U8X8_MSG_GPIO_DC:
		if (arg_int == 1) // arg_int=1: Input dir with pullup high for I2C clock pin
			GPIO_SetBits(GPIOB, GPIO_Pin_8);
		else if (arg_int == 0)
			GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		break;
	case U8X8_MSG_GPIO_RESET:
		if (arg_int == 1) // arg_int=1: Input dir with pullup high for I2C clock pin
			GPIO_SetBits(GPIOB, GPIO_Pin_7);
		else if (arg_int == 0)
			GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		break;
	case U8X8_MSG_GPIO_MENU_SELECT:
		u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
		break;
	case U8X8_MSG_GPIO_MENU_NEXT:
		u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
		break;
	case U8X8_MSG_GPIO_MENU_PREV:
		u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
		break;
	case U8X8_MSG_GPIO_MENU_HOME:
		u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
		break;
	default:
		u8x8_SetGPIOResult(u8x8, 1); // default return value
		break;
	}
	return 1;
}
