#ifndef __EPD_H__
#define __EPD_H__

#include "epd_port.h"

#define EPD_WIDTH       200
#define EPD_HEIGHT      200

#define EPD_WHITE       0xFF
#define EPD_BLACK       0x00

/*
 * ============================================================
 * HINK-E0154A05
 * 1.54 inch
 * 200 x 200
 * B/W
 * Controller: SSD1608
 *
 * BUSY:
 *      HIGH -> Busy
 *      LOW  -> Idle
 * ============================================================
 */


/*********************************************************************
 * Hardware reset
 *********************************************************************/
void EPD_Reset(void);


/*********************************************************************
 * Send one command byte
 *********************************************************************/
void EPD_WriteCommand(uint8_t command);


/*********************************************************************
 * Send one data byte
 *********************************************************************/
void EPD_WriteData(uint8_t data);


/*********************************************************************
 * Read BUSY pin
 *
 * return:
 *      0 -> LOW
 *      1 -> HIGH
 *********************************************************************/
uint8_t EPD_ReadBusy(void);


/*********************************************************************
 * Wait until E-Paper becomes idle
 *
 * SSD1608:
 *      BUSY = 1 -> Busy
 *      BUSY = 0 -> Idle
 *
 * timeout_ms:
 *      maximum waiting time in milliseconds
 *
 * return:
 *      0 -> success
 *      1 -> timeout
 *********************************************************************/
uint8_t EPD_WaitBusy(uint32_t timeout_ms);


/* SSD1608 driver */
uint8_t EPD_Init(void);

void EPD_SetWindow(uint16_t x_start,
                   uint16_t y_start,
                   uint16_t x_end,
                   uint16_t y_end);

void EPD_SetCursor(uint16_t x, uint16_t y);

uint8_t EPD_Refresh(void);

uint8_t EPD_Clear(uint8_t color);

uint8_t EPD_Display(const uint8_t *image);

void EPD_Sleep(void);

#endif /* __EPD_H__ */