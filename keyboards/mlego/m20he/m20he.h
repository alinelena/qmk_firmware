// Copyright 2020-2025 alin m elena (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

enum layer_names { _QW = 0, _LWR, _RSE, _ADJ };

const char *get_layer_name(const uint8_t);

enum custom_codes { RGB_HEAT = SAFE_RANGE, RGB_OFF_EFF, RGB_ON_EFF, RGB_IND, RGB_LSD, RGB_UON, RGB_UOF };

#if defined(QUANTUM_PAINTER_ENABLE)
void user_display_magic(void);
void render_oled_logo(void);
void clear_oled_screen(void);
void init_oled_timer(void);
void show_icons_lcd(void);
#endif

#if defined(QUANTUM_PAINTER_ENABLE)
void init_lcd_timer(void);
void render_lcd_logo(void);
void clear_lcd_screen(void);
void init_lcd_timer(void);
void draw_lcd_icon(const int, const int, const bool, const int, const int, const int, const int, const int, const int, const int);
void init_lcd_test(void);
#endif

#if defined(RGBLIGHT_ENABLE)
void                             set_rgb_layers(layer_state_t);
const rgblight_segment_t *const *my_rgb(void);
void                             set_default_rgb_layers(layer_state_t);
#endif

bool lcd_init(void);
void toggle_leds(const bool, const bool);
void set_led_toggle(const uint8_t, const bool);

#ifdef RGB_MATRIX_ENABLE
static inline RGB hsv2rgb(const uint16_t h, const uint8_t s, const uint8_t v) {
    uint8_t val = rgb_matrix_get_val();
    return hsv_to_rgb((HSV){h, s, (v > val) ? val : v});
}
#endif

static inline void toggle_rgb(const uint8_t index, const bool on, const uint16_t h, const uint8_t s, const uint8_t v) {
#ifdef RGB_MATRIX_ENABLE
    if (on) {
        RGB rgb = hsv2rgb(h, s, v);
        rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
    } else {
        rgb_matrix_set_color(index, RGB_OFF);
    }
#endif
}
