// Copyright 2020-2023 alin m elena (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
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

#define SPI_DRIVER SPID0
#define SPI_MODE 3
// rp2040 supports nothing else
#define SPI_lsbFirst false

#define SPI_MATRIX_CS_PIN GP21
#define SPI_SCK_PIN GP22
#define SPI_MISO_PIN GP20
#define SPI_MOSI_PIN GP23

#define SPI_DIVISOR 8

// 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000
//  1QA     1QB     1QC     1QD     1QE     1QF     1QG     1QH     2QA     2QB     2QC     2QD     2QE     2QF     2QG     2QH

//                C0      C1      C1      C3      C4      C5      C6      C7      C8     C9      C10     C11     C12     C13
//               2QH     2QG     2QF     2QE     2QD     2QC     2QB     2QA     1QH    1QG      1QF     1QE     1QA     1QB
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
*/

#define COLS \
    { 0x0004, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0002 }

/* Double tap reset button to enter bootloader */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

#if defined(CONSOLE_ENABLE)
#    define DEBUG_MATRIX_SCAN_RATE
#endif

#if defined(OLED_ENABLE)
#    define I2C1_SCL_PIN GP3
#    define I2C1_SDA_PIN GP2
#    define I2C_DRIVER I2CD1
#    define OLED_BRIGHTNESS 128
#    define OLED_FONT_H "keyboards/mlego/m65/lib/glcdfont.c"
#    define OLED_UPDATE_INTERVAL 10
#endif

#if defined(BACKLIGHT_ENABLE)
#    define BACKLIGHT_PWM_CHANNEL 1
#endif

#if defined(QUANTUM_PAINTER_ENABLE)

#    define SPI_DISP_CS_PIN GP6
#    if defined(QUANTUM_PAINTER_ST7735_ENABLE)
#    define DISP_ROT QP_ROTATION_90
#        define ST7735_NUM_DEVICES 1
#        define SPI_DISP_DC_PIN GP7
#        define SPI_DISP_RST_PIN GP8
#        define DISP_WIDTH 160
#        define DISP_HEIGHT 80
#        define ST7735_NO_AUTOMATIC_VIEWPORT_OFFSETS
#    endif

#    if defined(QUANTUM_PAINTER_LS0XX_ENABLE)
/*
#        define SPI_DISP_BL_PIN GP9
// version ls013b7dh05
#    define DISP_ROT QP_ROTATION_90
#        define DISP_WIDTH 144
#        define DISP_HEIGHT 168
// version ls013b7dh03
#    define DISP_ROT QP_ROTATION_270
#    define DISP_WIDTH 128
#    define DISP_HEIGHT 128
*/
// version LS011B7DH03
#    define DISP_ROT QP_ROTATION_180
#    define DISP_WIDTH  160
#    define DISP_HEIGHT 68

#    endif

#    if defined(CONSOLE_ENABLE)
#        define INIT_DELAY 3000
#        define QUANTUM_PAINTER_DEBUG
#    endif

#endif

#define QUANTUM_PAINTER_NUM_IMAGES 16

#if defined(RGB_MATRIX_ENABLE)
#    define RGB_MATRIX_LED_COUNT 73
#    define RGB_ENABLE_PIN GP12
#    define RGB_CURREN_FAULT_PIN GP13

#    define RGB_MATRIX_TIMEOUT 15000

#    define RGB_MATRIX_CAPSLOCK_I 26
#    define RGB_MATRIX_LWR_I 54
#    define RGB_MATRIX_RSE_I 56

#    define RGB_DISABLE_WHEN_USB_SUSPENDED
#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#    define RGB_MATRIX_KEYPRESSES
#    define ENABLE_RGB_MATRIX_TYPING_HEATMAP

#    define RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS 50
#    define RGB_MATRIX_TYPING_HEATMAP_SPREAD 40
#    define RGB_MATRIX_TYPING_HEATMAP_SLIM
#    define RGB_MATRIX_TYPING_HEATMAP_INCREASE_STEP 32

#    define RGB_MATRIX_HUE_STEP 4
#    define RGB_MATRIX_SAT_STEP 4
#    define RGB_MATRIX_VAL_STEP 4
#endif
