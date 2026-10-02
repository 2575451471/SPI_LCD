#ifndef __FONT_H__
#define __FONT_H__

#include "debug.h"


/*
 * 基础字模为 8x8
 *
 * 绘制时每一行纵向重复两次，
 * 最终显示尺寸为 8x16。
 */
#define FONT8_WIDTH        8
#define FONT8_HEIGHT       8

#define FONT8X16_WIDTH     8
#define FONT8X16_HEIGHT    16


typedef struct
{
    char ch;

    /*
     * 一行一个字节
     *
     * bit7 -> 左侧
     * bit0 -> 右侧
     */
    uint8_t data[8];

} Font8x8_t;


/*
 * 根据 ASCII 字符取得字模。
 *
 * 找不到时返回空格字模。
 */
const uint8_t *Font8x8_Get(char ch);


#endif