#ifndef __EPD_PORT_H__
#define __EPD_PORT_H__

#include "debug.h"

/* ============================================================
 * CH32V103C8T6 <-> E-Paper
 *
 * RST  -> PA1
 * DC   -> PA2
 * BUSY -> PA3
 * CS   -> PA4
 * SCK  -> PA5  SPI1_SCK
 * SDA  -> PA7  SPI1_MOSI
 * ============================================================ */


/* RST */
#define EPD_RST_PORT       GPIOA
#define EPD_RST_PIN        GPIO_Pin_1

/* DC */
#define EPD_DC_PORT        GPIOA
#define EPD_DC_PIN         GPIO_Pin_2

/* BUSY */
#define EPD_BUSY_PORT      GPIOA
#define EPD_BUSY_PIN       GPIO_Pin_3

/* CS1 */
#define EPD_CS_PORT        GPIOA
#define EPD_CS_PIN         GPIO_Pin_4

/* SPI1 SCK */
#define EPD_SCK_PORT       GPIOA
#define EPD_SCK_PIN        GPIO_Pin_5

/* SPI1 MOSI / SDA / DIN */
#define EPD_MOSI_PORT      GPIOA
#define EPD_MOSI_PIN       GPIO_Pin_7


/* GPIO control */

#define EPD_RST_HIGH()     (EPD_RST_PORT->BSHR = EPD_RST_PIN)
#define EPD_RST_LOW()      (EPD_RST_PORT->BCR  = EPD_RST_PIN)

#define EPD_DC_HIGH()      (EPD_DC_PORT->BSHR = EPD_DC_PIN)
#define EPD_DC_LOW()       (EPD_DC_PORT->BCR  = EPD_DC_PIN)

#define EPD_CS_HIGH()      (EPD_CS_PORT->BSHR = EPD_CS_PIN)
#define EPD_CS_LOW()       (EPD_CS_PORT->BCR  = EPD_CS_PIN)

#define EPD_BUSY_READ() \
    GPIO_ReadInputDataBit(EPD_BUSY_PORT, EPD_BUSY_PIN)


/* GPIO */
void EPD_GPIO_Init(void);

/* SPI1 */
void EPD_SPI_Init(void);
void EPD_SPI_WriteByte(uint8_t data);

#endif