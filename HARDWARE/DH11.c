#include "stm32f10x.h"
#include "DH11.h"
#include "Delay.h"
#include "LED.h"
#include "OLED.h"

/// @brief 初始化dh11,接口：GPIOA:0
/// @param state 1浮空输入；0推挽输出
void Dht11_OutInSet(uint8_t state)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;
    // 初始化GPIO结构体
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    if (state)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 浮空输入
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽式输出
    }
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    DH11__Set();
}

// 向DH11发送起始信号
void Drv_Dht11_Start(void)
{
    Dht11_OutInSet(DH11OUT); // GPIO配置为输出模式
    DH11__Clr();             // 拉低一小段时间
    Delay_ms(18);

    DH11__Set(); // 拉高一小段时间
    Delay_us(30);

    Dht11_OutInSet(DH11IN); // GPIO配置为输入模式
}

// 接收一帧数据
uint8_t Med_Dht11_ReceOneByte(void)
{
    uint8_t tempVar = 0;  // 临时循环变量
    uint8_t receData = 0; // 接收数据

    for (tempVar = 0; tempVar < 8; tempVar++)
    {
        while (!DHT11_SDA_DATA())
            ;         // 等待低电平过去
        Delay_us(40); // 延时40us之后判断是0还是1

        // 如果40us之后依旧为高电平
        if (DHT11_SDA_DATA())
        {
            while (DHT11_SDA_DATA())
                ;                                         // 等待高电平过去
            receData |= (uint8_t)(0x01 << (7 - tempVar));
        }
        else
        {
            receData &= (uint8_t) ~(0x01 << (7 - tempVar)); 
        }
    }

    return receData;
}

/// @brief 读取dh11数据，储存数据到数组
/// @param receData 传入数组名(地址)
void App_Dht11_Result_Process(uint8_t receData[])
{
    uint8_t receCheck = 0; // 接收到的校验码
    uint8_t tempCal = 0;   // 临时计算变量

    Drv_Dht11_Start(); // 发送一个起始信号
    // DH11__Set();       // 拉高SDA

    // 等待应答信号
    if (!DHT11_SDA_DATA())
    {
        while (!DHT11_SDA_DATA())
            ; // 等待83us低电平结束
        while (DHT11_SDA_DATA())
            ; // 等待87us高电平结束

        // 开始接收数据
        receData[0] = Med_Dht11_ReceOneByte(); // 湿度整数
        receData[1] = Med_Dht11_ReceOneByte(); // 湿度小数
        receData[2] = Med_Dht11_ReceOneByte(); // 温度整数
        receData[3] = Med_Dht11_ReceOneByte(); // 温度小数
        receCheck = Med_Dht11_ReceOneByte();   // 校验码
        tempCal = receData[0] + receData[1] + receData[2] + receData[3];
        Dht11_OutInSet(DH11OUT);
        DH11__Set();
        //  如果校验失败
        if (tempCal != receCheck)
        {
            // 清空接收
            receData[0] = receData[1] = receData[2] = receData[3] = 0;
        }
    }
    //没有应答信号 传感器错误
    else
    {
        receData[0] = receData[1] = receData[2] = receData[3] = 0;
       // OLED_ShowString(1,5,"SENSOR ERROR");
    }
}
