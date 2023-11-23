#include "KEY.h"
#include "Delay.h"

void KEY_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// 按下并松开返回1 否则返回0
uint8_t KEY_GetNum()
{
    uint8_t KeyNum = 0;
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
            ;
        Delay_ms(20);
        KeyNum = 1;
    }
    return KeyNum;
}

// 返回当前状态1按下 0常开
uint8_t KEY_State()
{

    Delay_ms(20);
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0;
}

static int count_times = 0;
// 检测按下次数(按下抬起后+1)
uint8_t KEY_Turn()
{
    count_times *= (count_times != 100);
    if (KEY_GetNum() == 1)
    {
        count_times++;
    }

    return count_times % 2;
}
