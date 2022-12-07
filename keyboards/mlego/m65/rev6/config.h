// Copyright 2022 alin m elena (@alinelena)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET // Activates the double-tap behavior
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 200U // Timeout window in ms in which the double tap can occur.
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED_MASK 0U

#ifdef OLED_ENABLE
#    define OLED_DISPLAY_128X32
#define I2C1_SCL_PIN        GP3
#define I2C1_SDA_PIN        GP2
#define I2C_DRIVER I2CD2
#define OLED_BRIGHTNESS 128
#define OLED_FONT_H "keyboards/mlego/m65/lib/glcdfont.c"
#endif
