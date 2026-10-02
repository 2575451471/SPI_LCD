#include "epd_paint.h"


/*
 * 200 x 200 / 8
 * = 5000 bytes
 *
 * 1 = white
 * 0 = black
 */
static uint8_t EPD_Buffer[EPD_BUFFER_SIZE];




/*********************************************************************
 * @fn      Paint_GetBuffer
 *
 * @brief   Return framebuffer pointer
 *********************************************************************/
uint8_t *Paint_GetBuffer(void)
{
    return EPD_Buffer;
}


/*********************************************************************
 * @fn      Paint_Clear
 *
 * @brief   Clear whole framebuffer
 *
 *          EPD_WHITE = 0xFF
 *          EPD_BLACK = 0x00
 *********************************************************************/
void Paint_Clear(uint8_t color)
{
    uint32_t i;

    for(i = 0; i < EPD_BUFFER_SIZE; i++)
    {
        EPD_Buffer[i] = color;
    }
}


/*********************************************************************
 * @fn      Paint_SetPixel
 *
 * @brief   Draw one pixel
 *
 *          framebuffer:
 *
 *          byte0:
 *              bit7 -> x=0
 *              bit6 -> x=1
 *              ...
 *              bit0 -> x=7
 *
 *          1 -> white
 *          0 -> black
 *********************************************************************/
void Paint_SetPixel(uint16_t x,
                    uint16_t y,
                    uint8_t color)
{
    uint32_t index;
    uint8_t mask;


    /*
     * Boundary protection
     */
    if(x >= EPD_WIDTH || y >= EPD_HEIGHT)
    {
        return;
    }


    /*
     * 25 bytes per row
     */
    index =
        ((uint32_t)y * (EPD_WIDTH / 8))
        + (x / 8);


    /*
     * x=0 -> bit7
     * x=1 -> bit6
     * ...
     */
    mask = 0x80 >> (x % 8);


    if(color == EPD_BLACK)
    {
        /*
         * Clear bit -> black
         */
        EPD_Buffer[index] &= ~mask;
    }
    else
    {
        /*
         * Set bit -> white
         */
        EPD_Buffer[index] |= mask;
    }
}


/*********************************************************************
 * Horizontal line
 *********************************************************************/
void Paint_DrawHLine(uint16_t x0,
                     uint16_t x1,
                     uint16_t y,
                     uint8_t color)
{
    uint16_t x;

    if(x0 > x1)
    {
        uint16_t temp = x0;
        x0 = x1;
        x1 = temp;
    }

    for(x = x0; x <= x1; x++)
    {
        Paint_SetPixel(x, y, color);
    }
}


/*********************************************************************
 * Vertical line
 *********************************************************************/
void Paint_DrawVLine(uint16_t x,
                     uint16_t y0,
                     uint16_t y1,
                     uint8_t color)
{
    uint16_t y;

    if(y0 > y1)
    {
        uint16_t temp = y0;
        y0 = y1;
        y1 = temp;
    }

    for(y = y0; y <= y1; y++)
    {
        Paint_SetPixel(x, y, color);
    }
}


/*********************************************************************
 * Filled rectangle
 *********************************************************************/
void Paint_FillRect(uint16_t x0,
                    uint16_t y0,
                    uint16_t x1,
                    uint16_t y1,
                    uint8_t color)
{
    uint16_t x;
    uint16_t y;


    if(x0 > x1)
    {
        uint16_t temp = x0;
        x0 = x1;
        x1 = temp;
    }

    if(y0 > y1)
    {
        uint16_t temp = y0;
        y0 = y1;
        y1 = temp;
    }


    for(y = y0; y <= y1; y++)
    {
        for(x = x0; x <= x1; x++)
        {
            Paint_SetPixel(x, y, color);
        }
    }
}



/*********************************************************************
 * @fn      Paint_DrawChar8x16
 *
 * @brief   Draw ASCII character in 8x16 size
 *
 *          原始字模:
 *              8 x 8
 *
 *          显示:
 *              8 x 16
 *
 *          方法:
 *              每一行纵向绘制两次
 *********************************************************************/
void Paint_DrawChar8x16(uint16_t x,
                        uint16_t y,
                        char ch,
                        uint8_t color)
{
    const uint8_t *font;

    uint8_t row;
    uint8_t col;

    uint8_t row_data;


    /*
     * 获得 8x8 字模
     */
    font = Font8x8_Get(ch);


    /*
     * 共 8 行原始数据
     */
    for(row = 0; row < 8; row++)
    {
        row_data = font[row];


        /*
         * 每行 8 个像素
         */
        for(col = 0; col < 8; col++)
        {
            if(row_data & (0x80 >> col))
            {
                /*
                 * 每一行重复两次
                 *
                 * 原 row=0
                 * ->
                 * y=0
                 * y=1
                 */
                Paint_SetPixel(
                    x + col,
                    y + row * 2,
                    color
                );

                Paint_SetPixel(
                    x + col,
                    y + row * 2 + 1,
                    color
                );
            }
        }
    }
}


/*********************************************************************
 * @fn      Paint_DrawString8x16
 *
 * @brief   Draw ASCII string
 *
 *          一个字符:
 *              8 pixels wide
 *
 *          字符间距:
 *              1 pixel
 *
 *          实际步进:
 *              9 pixels
 *********************************************************************/
void Paint_DrawString8x16(uint16_t x,
                          uint16_t y,
                          const char *str,
                          uint8_t color)
{
    while(*str != '\0')
    {
        /*
         * 防止超出右边界
         */
        if((x + FONT8X16_WIDTH) > EPD_WIDTH)
        {
            break;
        }


        /*
         * 防止超出下边界
         */
        if((y + FONT8X16_HEIGHT) > EPD_HEIGHT)
        {
            break;
        }


        Paint_DrawChar8x16(
            x,
            y,
            *str,
            color
        );


        /*
         * 8 pixel 字符
         * +
         * 1 pixel 间隔
         */
        x += 9;

        str++;
    }
}



/*********************************************************************
 * @fn      Paint_DrawChinese16x16
 *
 * @brief   Draw one Chinese character
 *
 *          16 pixels x 16 pixels
 *********************************************************************/
void Paint_DrawChinese16x16(uint16_t x,
                            uint16_t y,
                            uint16_t code,
                            uint8_t color)
{
    const uint8_t *font;

    uint8_t row;
    uint8_t col;

    uint8_t left;
    uint8_t right;


    /*
     * 防止超出屏幕范围
     */
    if((x + 16) > EPD_WIDTH ||
       (y + 16) > EPD_HEIGHT)
    {
        return;
    }


    /*
     * 查询字模
     */
    font = FontCN16_Get(code);


    /*
     * 找不到这个汉字
     */
    if(font == 0)
    {
        return;
    }


    /*
     * 每个汉字16行
     *
     * 每行：
     *      2 bytes
     */
    for(row = 0; row < 16; row++)
    {
        left  = font[row * 2];
        right = font[row * 2 + 1];


        /*
         * 左8个像素
         */
        for(col = 0; col < 8; col++)
        {
            if(left & (0x80 >> col))
            {
                Paint_SetPixel(
                    x + col,
                    y + row,
                    color
                );
            }
        }


        /*
         * 右8个像素
         */
        for(col = 0; col < 8; col++)
        {
            if(right & (0x80 >> col))
            {
                Paint_SetPixel(
                    x + 8 + col,
                    y + row,
                    color
                );
            }
        }
    }
}


/*********************************************************************
 * @fn      Paint_DrawChineseString16x16
 *
 * @brief   Draw Unicode Chinese character array
 *
 *          string ends with 0x0000
 *
 *          每个汉字：
 *              16 pixels
 *
 *          间距：
 *              2 pixels
 *
 *          步进：
 *              18 pixels
 *********************************************************************/
void Paint_DrawChineseString16x16(uint16_t x,
                                  uint16_t y,
                                  const uint16_t *str,
                                  uint8_t color)
{
    while(*str != 0x0000)
    {
        /*
         * 防止超出右边界
         */
        if((x + 16) > EPD_WIDTH)
        {
            break;
        }


        Paint_DrawChinese16x16(
            x,
            y,
            *str,
            color
        );


        /*
         * 16 pixel字符
         * +
         * 2 pixel间距
         */
        x += 18;

        str++;
    }
}