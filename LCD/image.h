#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "debug.h"


#define IMAGE_WIDTH     200
#define IMAGE_HEIGHT    200

#define IMAGE_SIZE \
    ((IMAGE_WIDTH * IMAGE_HEIGHT) / 8)


/*
 * ≤‚ ‘Õº∆¨
 *
 * 200 x 200
 * 1 bit per pixel
 *
 * 1 = white
 * 0 = black
 */
extern const uint8_t Image_Test[IMAGE_SIZE];


#endif