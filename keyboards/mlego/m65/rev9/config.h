// Copyright 2020-2023 alin m elena (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#define MATRIX_ROWS 5
#define MATRIX_COLS 14

/*
ShiftRegister SN74HC595N

1
QB |1    16| VCC
QC |2    15| QA
QD |3    14| SER data
QE |4    13| OE
QF |5    12| RCLK latch
QG |6    11| SRCLK clock
QH |7    10| SRCLR
 G |8    9 | QH*

2
QB |1    16| VCC
QC |2    15| QA
QD |3    14| SER
QE |4    13| OE
QF |5    12| RCLK
QG |6    11| SRCLK
QH |7    10| SRCLR
 G |8    9 | QH*

SRCLR - 10 to VCC - 16
1QH* - 9 (on first) to 2SER - 14 (on second)
common 1SRCLK-2SRCLK, 1RCLK-2RCLK between the two
OE - G
It uses four pins from the MCU to provide 16 output pins */
/* Shift Register Clock/Latch configuration (MCU to ShiftRegister.RCLK - 12) */
/* Shift Register SPI Data Out configuration (MCU to ShiftRegister.SER - 14) */
/* Shift Register SPI Serial Clock configuration (MCU to ShiftRegister.SRCLK - 11) */
/*
shift register 74HC165N

3
nPL |1    16| VCC
 CP |2    15| nCE
 D4 |3    14| D3
 D5 |4    13| D2
 D6 |5    12| D1
 D7 |6    11| D0
nQ7 |7    10| DS
  G |8    9 | Q7

nCE - G
nPL - Latch (same as 595)
CP - Serial Clock (same as 595)
Q7 - SPI Data in - MISO
*/

#define SPI_DRIVER SPID0

#define SPI_LATCH_PIN GP1
#define SPI_SCK_PIN GP2
#define SPI_MISO_PIN GP4
#define SPI_MOSI_PIN GP3
#define SPI_MODE 3
#define SPI_lsbFirst true

#define SPI_DIVISOR 8

#define DEBUG_MATRIX_SCAN_RATE


//0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000
// 1QA     1QB     1QC     1QD     1QE     1QF     1QG     1QH     2QA     2QB     2QC     2QD     2QE     2QF     2QG     2QH

//                  C0      C1      C2       C3      C4      C5     C6       C7      C8      C9     C10     C11     C12     C13
//                 2QB     2QC     2QD      2QE     2QF     2QG    2QH      2QA     1QH     1QG     1QF     1QE     1QA     1QD
#define COLS {  0x0200, 0x0400, 0x0800,  0x1000, 0x2000, 0x4000, 0x8000, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0001, 0x0008 }

#define   RGB_ENABLE_PIN GP11

/* Double tap reset button to enter bootloader */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

#define LED_LWR_PIN GP16
#define LED_RSE_PIN GP17

#ifdef OLED_ENABLE
#define I2C1_SCL_PIN        GP7
#define I2C1_SDA_PIN        GP6
#define I2C_DRIVER I2CD1
#define OLED_BRIGHTNESS 128
#define OLED_FONT_H "keyboards/mlego/m65/lib/glcdfont.c"
#define OLED_UPDATE_INTERVAL 10
#endif
