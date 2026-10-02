#include "epd_port.h"


/*********************************************************************
 * @fn      EPD_GPIO_Init
 *
 * @brief   Initialize E-Paper control GPIO
 *
 *          PA1 -> RST
 *          PA2 -> DC
 *          PA3 -> BUSY
 *          PA4 -> CS
 *
 * @return  none
 *********************************************************************/
void EPD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /*
     * 所有控制线都在 GPIOA
     */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


    /*
     * PA1 -> RST
     * PA2 -> DC
     * PA4 -> CS
     *
     * 配置为普通推挽输出
     */
    GPIO_InitStructure.GPIO_Pin =
            EPD_RST_PIN |
            EPD_DC_PIN  |
            EPD_CS_PIN;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;

    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /*
     * PA3 -> BUSY
     *
     * BUSY 是墨水屏输出给 MCU 的信号，
     * 所以 MCU 端配置为输入。
     */
    GPIO_InitStructure.GPIO_Pin  = EPD_BUSY_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /*
     * 默认状态
     *
     * CS  = High -> 不选中墨水屏
     * DC  = High -> 默认数据状态
     * RST = High -> 不复位
     */
    EPD_CS_HIGH();
    EPD_DC_HIGH();
    EPD_RST_HIGH();
}


/*********************************************************************
 * @fn      EPD_SPI_Init
 *
 * @brief   Initialize SPI1 for E-Paper
 *
 *          PA5 -> SPI1_SCK
 *          PA7 -> SPI1_MOSI
 *
 *          PA6/MISO is not used.
 *
 * @return  none
 *********************************************************************/
void EPD_SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef  SPI_InitStructure  = {0};


    /*
     * 开启 GPIOA 和 SPI1 时钟
     *
     * SPI1 位于 APB2
     */
    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA |
        RCC_APB2Periph_SPI1,
        ENABLE
    );


    /*
     * PA5 -> SPI1_SCK
     * PA7 -> SPI1_MOSI
     *
     * 两个都是 SPI1 复用推挽输出
     */
    GPIO_InitStructure.GPIO_Pin =
            EPD_SCK_PIN |
            EPD_MOSI_PIN;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;

    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /*
     * 防止 SPI1 之前被其他代码配置过
     */
    SPI_I2S_DeInit(SPI1);


    /*
     * 墨水屏只需要 MCU -> EPD 单向发送
     */
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;

    /*
     * CH32V103 为主机
     */
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

    /*
     * 一个字节 8 bit
     */
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;

    /*
     * SPI Mode 0
     *
     * CPOL = 0
     * CPHA = first edge
     */
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;

    /*
     * CS 使用 PA4 手动控制
     */
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;


    /*
     * 你的 Demo 默认系统时钟是 72MHz。
     *
     * 调试阶段先使用较低 SPI 时钟。
     *
     * 72MHz / 16 = 4.5MHz 左右
     *
     * 后面屏幕完全稳定以后可以再提高。
     */
    SPI_InitStructure.SPI_BaudRatePrescaler =
        SPI_BaudRatePrescaler_16;


    /*
     * 墨水屏 SPI 一般最高位先传
     */
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;

    SPI_InitStructure.SPI_CRCPolynomial = 7;


    /*
     * 写入 SPI1 配置
     */
    SPI_Init(SPI1, &SPI_InitStructure);


    /*
     * 启动 SPI1
     */
    SPI_Cmd(SPI1, ENABLE);
}


/*********************************************************************
 * @fn      EPD_SPI_WriteByte
 *
 * @brief   Send one byte through SPI1
 *
 * @param   data - byte to send
 *
 * @return  none
 *********************************************************************/
void EPD_SPI_WriteByte(uint8_t data)
{
    /*
     * 等待发送寄存器为空
     */
    while(SPI_I2S_GetFlagStatus(
              SPI1,
              SPI_I2S_FLAG_TXE) == RESET)
    {
    }


    /*
     * 发送一个字节
     */
    SPI_I2S_SendData(SPI1, data);


    /*
     * 等 SPI 真正发送完成
     */
    while(SPI_I2S_GetFlagStatus(
              SPI1,
              SPI_I2S_FLAG_BSY) == SET)
    {
    }
}


