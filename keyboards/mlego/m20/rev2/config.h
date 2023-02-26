// Copyright 2023-2023 alin m elena (@alinelena)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET              // Activates the double-tap behavior
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 200U // Timeout window in ms in which the double tap can occur.
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED_MASK 0U

#ifdef RGB_MATRIX_ENABLE

#define RGB_MATRIX_LED_COUNT 20
#define RGB_MATRIX_TIMEOUT 5000

#define RGB_MATRIX_CAPSLOCK_I 11
#define RGB_MATRIX_LWR_I 19
#define RGB_MATRIX_RSE_I 16
#define RGB_DISABLE_WHEN_USB_SUSPENDED
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_KEYPRESSES
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP

#define RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS 50
#define RGB_MATRIX_TYPING_HEATMAP_SPREAD 40
#define RGB_MATRIX_TYPING_HEATMAP_SLIM
#define RGB_MATRIX_TYPING_HEATMAP_INCREASE_STEP 32

#    define RGB_MATRIX_HUE_STEP 4
#    define RGB_MATRIX_SAT_STEP 4
#    define RGB_MATRIX_VAL_STEP 4
#endif

#define LED_LWR_PIN GP25

#ifdef OLED_ENABLE
#    define I2C1_SCL_PIN GP3
#    define I2C1_SDA_PIN GP2
#    define I2C_DRIVER I2CD1
#    define OLED_BRIGHTNESS 128
#    define OLED_FONT_H "keyboards/mlego/m65/lib/glcdfont.c"
#endif
