#include "utf8.h"


/*********************************************************************
 * @fn      UTF8_Decode
 *
 * @brief   Decode one UTF-8 character
 *
 * @return
 *          number of UTF-8 bytes consumed
 *
 *          1 -> ASCII
 *          2 -> 2-byte UTF-8
 *          3 -> 3-byte UTF-8
 *          0 -> invalid / end
 *********************************************************************/
uint8_t UTF8_Decode(const char *str,
                    uint16_t *code)
{
    const uint8_t *p;


    if(str == 0 || code == 0)
    {
        return 0;
    }


    p = (const uint8_t *)str;


    /*
     * 字符串结束
     */
    if(p[0] == 0)
    {
        return 0;
    }


    /*
     * =============================================
     * ASCII
     *
     * 0xxxxxxx
     * =============================================
     */
    if((p[0] & 0x80) == 0x00)
    {
        *code = p[0];

        return 1;
    }


    /*
     * =============================================
     * UTF-8 两字节
     *
     * 110xxxxx
     * 10xxxxxx
     * =============================================
     */
    if((p[0] & 0xE0) == 0xC0)
    {
        /*
         * 检查后续字节
         */
        if((p[1] & 0xC0) != 0x80)
        {
            return 0;
        }


        *code =
            ((uint16_t)(p[0] & 0x1F) << 6)
            |
            ((uint16_t)(p[1] & 0x3F));


        return 2;
    }


    /*
     * =============================================
     * UTF-8 三字节
     *
     * 1110xxxx
     * 10xxxxxx
     * 10xxxxxx
     *
     * 中文通常就在这里
     * =============================================
     */
    if((p[0] & 0xF0) == 0xE0)
    {
        /*
         * 检查后续两个字节
         */
        if((p[1] & 0xC0) != 0x80 ||
           (p[2] & 0xC0) != 0x80)
        {
            return 0;
        }


        *code =
            ((uint16_t)(p[0] & 0x0F) << 12)
            |
            ((uint16_t)(p[1] & 0x3F) << 6)
            |
            ((uint16_t)(p[2] & 0x3F));


        return 3;
    }


    /*
     * 当前暂不支持 UTF-8 四字节字符
     *
     * 例如 emoji：
     * ?
     *
     * 它们超出了 uint16_t 范围。
     */
    return 0;
}