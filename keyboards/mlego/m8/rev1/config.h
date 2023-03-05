/*
Copyright 2021-2023 Alin M Elena <alinm.elena@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#define MATRIX_ROWS 4
#define MATRIX_COLS 2

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


#define SPI_DRIVER SPID0
#define SPI_CS_PIN GP1
#define SPI_SCK_PIN GP2
#define SPI_MISO_PIN GP4
#define SPI_MOSI_PIN GP3
#define SPI_LSBFIRST true
#define SPI_DIVISOR 8
#define SPI_MODE 3
#define DEBUG_MATRIX_SCAN_RATE

// 00000001
// 00000010
// 00000100
// 00001000
// 00010000
// 00100000
// 01000000
// 10000000
//0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000
// 1QA     1QB     1QC     1QD     1QE     1QF     1QG     1QH     2QA     2QB     2QC     2QD     2QE     2QF     2QG     2QH

//              C0      C1
//              1QC     1QB
#define COLS { 0x0004, 0x0002}
#define ROWS {3,2,1,0}

#define   RGB_ENABLE_PIN GP11
#define POWER_LED_PIN GP25

#define INIT_DELAY 3000
#define QUANTUM_PAINTER_DEBUG

#define EINK_RST_PIN GP6
#define EINK_CS_PIN GP0
#define EINK_DC_PIN GP29
#define EINK_BUSY_PIN GP28
#define EINK_BWR

/* Double tap reset button to enter bootloader */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U
