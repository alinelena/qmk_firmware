// Copyright 2020-2023 alin m elena (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#define MATRIX_ROWS 5
#define MATRIX_COLS 14

/*
ShiftRegister SN74HC595N

1
c13 QB |1    16| VCC
 c0 QC |2    15| QA
 c8 QD |3    14| SER mosi - GP23
 c9 QE |4    13| OE - gnd
c10 QF |5    12| RCLK cs - GP21
c11 QG |6    11| SRCLK clock -  GP22
c12 QH |7    10| SRCLR - 3v3
     G |8    9 | QH* --------------------|

2
c1 QB |1    16| VCC
c2 QC |2    15| QA
c3 QD |3    14| SER   <------------------|
c4 QE |4    13| OE   - gnd
c5 QF |5    12| RCLK - GP21
c6 QG |6    11| SRCLK - GP22
c7 QH |7    10| SRCLR - 3v3
    G |8    9 | QH*




*/

#define ROWS {GP1, GP26, GP29, GP28, GP27}

#define SPI_DRIVER SPID0

#define SPI_MATRIX_CS_PIN GP21
#define SPI_SCK_PIN GP22
#define SPI_MISO_PIN GP20
#define SPI_MOSI_PIN GP23
#define SPI_MODE 3

//rp2040 supports nothing else
#define SPI_lsbFirst false

#define SPI_DIVISOR 32



//0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000
// 1QA     1QB     1QC     1QD     1QE     1QF     1QG     1QH     2QA     2QB     2QC     2QD     2QE     2QF     2QG     2QH

//                C0      C1      C1      C3      C4      C5      C6      C7      C8     C9      C10     C11     C12     C13
//               2QH     2QG     2QF     2QE     2QD     2QC     2QB     2QA     1QH    1QG      1QF     1QE     1QA     1QB
#define COLS {0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0001, 0x0002}

/* Double tap reset button to enter bootloader */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

#if defined(CONSOLE_ENABLE)
#define DEBUG_MATRIX_SCAN_RATE
#endif

#if defined(OLED_ENABLE)
#define I2C1_SCL_PIN        GP3
#define I2C1_SDA_PIN        GP2
#define I2C_DRIVER I2CD1
#define OLED_BRIGHTNESS 128
#define OLED_FONT_H "keyboards/mlego/m65/lib/glcdfont.c"
#define OLED_UPDATE_INTERVAL 10
#endif


#if defined(QUANTUM_PAINTER_ENABLE)

#define SPI_DISP_CS_PIN GP6
#define SPI_DISP_DC_PIN GP7
#define SPI_DISP_RST_PIN GP8
#define SPI_DISP_BL_PIN GP9

#endif

#if defined(RGB_MATRIX_ENABLE)

#define RGB_ENABLE_PIN GP12
#define RGB_CURREN_FAULT_PIN GP13

#endif
