#include "main.h"
uint8_t DH_Data[4];
extern uint8_t Flag; // 数据包是否发送

int main(void)
{

	Dht11_OutInSet(0);
	u8g2Init();
	loop();//开机动画
	USART_Config();
	Timer_Init();
	OLED_Init();
	EXTI_GenerateSWInterrupt(EXTI_Line1); // 软件触发外部中断
	App_Dht11_Result_Process(DH_Data);

	while (1)
	{
		OLED_START();
		if (Flag == 2) // 接收到指令数据包
		{
			// 发送温湿度
			BL_SendArr(USART2, DH_Data, sizeof(DH_Data), 1);
			Flag = 0;
		}
	}
}
