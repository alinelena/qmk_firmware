/* Copyright 2021-2022 alin m elena <alinm.elena@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define PRODUCT_ID 0x0001
#define DEVICE_VER 0x6461

#define MATRIX_ROW_PINS \
    { B15, B14, B13, B13, A8, A10, A15, B3}
#define MATRIX_COL_PINS \
    { B10, B1, B0, A3, A2, A1, A0, C15, C14, C13, B9, B8, B7, B6, B5, B4 }

/*
ShiftRegister SN74HC595N

QB |1    16| VCC
QC |2    15| QA
QD |3    14| SER
QE |4    13| OE
QF |5    12| RCLK
QG |6    11| SRCLK
QH |7    10| SRCLR
 G |8    9 | QH*

SRCLR - 10 to VCC - 16
It uses four pins from the MCU to provide 8 output pins */
/* Shift Register Clock/Latch configuration (MCU to ShiftRegister.RCLK - 12) */
#define SR_LATCH_PIN         A6

/* Shift Register Output Enable configuration (MCU to ShiftRegister.OE - 13) */
#define SR_OE_PIN            A7

/* Shift Register SPI Data Out configuration (MCU to ShiftRegister.SER - 14) */
#define SR_DATA_PIN          A4

/* Shift Register SPI Serial Clock configuration (MCU to ShiftRegister.SRCLK - 11) */
#define SR_CLOCK_PIN         A5


#define UNUSED_PINS

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION COL2ROW
