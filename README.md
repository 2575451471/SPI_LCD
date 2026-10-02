# CH32V103 SSD1608 E-Paper Driver

CH32V103C8T6 + HINK-E0154A05 1.54" 200x200 monochrome E-Paper.

Controller: SSD1608

## Current Status

Working:

- SPI1 communication
- SSD1608 initialization
- BUSY detection
- Full refresh
- Full white / full black display
- 200x200 1-bit framebuffer
- Pixel drawing
- Horizontal / vertical lines
- Filled rectangles
- Basic 5x7 ASCII characters

## Hardware

MCU:

- CH32V103C8T6

Display:

- HINK-E0154A05
- 1.54 inch
- 200 x 200
- Black / White
- SSD1608

## Wiring

| E-Paper | CH32V103 |
|---|---|
| RST | PA1 |
| DC | PA2 |
| BUSY | PA3 |
| CS1 | PA4 |
| SCK | PA5 |
| SDA / DIN | PA7 |

SPI:

- SPI1
- PA5 = SPI1_SCK
- PA7 = SPI1_MOSI
- PA4 = software-controlled CS

BUSY:

- HIGH = Busy
- LOW = Idle

## Coordinate System

With the FPC/interface facing downward:

- `(0, 0)` = top-left
- X increases to the right
- Y increases downward

## Current Test

Current framebuffer test successfully displays:

- large square at top-left
- horizontal bar at top-right
- vertical bar at bottom-left
- small square at bottom-right
- basic ASCII text

## Development Environment

- MounRiver Studio
- CH32V103
- C