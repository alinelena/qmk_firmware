// Copyright 2020-2025 alin m elena (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once


#define ESC_PIN B0
#define ESC_ROW 1
#define ESC_COL 0


#define ENCODER_BUTTON_PIN C15
#define ENCODER_ROW 0
#define ENCODER_COL 0

#define EECONFIG_KB_DATA_SIZE 6

#define MUXES 2
// adc1-in3 A2
// adc2-in3 A6
#define MUX_PINS { A2, A6 }

#define MUX_SELECTOR_BITS 4
#define MUX_SELECTOR_PINS { B11, B10, B1, B2 }

#define MUX_CHANNELS ( 1 << MUX_SELECTOR_BITS )

#define ADC_RESOLUTION      ADC_CFGR_RES_12BITS
#define ADC_SAMPLING_TIME  ADC_SMPR_SMP_2P5
#define ADC_RESOLUTION_MAX  1 << 12

#define CALIBRATION_RANGE 255

#if defined(CONSOLE_ENABLE)
#    define DEBUG_MATRIX_SCAN_RATE
#endif

#if defined(QUANTUM_PAINTER_ENABLE)

#define SPI_DRIVER SPID1
#define SPI_MODE 3
#define SPI_lsbFirst false

#define SPI_SCK_PIN B3
#define SPI_MISO_PIN B4
#define SPI_MOSI_PIN B5

#define SPI_DIVISOR 16

#    define SPI_DISP_CS_PIN A10
#    if defined(QUANTUM_PAINTER_ST7735_ENABLE)
#    define DISP_ROT QP_ROTATION_90
#        define ST7735_NUM_DEVICES 1
#        define SPI_DISP_DC_PIN C6
#        define SPI_DISP_RST_PIN A15
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
// version LS011B7DH03
#    define DISP_ROT QP_ROTATION_180
#    define DISP_WIDTH  160
#    define DISP_HEIGHT 68
*/
#    endif

#    if defined(CONSOLE_ENABLE)
#        define INIT_DELAY 3000
#        define QUANTUM_PAINTER_DEBUG
#    endif

#define QUANTUM_PAINTER_NUM_IMAGES 16
#endif


#if defined(RGB_MATRIX_ENABLE)

#   define WS2812_PWM_DRIVER PWMD3
#   define WS2812_PWM_CHANNEL 4
#   define WS2812_PWM_PAL_MODE 10
#   define WS2812_DMA_STREAM STM32_DMA1_STREAM2
#   define WS2812_DMA_CHANNEL 2
#   define WS2812_DMAMUX_ID STM32_DMAMUX1_TIM3_UP

#    define RGB_ENABLE_PIN B6
#    define RGB_CURREN_FAULT_PIN B9

#    define RGB_MATRIX_TIMEOUT 15000

#    define RGB_MATRIX_CAPSLOCK_I 8
#    define RGB_MATRIX_LWR_I 17
#    define RGB_MATRIX_RSE_I 19

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
