#ifndef __FONT_CN_H__
#define __FONT_CN_H__

#include "debug.h"


#define FONT_CN16_WIDTH     16
#define FONT_CN16_HEIGHT    16


typedef struct
{
    /*
     * Unicode code point
     *
     * 例如：
     * 墨 = 0x58A8
     */
    uint16_t code;

    /*
     * 16 x 16 = 256 bit = 32 bytes
     *
     * 每一行2字节，共16行
     *
     * bit7 -> 左侧
     * bit0 -> 右侧
     */
    uint8_t data[32];

} FontCN16_t;


/*
 * 查找中文字模
 *
 * 找到：
 *     返回32字节字模
 *
 * 找不到：
 *     返回0
 */
const uint8_t *FontCN16_Get(uint16_t code);


#endif