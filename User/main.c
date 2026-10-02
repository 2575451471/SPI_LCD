#include "debug.h"
#include "epd_port.h"
#include "epd.h"
#include "epd_paint.h"


/*
 * 墨水屏
 */
static const uint16_t Text_EPD[] =
{
    0x58A8,
    0x6C34,
    0x5C4F,
    0x0000
};


/*
 * 状态正常
 */
static const uint16_t Text_Status[] =
{
    0x72B6,
    0x6001,
    0x6B63,
    0x5E38,
    0x0000
};


int main(void)
{
    SystemCoreClockUpdate();

    Delay_Init();

    USART_Printf_Init(115200);


    /*
     * Hardware
     */
    EPD_GPIO_Init();

    EPD_SPI_Init();


    /*
     * SSD1608
     */
    EPD_Init();


    /*
     * =============================================
     * Framebuffer
     * =============================================
     */
    Paint_Clear(EPD_WHITE);


    /*
     * 外边框
     */
    Paint_DrawHLine(
        0,
        199,
        0,
        EPD_BLACK
    );

    Paint_DrawHLine(
        0,
        199,
        199,
        EPD_BLACK
    );

    Paint_DrawVLine(
        0,
        0,
        199,
        EPD_BLACK
    );

    Paint_DrawVLine(
        199,
        0,
        199,
        EPD_BLACK
    );


    /*
     * English title
     */
    Paint_DrawString8x16(
        12,
        15,
        "CH32V103",
        EPD_BLACK
    );


    /*
     * separator
     */
    Paint_DrawHLine(
        10,
        189,
        42,
        EPD_BLACK
    );


    /*
     * 中文：墨水屏
     */
    Paint_DrawChineseString16x16(
        12,
        60,
        Text_EPD,
        EPD_BLACK
    );


    /*
     * 中文：状态正常
     */
    Paint_DrawChineseString16x16(
        12,
        90,
        Text_Status,
        EPD_BLACK
    );


    /*
     * English information
     */
    Paint_DrawString8x16(
        12,
        125,
        "SSD1608",
        EPD_BLACK
    );


    Paint_DrawString8x16(
        12,
        155,
        "200X200",
        EPD_BLACK
    );


    /*
     * Display
     */
    EPD_Display(
        Paint_GetBuffer()
    );


    while(1)
    {
        Delay_Ms(1000);
    }
}