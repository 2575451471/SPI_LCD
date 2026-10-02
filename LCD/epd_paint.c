#include "epd_paint.h"


/*
 * 200 x 200 / 8
 * = 5000 bytes
 *
 * 1 = white
 * 0 = black
 */
static uint8_t EPD_Buffer[EPD_BUFFER_SIZE];

typedef struct
{
    char ch;
    uint8_t data[5];
} Font5x7_t;


/*
 * 每个字符 5 列 × 7 行
 *
 * 每个字节表示一列，
 * bit0 对应顶部像素。
 */
static const Font5x7_t Font5x7[] =
{
    {
        'E',
        {
            0x7F,
            0x49,
            0x49,
            0x49,
            0x41
        }
    },

    {
        'P',
        {
            0x7F,
            0x09,
            0x09,
            0x09,
            0x06
        }
    },

    {
        'D',
        {
            0x7F,
            0x41,
            0x41,
            0x22,
            0x1C
        }
    },

    {
        'O',
        {
            0x3E,
            0x41,
            0x41,
            0x41,
            0x3E
        }
    },

    {
        'K',
        {
            0x7F,
            0x08,
            0x14,
            0x22,
            0x41
        }
    },

    {
        '0',
        {
            0x3E,
            0x51,
            0x49,
            0x45,
            0x3E
        }
    },

    {
        '1',
        {
            0x00,
            0x42,
            0x7F,
            0x40,
            0x00
        }
    },

    {
        '2',
        {
            0x42,
            0x61,
            0x51,
            0x49,
            0x46
        }
    },

    {
        '3',
        {
            0x21,
            0x41,
            0x45,
            0x4B,
            0x31
        }
    }
};

#define FONT5X7_COUNT \
    (sizeof(Font5x7) / sizeof(Font5x7[0]))


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
 * @fn      Paint_DrawChar
 *
 * @brief   Draw one 5x7 character
 *********************************************************************/
void Paint_DrawChar(uint16_t x,
                    uint16_t y,
                    char ch,
                    uint8_t color)
{
    uint16_t i;
    uint8_t col;
    uint8_t row;
    uint8_t column_data;


    /*
     * 查找字符
     */
    for(i = 0; i < FONT5X7_COUNT; i++)
    {
        if(Font5x7[i].ch == ch)
        {
            break;
        }
    }


    /*
     * 字库中没有这个字符
     */
    if(i >= FONT5X7_COUNT)
    {
        return;
    }


    /*
     * 5列
     */
    for(col = 0; col < 5; col++)
    {
        column_data = Font5x7[i].data[col];


        /*
         * 每列7行
         */
        for(row = 0; row < 7; row++)
        {
            if(column_data & (1 << row))
            {
                Paint_SetPixel(
                    x + col,
                    y + row,
                    color
                );
            }
        }
    }
}


/*********************************************************************
 * @fn      Paint_DrawString
 *
 * @brief   Draw 5x7 ASCII string
 *
 *          每个字符占 6 pixels:
 *          5 pixel字符 + 1 pixel间隔
 *********************************************************************/
void Paint_DrawString(uint16_t x,
                      uint16_t y,
                      const char *str,
                      uint8_t color)
{
    while(*str != '\0')
    {
        /*
         * 空格单独处理
         */
        if(*str != ' ')
        {
            Paint_DrawChar(
                x,
                y,
                *str,
                color
            );
        }


        /*
         * 5 pixels 字符
         * +
         * 1 pixel 间隔
         */
        x += 6;

        str++;


        /*
         * 防止越过屏幕
         */
        if(x >= EPD_WIDTH)
        {
            break;
        }
    }
}