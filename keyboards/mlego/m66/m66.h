// Copyright 2020-2023 alin m elena (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

#if defined(UNICODEMAP_ENABLE)
enum unicode_names { la = 0, lA, lb, lB, lc, lC, ld, lD, le, lE, lf, lF, lg, lG, lh, lH, li, lI, lj, lJ, lk, lK, ll, lL, lm, lM, ln, lN, lo, lO, lp, lP, lq, lQ, lr, lR, ls, lS, lt, lT, lu, lU, lv, lV, lw, lW, lx, lX, ly, lY, lz, lZ, lc1, lC1, lp1, lP1, lq1, lQ1, ll1, lL1, lk1, lK1, lbo1, lbo2, lbc1, lbc2, ls1, ls2, ra, rA, ra1, rA1, rc, rC, ri, rI, rs, rS, rt, rT, ro, rO, ra2, rA2, ra3, rA3, rx, rX, rv, rV, rb, rB, rn, rN };
#endif

enum layer_names { _QW = 0, _LWR, _RSE, _ADJ };

const char *get_layer_name(const uint8_t);
const char *get_unicde_name(const uint8_t);

enum custom_codes { RGB_HEAT = SAFE_RANGE, RGB_OFF_EFF, RGB_ON_EFF, RGB_IND, RGB_LSD };

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

static inline void init_lwr_rse_led(void) {
#if defined(LED_LWR_PIN)
    setPinOutput(LED_LWR_PIN);
    writePin(LED_LWR_PIN, false);
    wait_ms(30);
#endif

#if defined(LED_RSE_PIN)
    setPinOutput(LED_RSE_PIN);
    writePin(LED_RSE_PIN, false);
    wait_ms(30);
#endif
}

static inline void led_lwr(const bool on) {
#if defined(LED_LWR_PIN)
    if ((PRODUCT_ID == 0x6064) || (PRODUCT_ID == 0x6065) || (PRODUCT_ID == 0x6066) || (PRODUCT_ID == 0x6067)) {
        writePin(LED_LWR_PIN, !on);
    } else {
        writePin(LED_LWR_PIN, on);
    }
#endif
}

static inline void led_rse(const bool on) {
#if defined(LED_RSE_PIN)
    if ((PRODUCT_ID == 0x6064) || (PRODUCT_ID == 0x6065) || (PRODUCT_ID == 0x6066) || (PRODUCT_ID == 0x6067)) {
        writePin(LED_RSE_PIN, !on);
    } else {
        writePin(LED_RSE_PIN, on);
    }
#endif
}
