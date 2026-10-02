#include "debug.h"
#include "epd_port.h"
#include "epd.h"
#include "epd_paint.h"


/*
 * UTF-8 字符串
 *
 * 注意：
 * 当前工程源码保持 GBK。
 * 中文显示字符串使用 \xNN 形式写入真正的 UTF-8 字节，
 * 这样不受源文件编码影响。
 */


/*
 * "墨水屏"
 *
 * 墨 U+58A8 -> E5 A2 A8
 * 水 U+6C34 -> E6 B0 B4
 * 屏 U+5C4F -> E5 B1 8F
 */
static const char Text_EPD_UTF8[] =
    "\xE5\xA2\xA8"
    "\xE6\xB0\xB4"
    "\xE5\xB1\x8F";


/*
 * "状态正常"
 *
 * 状 U+72B6 -> E7 8A B6
 * 态 U+6001 -> E6 80 81
 * 正 U+6B63 -> E6 AD A3
 * 常 U+5E38 -> E5 B8 B8
 */
static const char Text_Status_UTF8[] =
    "\xE7\x8A\xB6"
    "\xE6\x80\x81"
    "\xE6\xAD\xA3"
    "\xE5\xB8\xB8";


/*
 * "EPD 墨水屏 OK"
 */
static const char Text_Mix_UTF8[] =
    "EPD "
    "\xE5\xA2\xA8"
    "\xE6\xB0\xB4"
    "\xE5\xB1\x8F"
    " OK";


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
     * 标题
     */
    Paint_DrawUTF8String16x16(
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
        42,
        EPD_BLACK
    );


    /*
     * =============================================
     * UTF-8 中文
     * =============================================
     */

    /*
     * 墨水屏
     */
    Paint_DrawUTF8String16x16(
        12,
        60,
        Text_EPD_UTF8,
        EPD_BLACK
    );


    /*
     * 状态正常
     */
    Paint_DrawUTF8String16x16(
        12,
        90,
        Text_Status_UTF8,
        EPD_BLACK
    );


    /*
     * 中英文混排：
     *
     * EPD 墨水屏 OK
     */
    Paint_DrawUTF8String16x16(
        12,
        125,
        Text_Mix_UTF8,
        EPD_BLACK
    );


    /*
     * 英文数字
     */
    Paint_DrawUTF8String16x16(
        12,
        155,
        "SSD1608 200X200",
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