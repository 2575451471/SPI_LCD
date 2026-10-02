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
     * 清 framebuffer 为白色
     */
    Paint_Clear(EPD_WHITE);


    /*
     * 外框
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
     * 字符测试
     */
    Paint_DrawString(
        20,
        20,
        "EPD OK",
        EPD_BLACK
    );


    Paint_DrawString(
        20,
        40,
        "0123",
        EPD_BLACK
    );


    /*
     * 再画一个参考方块
     */
    Paint_FillRect(
        20,
        70,
        60,
        110,
        EPD_BLACK
    );


    /*
     * 一次性刷新
     */
    EPD_Display(
        Paint_GetBuffer()
    );


    while(1)
    {
        Delay_Ms(1000);
    }
}