#include "bsp_usart.h"
#include "math.h"
#include "OLED.h"
#include "string.h"
/*******************************************************
len的长度代表接收数据包的字节13=1字节(包头)+2字节(按键)+8字节(2个int)+2(校验位和包尾);
根据要求设置相应数据，然后在调试器上设置数据包
*******************************************************/
__IO uint8_t usart_value = 0; // 接收一个字节数据的变量

uint8_t len = 0;		   // 接收数据的数组当前下标
uint8_t num[REC_BUF_SIZE]; // 存放接收一次常规数据包的数组
char cha[200];	   // 暂时存放接受一次非常规数据包（短数据包）
uint8_t Flag = 0;		   // 接收到常规数据包数据之后Flag=1 非常规数据包Flag=2， 只凭借长度判断
static uint8_t f = 0;	   // 从0x8F开始接收0xF8结束 8f 12 f8
// 中断服务函数
void DEBUG_USART_IRQHandler(void)
{
	if (USART_GetITStatus(DEBUG_USARTx, USART_IT_RXNE))
	{
		// 接收中断标志位变化

		usart_value = USART_ReceiveData(DEBUG_USARTx); // 接收一个字节的数据

		if (usart_value == 0x8F) // 从0x8F开始
		{
			len = 0;
			f = 1;
		}
		if (f == 1)
		{
			num[len] = usart_value;
			len++;
		}
	}
	if (len == REC_BUF_SIZE && usart_value == 0xF8)
	{ // 接收到包尾，结束本次接收
		Flag = 1;
		len = 0;
		f = 0;
	}
	else if (len < REC_BUF_SIZE && usart_value == 0xf8)
	{
		memcpy(cha, num, len);
		Flag = 2;
		len = 0;
		f = 0;
	}

	else if (len > REC_BUF_SIZE)
	{ // 如果长度大于数据包的长度，也结束本次接收
		f = 0;
		len = 0;
	}
	USART_ClearFlag(DEBUG_USARTx, USART_IT_RXNE); // 清除中断标志位
}

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;

	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}
//
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);

	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(DEBUG_USARTx, &USART_InitStructure);

	NVIC_Config();
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	USART_Cmd(DEBUG_USARTx, ENABLE);
}
//
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t data)
{
	USART_SendData(pUSARTx, data);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;
}
