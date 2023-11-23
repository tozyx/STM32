#ifndef _DH11_H
#define _DH11_H

#define DH11__Set() GPIO_SetBits(GPIOA, GPIO_Pin_0)   // 置高电平
#define DH11__Clr() GPIO_ResetBits(GPIOA, GPIO_Pin_0) // 置低电平
#define DHT11_SDA_DATA() GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define DH11OUT 0 // 输出模式
#define DH11IN 1  // 输入模式

void Dht11_OutInSet(uint8_t);
void Drv_Dht11_Start(void);
void App_Dht11_Result_Process(uint8_t receData[]);

#endif
