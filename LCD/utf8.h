#ifndef __UTF8_H__
#define __UTF8_H__

#include "debug.h"


/*
 * 从 UTF-8 字符串中解析一个 Unicode 字符
 *
 * str:
 *     当前 UTF-8 字节位置
 *
 * code:
 *     输出 Unicode 编码
 *
 * 返回值:
 *     1 -> ASCII
 *     2 -> UTF-8 两字节字符
 *     3 -> UTF-8 三字节字符
 *     0 -> 无效字符 / 字符串结束
 *
 * 当前项目 Unicode 使用 uint16_t，
 * 因此主要支持 BMP 范围：
 *
 * U+0000 ~ U+FFFF
 *
 * 中文常用汉字完全够用。
 */
uint8_t UTF8_Decode(const char *str,
                    uint16_t *code);


#endif