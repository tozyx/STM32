#include "main.h"

extern uint8_t Flag;// 数据包是否发送
extern uint8_t num[];
extern uint8_t cha;
uint8_t DH_Data[4];



int main(void)
{
	LED_Init(); // 初始化gpioc
	// KEY_Init(); // 初始化gpioa
	Dht11_OutInSet(0);
	OLED_Init();
	USART_Config();
	OLED_ShowChinese(1, 1, 0); //"你"
	OLED_ShowChinese(1, 2, 1); //"好"
	OLED_ShowChinese(2, 1, 3); //"温"
	OLED_ShowChinese(3, 1, 2); //"湿"
	OLED_ShowChinese(3, 7, 5); //"℃"
	OLED_ShowChinese(2, 7, 6); //"％"
	OLED_ShowChinese(2, 2, 4); //"度"
	OLED_ShowChinese(3, 2, 4); //"度"
	OLED_ShowChar(3, 6, ':');
	OLED_ShowChar(2, 6, ':');
	OLED_ShowChar(3, 10, '.');
	OLED_ShowChar(2, 10, '.');
	// OLED_ShowString(1, 3, "HelloWorld!");
	// OLED_ShowNum(2, 1, 12345, 5);
	// OLED_ShowSignedNum(2, 7, -66, 2);
	// OLED_ShowHexNum(3, 1, 0xAA55, 4);
	// OLED_ShowBinNum(4, 1, 0xAA55, 16);
	// uint8_t KeyNum;
	uint8_t *initMessage = {"init bluetooth successfully"};
	BL_SendArr(USART2, initMessage, strlen(initMessage), 0);
	LED_Off();
	while (1)
	{


		// KeyNum = KEY_GetNum();
		// int i;
		// i=KEY_State()^1;
		// LED_Shine(i);
		Delay_ms(1000);
		
		App_Dht11_Result_Process(DH_Data);
		OLED_ShowNum(2, 8, DH_Data[0], 2);
		OLED_ShowNum(2, 11, DH_Data[1], 1);
		OLED_ShowNum(3, 8, DH_Data[2], 2);
		OLED_ShowNum(3, 11, DH_Data[3], 1);
		if (Flag == 1)//接受到常规数据包
		{
			INPUT Rec;
			Rec = DATARecv();

			if (Rec.c == 0)
				LED_On();
			else
				LED_Shine(1);
			OLED_ShowHexNum(1, 6, num[2], 2);
			OLED_ShowHexNum(1, 8, num[3], 2);
			OLED_ShowHexNum(1, 10, num[4], 2);
			OLED_ShowHexNum(1, 12, num[5], 2);
		}else if (Flag ==2)
		{
			char* recChar = ChaRecv(cha);
			//OLED_ShowHexNum(1,5,num[1],2);
			BL_SendArr(USART2, DH_Data,sizeof(DH_Data), 1);

			
		}
		
	}
}
