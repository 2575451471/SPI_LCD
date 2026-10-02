#include "epd.h"


/*********************************************************************
 * @fn      EPD_Reset
 *
 * @brief   Hardware reset E-Paper
 *
 *          RST:
 *              HIGH
 *              LOW
 *              HIGH
 *
 * @return  none
 *********************************************************************/
void EPD_Reset(void)
{
    /*
     * 先保持正常高电平
     */
    EPD_RST_HIGH();

    Delay_Ms(200);


    /*
     * 拉低 RST
     */
    EPD_RST_LOW();

    Delay_Ms(5);


    /*
     * 释放复位
     */
    EPD_RST_HIGH();

    /*
     * 等待控制器从硬件复位中恢复
     */
    Delay_Ms(200);
}


/*********************************************************************
 * @fn      EPD_WriteCommand
 *
 * @brief   Send one command byte to SSD1608
 *
 *          DC = LOW  -> Command
 *          CS = LOW  -> Select E-Paper
 *
 * @param   command - command byte
 *
 * @return  none
 *********************************************************************/
void EPD_WriteCommand(uint8_t command)
{
    /*
     * 命令模式
     */
    EPD_DC_LOW();


    /*
     * 选中墨水屏
     */
    EPD_CS_LOW();


    /*
     * SPI1 发送一个字节
     */
    EPD_SPI_WriteByte(command);


    /*
     * 释放墨水屏
     */
    EPD_CS_HIGH();
}


/*********************************************************************
 * @fn      EPD_WriteData
 *
 * @brief   Send one data byte to SSD1608
 *
 *          DC = HIGH -> Data
 *          CS = LOW  -> Select E-Paper
 *
 * @param   data - data byte
 *
 * @return  none
 *********************************************************************/
void EPD_WriteData(uint8_t data)
{
    /*
     * 数据模式
     */
    EPD_DC_HIGH();


    /*
     * 选中墨水屏
     */
    EPD_CS_LOW();


    /*
     * SPI1 发送数据
     */
    EPD_SPI_WriteByte(data);


    /*
     * 释放墨水屏
     */
    EPD_CS_HIGH();
}


/*********************************************************************
 * @fn      EPD_ReadBusy
 *
 * @brief   Read SSD1608 BUSY pin
 *
 * @return
 *          0 -> BUSY LOW
 *          1 -> BUSY HIGH
 *********************************************************************/
uint8_t EPD_ReadBusy(void)
{
    if(EPD_BUSY_READ() == Bit_SET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*********************************************************************
 * @fn      EPD_WaitBusy
 *
 * @brief   Wait SSD1608 until idle
 *
 *          SSD1608:
 *
 *          BUSY = HIGH -> Controller busy
 *          BUSY = LOW  -> Controller idle
 *
 * @param   timeout_ms - maximum waiting time
 *
 * @return
 *          0 -> success
 *          1 -> timeout
 *********************************************************************/
uint8_t EPD_WaitBusy(uint32_t timeout_ms)
{
    uint32_t count = 0;


    /*
     * SSD1608:
     *
     * BUSY = 1 时持续等待
     */
    while(EPD_ReadBusy() == 1)
    {
        Delay_Ms(1);

        count++;


        /*
         * 防止硬件接线错误、屏幕异常等情况下
         * 程序永久卡死在这里。
         */
        if(count >= timeout_ms)
        {
            return 1;
        }
    }


    /*
     * BUSY 已经变成 LOW
     */
    return 0;
}



/*
 * SSD1608 Full Refresh LUT
 *
 * 来源：Waveshare 1.54inch e-Paper V1 reference driver
 */
static const uint8_t EPD_LUT_FULL_UPDATE[30] =
{
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};


/*********************************************************************
 * @fn      EPD_SetLUT
 *
 * @brief   Load SSD1608 waveform LUT
 *********************************************************************/
static void EPD_SetLUT(void)
{
    uint8_t i;

    EPD_WriteCommand(0x32);

    for(i = 0; i < 30; i++)
    {
        EPD_WriteData(EPD_LUT_FULL_UPDATE[i]);
    }
}


/*********************************************************************
 * @fn      EPD_Init
 *
 * @brief   Initialize HINK-E0154A05 / SSD1608
 *
 * @return  0 success
 *********************************************************************/
uint8_t EPD_Init(void)
{
    EPD_Reset();


    /*
     * 0x01 DRIVER_OUTPUT_CONTROL
     *
     * 200 lines -> 199 = 0x00C7
     */
    EPD_WriteCommand(0x01);

    EPD_WriteData((EPD_HEIGHT - 1) & 0xFF);
    EPD_WriteData(((EPD_HEIGHT - 1) >> 8) & 0xFF);
    EPD_WriteData(0x00);


    /*
     * 0x0C BOOSTER_SOFT_START_CONTROL
     */
    EPD_WriteCommand(0x0C);

    EPD_WriteData(0xD7);
    EPD_WriteData(0xD6);
    EPD_WriteData(0x9D);


    /*
     * 0x2C WRITE_VCOM_REGISTER
     */
    EPD_WriteCommand(0x2C);
    EPD_WriteData(0xA8);


    /*
     * 0x3A SET_DUMMY_LINE_PERIOD
     */
    EPD_WriteCommand(0x3A);
    EPD_WriteData(0x1A);


    /*
     * 0x3B SET_GATE_TIME
     */
    EPD_WriteCommand(0x3B);
    EPD_WriteData(0x08);


    /*
     * 0x11 DATA_ENTRY_MODE_SETTING
     *
     * X increment
     * Y increment
     */
    EPD_WriteCommand(0x11);
    EPD_WriteData(0x03);


    /*
     * Full refresh waveform
     */
    EPD_SetLUT();


    /*
     * Set whole screen RAM window
     */
    EPD_SetWindow(
        0,
        0,
        EPD_WIDTH - 1,
        EPD_HEIGHT - 1
    );


    /*
     * RAM cursor -> (0,0)
     */
    EPD_SetCursor(0, 0);


    return 0;
}


/*********************************************************************
 * @fn      EPD_SetWindow
 *
 * @brief   Set SSD1608 RAM area
 *********************************************************************/
void EPD_SetWindow(uint16_t x_start,
                   uint16_t y_start,
                   uint16_t x_end,
                   uint16_t y_end)
{
    /*
     * X address
     *
     * SSD1608 X RAM is byte addressed.
     * 200 pixels / 8 = 25 bytes.
     */
    EPD_WriteCommand(0x44);

    EPD_WriteData((x_start >> 3) & 0xFF);
    EPD_WriteData((x_end >> 3) & 0xFF);


    /*
     * Y address
     */
    EPD_WriteCommand(0x45);

    EPD_WriteData(y_start & 0xFF);
    EPD_WriteData((y_start >> 8) & 0xFF);

    EPD_WriteData(y_end & 0xFF);
    EPD_WriteData((y_end >> 8) & 0xFF);
}


/*********************************************************************
 * @fn      EPD_SetCursor
 *
 * @brief   Set current SSD1608 RAM cursor
 *********************************************************************/
void EPD_SetCursor(uint16_t x, uint16_t y)
{
    /*
     * X counter
     */
    EPD_WriteCommand(0x4E);

    EPD_WriteData((x >> 3) & 0xFF);


    /*
     * Y counter
     */
    EPD_WriteCommand(0x4F);

    EPD_WriteData(y & 0xFF);
    EPD_WriteData((y >> 8) & 0xFF);
}


/*********************************************************************
 * @fn      EPD_Refresh
 *
 * @brief   Start full display refresh
 *
 * @return  0 success
 *          1 BUSY timeout
 *********************************************************************/
uint8_t EPD_Refresh(void)
{
    /*
     * DISPLAY_UPDATE_CONTROL_2
     */
    EPD_WriteCommand(0x22);
    EPD_WriteData(0xC4);


    /*
     * MASTER_ACTIVATION
     */
    EPD_WriteCommand(0x20);


    /*
     * TERMINATE_FRAME_READ_WRITE
     */
    EPD_WriteCommand(0xFF);


    /*
     * Full refresh normally takes about seconds,
     * give it a generous timeout.
     */
    if(EPD_WaitBusy(10000))
    {
        return 1;
    }

    return 0;
}


/*********************************************************************
 * @fn      EPD_Clear
 *
 * @brief   Fill whole screen with one color
 *
 * @param
 *      0xFF -> white
 *      0x00 -> black
 *
 * @return
 *      0 success
 *      1 BUSY timeout
 *********************************************************************/
uint8_t EPD_Clear(uint8_t color)
{
    uint16_t y;
    uint16_t x;


    EPD_SetWindow(
        0,
        0,
        EPD_WIDTH - 1,
        EPD_HEIGHT - 1
    );


    /*
     * 200 pixels / 8 = 25 bytes per line
     *
     * We deliberately write line by line for the first test.
     * This follows the conservative reference-driver method.
     */
    for(y = 0; y < EPD_HEIGHT; y++)
    {
        EPD_SetCursor(0, y);


        /*
         * WRITE_RAM
         */
        EPD_WriteCommand(0x24);


        for(x = 0; x < (EPD_WIDTH / 8); x++)
        {
            EPD_WriteData(color);
        }
    }


    return EPD_Refresh();
}


/*********************************************************************
 * @fn      EPD_Display
 *
 * @brief   Display 200x200 / 1-bit image
 *
 *          image size must be:
 *
 *          200 * 200 / 8 = 5000 bytes
 *
 *          bit:
 *              1 -> white
 *              0 -> black
 *
 * @return
 *      0 success
 *      1 BUSY timeout
 *********************************************************************/
uint8_t EPD_Display(const uint8_t *image)
{
    uint16_t y;
    uint16_t x;
    uint32_t offset;


    if(image == 0)
    {
        return 1;
    }


    EPD_SetWindow(
        0,
        0,
        EPD_WIDTH - 1,
        EPD_HEIGHT - 1
    );


    for(y = 0; y < EPD_HEIGHT; y++)
    {
        EPD_SetCursor(0, y);

        EPD_WriteCommand(0x24);


        offset = (uint32_t)y * (EPD_WIDTH / 8);


        for(x = 0; x < (EPD_WIDTH / 8); x++)
        {
            EPD_WriteData(image[offset + x]);
        }
    }


    return EPD_Refresh();
}


/*********************************************************************
 * @fn      EPD_Sleep
 *
 * @brief   SSD1608 deep sleep
 *
 *          After this function, hardware reset / EPD_Init()
 *          is required before next display update.
 *********************************************************************/
void EPD_Sleep(void)
{
    EPD_WriteCommand(0x10);
    EPD_WriteData(0x01);

    Delay_Ms(200);
}