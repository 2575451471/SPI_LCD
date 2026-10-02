#ifndef __EPD_PAINT_H__
#define __EPD_PAINT_H__

#include "epd.h"


#define EPD_BUFFER_SIZE    ((EPD_WIDTH * EPD_HEIGHT) / 8)


void Paint_Clear(uint8_t color);

void Paint_SetPixel(uint16_t x,
                    uint16_t y,
                    uint8_t color);

void Paint_DrawHLine(uint16_t x0,
                     uint16_t x1,
                     uint16_t y,
                     uint8_t color);

void Paint_DrawVLine(uint16_t x,
                     uint16_t y0,
                     uint16_t y1,
                     uint8_t color);

void Paint_FillRect(uint16_t x0,
                    uint16_t y0,
                    uint16_t x1,
                    uint16_t y1,
                    uint8_t color);

uint8_t *Paint_GetBuffer(void);

void Paint_DrawChar(uint16_t x,
                    uint16_t y,
                    char ch,
                    uint8_t color);

void Paint_DrawString(uint16_t x,
                      uint16_t y,
                      const char *str,
                      uint8_t color);


#endif