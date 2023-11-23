#include "stm32f10x.h"
#include "LED.h"
#include "Delay.h"

void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 使能时钟
    // 定义结构体以便初始化gpio
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure); // 初始化gpioc
}

// 开灯
void LED_On(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

// 关灯
void LED_Off(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

// led取反
void LED_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == 0)
    {
        LED_Off();
    }
    else
    {
        LED_On();
    }

    // GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) ^ 0x01));
}

/// @brief 呼吸灯
/// @param conti 使能，1有效 0失效
void LED_Shine(uint8_t conti)
{
    if (conti==1)
    {
        int i = 0;
        int Time_Shine = 15; // 闪烁频率
        //int Time_Delay = 1;  // 呼吸频率
        for (; i < Time_Shine; i++)
        {
            Delay_ms(i);
            LED_Off();
            Delay_ms(Time_Shine - i);
            LED_On();
        }
        Delay_ms(10);//延长常亮时间
        for (; i > 0; i--)
        {

            Delay_ms(i);
            LED_Off();
            Delay_ms(Time_Shine - i);
            LED_On();
        }
        LED_Off();
        Delay_ms(500);
    }
    LED_Off();
}
