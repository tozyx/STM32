#include "pic.h"
#include "stm32f10x.h"
#include "u8g2.h"
#include ""

u8g2_t u8g2;

void GPIO_SPI_Init(void)
{
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void u8g2Init(void)
{
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_stm32_gpio_and_delay);
    u8g2_InitDisplay(&u8g2);     // 根据所选的芯片进行初始化工作，初始化完成后，显示器处于关闭状态
    u8g2_SetPowerSave(&u8g2, 0); // 打开显示器
}

//显示数据和图片
void show_data(void)
{
    u8g2_FirstPage(void);
    do
    {
        u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
        u8g2_DrawXBMP(&u8g2, 0, 1, 32, 32, tem_logo);
        u8g2_DrawXBMP(&u8g2, 96, 1, 32, 32, wet_logo);
    } while (u8g2_NextPage(&u8g2));
}
