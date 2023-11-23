#include "function.h"
#include "bsp_usart.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "stm32f10x_it.h"

extern uint8_t Flag;			  // ���ݰ��Ƿ���
extern uint8_t num[REC_BUF_SIZE]; // �洢��λ�����������ݰ�

/**************************************************************************
��������DATARecv

���ã�  ���жϽ��յ����ݰ�������INPUT�ṹ�壬�ṹ���DATARccv�ɸ���ʵ��������и���

����ֵ��INPUT���͵Ľṹ��

ʹ�ã�BL_Send(DEBUG_USARTx,mode,quan)
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

		// ����һ��char���͵�����
		structure.c = num[1];
		// ����һ��int��������
		for (i = 6; i >= 2; i--)
		{
			a |= (num[i] << ((i - 2) * 8));
		}
		structure.num = a;
	}
	a = 0;
	Flag = 0; // �������
	return structure;
}

/// @brief  ��������ķǳ������ݰ������ݲ��֣����޳���β�ֽ�
/// @param cha ԭʼ���ݰ� ������β��־�ֽ�
/// @return char����
char *ChaRecv(const char *cha)
{
	if (strlen(cha) <= 2)
	{
		// �������Ϊ�ջ򳤶�С�ڵ���2���޷�ɾ����β�ַ���ֱ�ӷ���ԭ�ַ����Ŀ���
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
��������BL_Send

���ã�  ��λ��������ʾ�����ӷ�����λ��������Ҫ���͵������ֽڣ��ڵ����������ý������ݰ�
				����1�����ڣ���������͵����ݣ��ɸ���ʵ��������и���

������(�������ͣ�Ҫ���͵Ĳ���1������2������3)���޸ĸ�����ͬʱҲҪ�޸ķ��͵��ֽھ�������ע�͵��Ĳ���

ʹ�ã�BL_Send(DEBUG_USARTx,mode,quan)
***************************************************************************/
void BL_Send(USART_TypeDef *pUSARTx, u8 send_ok)
{
	u8 sum = 0;					   // У��λ--�����ֽ�֮�͵ĵͰ�λ
	Usart_SendByte(pUSARTx, 0x8F); // ͷ
	///////////////////////////////////////����ģʽ
	Usart_SendByte(pUSARTx, send_ok);
	sum += send_ok;
	Usart_SendByte(pUSARTx, sum);  // У��λ
	Usart_SendByte(pUSARTx, 0xF8); // β
}

/// @brief ������������ ���������ݰ�
/// @param pUSARTx ��ʹ�ô��ں� ��USART2 USART1
/// @param send_arr ���������׵�ַ �������������ɣ����ݰ��Զ���װͷ��β
/// @param size ���鳤��
/// @param isCheck �Ƿ����У��λ 0 �����У�鼰��β 1 ���У�鼰��β
void BL_SendArr(USART_TypeDef *pUSARTx, u8 *send_arr, u8 size, u8 isCheck)
{
	uint8_t i = 0;
	u8 sum = 0; // У��λ--�����ֽ�֮�͵ĵͰ�λ
	if (isCheck)
	{
		Usart_SendByte(pUSARTx, 0x8F); // ͷ
		Usart_SendByte(pUSARTx, 0x00);//Ĭ���ַ� 0x32
	}

	for (i = 0; i < size; i++, send_arr++)
	{
		if (*send_arr != 0xf8 && *send_arr != 0x8f) // ��ȥͷβ
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
		Usart_SendByte(pUSARTx, sum);  // У��λ
		Usart_SendByte(pUSARTx, 0xF8); // β
	}
}
