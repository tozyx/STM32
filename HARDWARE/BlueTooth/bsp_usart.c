#include "bsp_usart.h"
#include "math.h"
#include "OLED.h"
#include "string.h"
/*******************************************************
len�ĳ��ȴ���������ݰ����ֽ�13=1�ֽ�(��ͷ)+2�ֽ�(����)+8�ֽ�(2��int)+2(У��λ�Ͱ�β);
����Ҫ��������Ӧ���ݣ�Ȼ���ڵ��������������ݰ�
*******************************************************/
__IO uint8_t usart_value = 0; // ����һ���ֽ����ݵı���

uint8_t len = 0;		   // �������ݵ����鵱ǰ�±�
uint8_t num[REC_BUF_SIZE]; // ��Ž���һ�γ������ݰ�������
char cha[200];	   // ��ʱ��Ž���һ�ηǳ������ݰ��������ݰ���
uint8_t Flag = 0;		   // ���յ��������ݰ�����֮��Flag=1 �ǳ������ݰ�Flag=2�� ֻƾ�賤���ж�
static uint8_t f = 0;	   // ��0x8F��ʼ����0xF8���� 8f 12 f8
// �жϷ�����
void DEBUG_USART_IRQHandler(void)
{
	if (USART_GetITStatus(DEBUG_USARTx, USART_IT_RXNE))
	{
		// �����жϱ�־λ�仯

		usart_value = USART_ReceiveData(DEBUG_USARTx); // ����һ���ֽڵ�����

		if (usart_value == 0x8F) // ��0x8F��ʼ
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
	{ // ���յ���β���������ν���
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
	{ // ������ȴ������ݰ��ĳ��ȣ�Ҳ�������ν���
		f = 0;
		len = 0;
	}
	USART_ClearFlag(DEBUG_USARTx, USART_IT_RXNE); // ����жϱ�־λ
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
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
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
