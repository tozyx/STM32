#include "pic.h"
#include "stm32f10x.h"
#include "u8g2.h"
#include "pic_pic.h"
#include "Timer.h"
#include "Delay.h"
#include "stdio.h"
#include "string.h"
#include "Gif.h"

u8g2_t u8g2;

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
                                  U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                                  U8X8_UNUSED void *arg_ptr)
{
    uint16_t n;
    switch (msg)
    {
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        __NOP();
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds

        for (n = 0; n < 320; n++)
        {
            __NOP();
        }
        break;
    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
        Delay_ms(1);
        break;
    case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
        Delay_us(5);
        break; // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_SPI_DATA:
        if (arg_int == 1) // arg_int=1: Input dir with pullup high for I2C clock pin
            GPIO_SetBits(GPIOB, GPIO_Pin_6);
        else if (arg_int == 0)
            GPIO_ResetBits(GPIOB, GPIO_Pin_6);
        break;
    case U8X8_MSG_GPIO_SPI_CLOCK:
        if (arg_int == 1) // arg_int=1: Input dir with pullup high for I2C clock pin
            GPIO_SetBits(GPIOB, GPIO_Pin_5);
        else if (arg_int == 0)
            GPIO_ResetBits(GPIOB, GPIO_Pin_5);
        break;
    case U8X8_MSG_GPIO_CS:
        if (arg_int == 1) // arg_int=1: Input dir with pullup high for I2C clock pin
            GPIO_SetBits(GPIOB, GPIO_Pin_9);
        else if (arg_int == 0)
            GPIO_ResetBits(GPIOB, GPIO_Pin_9);
        break;
    case U8X8_MSG_GPIO_DC:
        if (arg_int == 1) // arg_int=1: Input dir with pullup high for I2C clock pin
            GPIO_SetBits(GPIOB, GPIO_Pin_8);
        else if (arg_int == 0)
            GPIO_ResetBits(GPIOB, GPIO_Pin_8);
        break;
    case U8X8_MSG_GPIO_RESET:
        if (arg_int == 1) // arg_int=1: Input dir with pullup high for I2C clock pin
            GPIO_SetBits(GPIOB, GPIO_Pin_7);
        else if (arg_int == 0)
            GPIO_ResetBits(GPIOB, GPIO_Pin_7);
        break;
    case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
        break;
    default:
        u8x8_SetGPIOResult(u8x8, 1); // default return value
        break;
    }
    return 1;
}
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
    GPIO_SPI_Init();
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_stm32_gpio_and_delay);
    u8g2_InitDisplay(&u8g2);     // 根据所选的芯片进行初始化工作，初始化完成后，显示器处于关闭状态
    u8g2_SetPowerSave(&u8g2, 0); // 打开显示器
}

// 显示数据和图片
void show_data(void)
{
    u8g2_FirstPage(&u8g2);
    do
    {
        u8g2_DrawXBMP(&u8g2, 0, 1, 32, 32, tem_logo);
        u8g2_DrawXBMP(&u8g2, 96, 1, 32, 32, wet_logo);
        u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);

    } while (u8g2_NextPage(&u8g2));
}

void show_gif(int i)
{
    // drawBox目的是防止闪屏，所以绘制一个纯黑的矩形覆盖
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawBox(&u8g2, 0, 0, 128, 64);
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawXBMP(&u8g2, 0, 1, 32, 32, tem_logo);
    u8g2_DrawXBMP(&u8g2, 96, 1, 32, 32, wet_logo);
    u8g2_DrawXBMP(&u8g2, 32, 0, 64, 64, gif[i]);
    u8g2_SendBuffer(&u8g2);
}
void loop(void)
{
    int i = 0;
    for (; i < gif_length; ++i)
    {
        show_gif(i);
        Delay_ms(10);
    }
    u8g2_ClearDisplay(&u8g2);
    u8g2_ClearBuffer(&u8g2);
}
