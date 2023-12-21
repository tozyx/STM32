#include "main.h"

int main(void)
{

	Dht11_OutInSet(0);
	// OLED_Init();
	GPIO_SPI_Init();
	USART_Config();
	Timer_Init();
	EXTI_GenerateSWInterrupt(EXTI_Line1); // 软件触发外部中断
	App_Dht11_Result_Process(DH_Data);
	
	while (1)
	{
		// OLED_START();

		if (Flag == 2) // 接收到指令数据包
		{
			// 发送温湿度
			BL_SendArr(USART2, DH_Data, sizeof(DH_Data), 1);
			Flag = 0;
		}
	}
}
