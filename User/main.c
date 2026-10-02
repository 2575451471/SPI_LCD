#include "debug.h"
#include "epd_port.h"
#include "epd.h"
#include "epd_paint.h"


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
     * 标题
     */
    Paint_DrawString8x16(
        12,
        15,
        "CH32V103",
        EPD_BLACK
    );


    /*
     * 分割线
     */
    Paint_DrawHLine(
        10,
        189,
        40,
        EPD_BLACK
    );


    /*
     * 屏幕信息
     */
    Paint_DrawString8x16(
        12,
        55,
        "EPD 200X200",
        EPD_BLACK
    );


    Paint_DrawString8x16(
        12,
        80,
        "SSD1608",
        EPD_BLACK
    );


    Paint_DrawString8x16(
        12,
        105,
        "STATUS: OK",
        EPD_BLACK
    );


    /*
     * 一个简单状态方块
     */
    Paint_FillRect(
        12,
        140,
        35,
        163,
        EPD_BLACK
    );


    Paint_DrawString8x16(
        48,
        144,
        "READY",
        EPD_BLACK
    );


    /*
     * =============================================
     * Display
     * =============================================
     */
    EPD_Display(
        Paint_GetBuffer()
    );


    while(1)
    {
        Delay_Ms(1000);
    }
}