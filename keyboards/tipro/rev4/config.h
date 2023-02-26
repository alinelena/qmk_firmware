// Copyright 2020-2023 alin m elena (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#define MATRIX_ROWS 8
#define MATRIX_COLS 16

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
It uses four pins from the MCU to provide 16 output pins
Shift Register Clock/Latch configuration (MCU to ShiftRegister.RCLK - 12)
Shift Register SPI Data Out configuration (MCU to ShiftRegister.SER - 14)
Shift Register SPI Serial Clock configuration (MCU to ShiftRegister.SRCLK - 11)

shift register 74HC589ag (this is much more powerful than this... see the usage in ghoul)
https://github.com/tzarc/ghoul

for a version with a simpler 74HC165 check mlego/m65/rev9
https://gitlab.com/m-lego/m65/-/tags/rev9

3
  B |1    16| VCC
  C |2    15| A
  D |3    14| SA
  E |4    13| SS/PL
  F |5    12| CS - latch
  G |6    11| SCK - clock
  H |7    10| OE
GND |8    9 | QH

SA - PL - check diagram
CS - Latch (same as 595)
SCK - Serial Clock (same as 595)
QH - SPI Data in - MISO
*/

#define SPI_DRIVER SPID0

#define SPI_MATRIX_CS_PIN GP1
#define SPI_SCK_PIN GP2
#define SPI_MISO_PIN GP4
#define SPI_MOSI_PIN GP3
#define SPI_MODE 3

//rp2040 supports nothing else
#define SPI_lsbFirst false

#define SPI_DIVISOR 32

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
/*
      SR2     SR1
LSB                  <-- LSBFIRST
    --> MSBFIRST                MSB
1Q0 00000000 00000001 == 0x0001 1Q7
1Q1 00000000 00000010 == 0x0002 1Q6
1Q2 00000000 00000100 == 0x0004 1Q5
1Q3 00000000 00001000 == 0x0008 1Q4
1Q4 00000000 00010000 == 0x0010 1Q3
1Q5 00000000 00100000 == 0x0020 1Q2
1Q6 00000000 01000000 == 0x0040 1Q1
1Q7 00000000 10000000 == 0x0080 1Q0
2Q0 00000001 00000000 == 0x0100 2Q7
2Q1 00000010 00000000 == 0x0200 2Q6
2Q2 00000100 00000000 == 0x0400 2Q5
2Q3 00001000 00000000 == 0x0800 2Q4
2Q4 00010000 00000000 == 0x1000 2Q3
2Q5 00100000 00000000 == 0x2000 2Q2
2Q6 01000000 00000000 == 0x4000 2Q1
2Q7 10000000 00000000 == 0x8000 2Q0

rp2040 will do MSB regardless of what you pass

 */
// due to a mistake in circuit... c0 needs to be the last one
//                 C0      C1     C2     C3     C4     C5     C6     C7     C8     C9    C10    C11    C12    C13    C14    C15
//                1QC     2QB    2QC    2QD    2QE    2QF    2QG    2QH    2QA    1QD    1QE    1QF    1QG    1QB    1QH    1QA
#define COLS {  0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000,0x0100,0x0008,0x0010,0x0020,0x0040,0x0002,0x0004, 0x0080,0x0001}
#define ROWS {0, 1, 2, 3, 4, 5, 6, 7}

/* Double tap reset button to enter bootloader */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

#define LED_LWR_PIN GP16
#define LED_RSE_PIN GP17
#define LED_CAPS_PIN GP25

#if defined(CONSOLE_ENABLE)
#define DEBUG_MATRIX_SCAN_RATE
#endif
