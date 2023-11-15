// Copyright 2020-2023 alin m elena (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#if defined(CONSOLE_ENABLE)
#    include "print.h"
#endif
#if defined(OLED_ENABLE) || defined(QUANTUM_PAINTER_ENABLE)
#    include "version.h"
#endif

#if defined(QUANTUM_PAINTER_ENABLE)
#    include "color.h"
#    include "qp.h"
#    include "qp_comms.h"
#    include "graphics.h"
#    if defined(QUANTUM_PAINTER_LS0XX_ENABLE)
#        include "qp_surface.h"
#    endif
#endif

#if defined(RGB_MATRIX_ENABLE)
static bool    toggle_caps       = false;
static uint8_t current_rgb_layer = 42;
#    if defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP)
static bool toggle_heat = false;
#    endif
#endif

#if defined(QUANTUM_PAINTER_ENABLE)
static painter_device_t lcd;
static uint32_t         lcd_logo_timer  = 0;
static bool             clear_lcd_logo  = true;
static bool             update_lcd_logo = true;
static uint8_t          current_layer   = 42;

static led_t last_led_state = {42};
#    if defined(RGB_MATRIX_ENABLE)
static bool current_rgb = false;
#    endif

#    if defined(UNICODE_COMMON_ENABLE)
static uint8_t current_unicode = 42;
#    endif

#    if defined(QUANTUM_PAINTER_LS0XX_ENABLE)
static int8_t buf[SURFACE_REQUIRED_BUFFER_BYTE_SIZE(DISP_WIDTH, DISP_HEIGHT, 1)] = {0};
#    endif

#endif

// let us assume we start with both layers off
static bool toggle_lwr = false;
static bool toggle_rse = false;

#if defined(RGBLIGHT_ENABLE)

const rgblight_segment_t PROGMEM my_qwerty_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_OFF});
const rgblight_segment_t PROGMEM my_lwr_layer[]    = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_GREEN});
const rgblight_segment_t PROGMEM my_rse_layer[]    = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_RED});
const rgblight_segment_t PROGMEM my_adj_layer[]    = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_CORAL});

const rgblight_segment_t *const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(my_qwerty_layer, my_lwr_layer, my_rse_layer, my_adj_layer);

#endif

#if defined(OLED_ENABLE)
static uint32_t           oled_logo_timer = 0;
static bool               clear_logo      = true;
static const char PROGMEM m65_logo[]      = {0x92, 0x92, 0x93, 0x94, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x92, 0x92, 0x93, 0x94, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB2, 0x92, 0xB3, 0xB4, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0};
#endif
#if defined(OLED_ENABLE) || defined(QUANTUM_PAINTER_ENABLE)
static const char bdate[] = QMK_BUILDDATE;
static const char hash[]  = QMK_GIT_HASH;
#endif

#if defined(RGBLIGHT_ENABLE)

void set_rgb_layers(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _QW));
    rgblight_set_layer_state(1, layer_state_cmp(state, _LWR));
    rgblight_set_layer_state(2, layer_state_cmp(state, _RSE));
    rgblight_set_layer_state(3, layer_state_cmp(state, _ADJ));
}

void set_default_rgb_layers(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _QW));
}

const rgblight_segment_t *const *my_rgb(void) {
    return my_rgb_layers;
}

#endif

void set_led_toggle(const uint8_t layer, const bool state) {
    switch (layer) {
        case _LWR:
            toggle_lwr = state;
            break;
        case _RSE:
            toggle_rse = state;
            break;
        default:
            break;
    }
}

void toggle_leds(const bool toggle_lwr, const bool toggle_rse) {
    led_lwr(toggle_lwr);
    led_rse(toggle_rse);
    if (layer_state_is(_ADJ)) {
        led_lwr(true);
        led_rse(true);
    }
}

const char *get_layer_name(const uint8_t layer) {
    switch (layer) {
        case _QW:
            return "qwerty";
        case _LWR:
            return "lower ";
        case _RSE:
            return "raise ";
        case _ADJ:
            return "adjust";
        default:
            return "none  ";
    }
}

const char *get_unicode_name(const uint8_t uni) {
#if defined(UNICODE_COMMON_ENABLE)
    switch (uni) {
        case UNICODE_MODE_LINUX:
            return "linux";
        case UNICODE_MODE_MACOS:
            return "apple";
        case UNICODE_MODE_WINDOWS:
            return "win  ";
        case UNICODE_MODE_WINCOMPOSE:
            return "win c";
        case UNICODE_MODE_BSD:
            return "bsd  ";
        case UNICODE_MODE_EMACS:
            return "emacs";
        default:
            return "none ";
    }
#else
    return "none ";
#endif
}

#if defined(OLED_ENABLE)

void init_oled_timer(void) {
    oled_logo_timer = timer_read32();
};

void user_oled_magic(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);
    oled_write_P(PSTR(get_layer_name(get_highest_layer(layer_state))), false);
    oled_write_P(PSTR("\n"), false);

#    if defined(UNICODE_COMMON_ENABLE)
    oled_write_P(PSTR("uni: "), false);
    oled_write_ln_P(PSTR(get_unicode_name(get_unicode_input_mode())), false);
    // oled_write_P(PSTR("\n"), false);
    //  Host Keyboard LED Status
#    endif
    led_t led_state = host_keyboard_led_state();
    oled_write_P("Lock:", false);
    oled_write_P(led_state.num_lock ? PSTR("Num") : PSTR("   "), false);
    oled_write_P(led_state.scroll_lock ? PSTR(" Scroll") : PSTR("       "), false);
    oled_write_ln_P(led_state.caps_lock ? PSTR(" Caps") : PSTR("     "), false);

#    if defined(WPM_ENABLE)
    oled_write_P(PSTR("wpm: "), false);
    uint8_t wpm = get_current_wpm();
    oled_write_P(wpm != 0 ? get_u8_str(wpm, ' ') : PSTR("   "), false);
#    endif
    oled_write_P(PSTR(" "), false);
    oled_write_ln_P(PSTR(hash), false);
}

void render_oled_logo(void) {
    oled_write_P(m65_logo, false);
    oled_write_ln_P(PSTR(bdate), false);
}

void clear_oled_screen(void) {
    if (clear_logo) {
        for (uint8_t i = 0; i < OLED_DISPLAY_HEIGHT; ++i) {
            for (uint8_t j = 0; j < OLED_DISPLAY_WIDTH; ++j) {
                oled_write_raw_byte(0x0, i * OLED_DISPLAY_WIDTH + j);
            }
        }
        clear_logo = false;
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

#    define SHOW_LOGO 5000
bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    if ((timer_elapsed32(oled_logo_timer) < SHOW_LOGO)) {
        render_oled_logo();
    } else {
        clear_oled_screen();
        user_oled_magic();
    }
    return false;
}

#endif

#if defined(QUANTUM_PAINTER_ENABLE)

void user_lcd_magic(void) {
    char buf[90] = {0};
    int  h       = qp_fonts[0]->line_height;

    uint8_t layer = get_highest_layer(layer_state);
    if (layer != current_layer) {
        const char *layer_name = get_layer_name(layer);
        snprintf(buf, sizeof(buf), "layer: %s", layer_name);
        qp_drawtext_recolor(lcd, 0, 0.5 * h, qp_fonts[0], buf, HSV_ORANGE, HSV_BLACK);
        if (DISP_WIDTH < 130) {
            draw_lcd_icon(20, 72, true, 5 + layer, HSV_BLACK, HSV_ORANGE);

        } else {
            draw_lcd_icon(110, 5, true, 5 + layer, HSV_BLACK, HSV_ORANGE);
        }
        current_layer = layer;
    }

#    if (UNICODE_COMMON_ENABLE)
    uint8_t uni = get_unicode_input_mode();
    if (uni != current_unicode) {
        const char *uni_name = get_unicode_name(uni);
        snprintf(buf, sizeof(buf), "uni: %s", uni_name);
        qp_drawtext_recolor(lcd, 0, 1.5 * h, qp_fonts[0], buf, HSV_PINK, HSV_BLACK);
        current_unicode = uni;
    }
#    endif

    h = 36;
    // this is lifted from djinn
    if (last_led_state.raw != host_keyboard_led_state().raw) {
        show_icons_lcd();
    }
#    if defined(RGB_MATRIX_ENABLE)
    bool rgb = rgb_matrix_is_enabled();
    if (current_rgb != rgb) {
        if (DISP_WIDTH < 130) {
            draw_lcd_icon(60, 2 * h, rgb, 13, HSV_CHARTREUSE, HSV_BLACK);
        } else {
            draw_lcd_icon(110, h, rgb, 13, HSV_CHARTREUSE, HSV_BLACK);
        }
        current_rgb = rgb;
    }
#    endif
}

void draw_lcd_icon(const int x, const int y, const bool on, const int i, const int bh, const int bs, const int bv, const int fh, const int fs, const int fv) {
    if (on) {
        qp_drawimage_recolor(lcd, x, y, qp_images[i], bh, bs, bv, fh, fs, fv);
    } else {
        qp_drawimage_recolor(lcd, x, y, qp_images[i], fh, fs, fv, bh, bs, bv);
    }
}
void init_lcd_timer(void) {
    lcd_logo_timer = timer_read32();
};

void show_icons_lcd(void) {
    const int h = 36;

    last_led_state.raw = host_keyboard_led_state().raw;
    draw_lcd_icon(4, h, last_led_state.caps_lock, 2, HSV_PINK, HSV_BLACK);
    draw_lcd_icon(40, h, last_led_state.scroll_lock, 4, HSV_YELLOW, HSV_BLACK);
    draw_lcd_icon(76, h, last_led_state.num_lock, 3, HSV_CYAN, HSV_BLACK);
}

void render_lcd_logo(void) {
    if (update_lcd_logo) {
        if (DISP_HEIGHT > 81) {
            qp_drawimage_recolor(lcd, 0, 72, qp_images[1], HSV_BLACK, HSV_PINK);
            qp_drawimage_recolor(lcd, 0, 20, qp_images[0], HSV_CYAN, HSV_BLACK);
        } else {
            qp_drawimage_recolor(lcd, 35, 20, qp_images[1], HSV_BLACK, HSV_PINK);
            qp_drawimage_recolor(lcd, 0, 20, qp_images[0], HSV_CYAN, HSV_BLACK);
        }
        int     h          = qp_fonts[0]->line_height;
        int16_t hash_width = qp_textwidth(qp_fonts[0], hash);
        qp_drawtext_recolor(lcd, DISP_WIDTH - hash_width - 5, DISP_HEIGHT - 1.25 * h, qp_fonts[0], hash, HSV_ORANGE, HSV_BLACK);
        int16_t build_width = qp_textwidth(qp_fonts[0], bdate);
        qp_drawtext_recolor(lcd, DISP_WIDTH - build_width - 10, 10, qp_fonts[0], bdate, HSV_ORANGE, HSV_BLACK);
        update_lcd_logo = false;
    }
}

void clear_lcd_display(void) {
    if (clear_lcd_logo) {
        qp_rect(lcd, 0, 0, DISP_WIDTH, DISP_HEIGHT, HSV_BLACK, true);
        show_icons_lcd();
#    if defined(RGB_MATRIX_ENABLE)
        if (DISP_WIDTH < 130) {
            draw_lcd_icon(60, 72, current_rgb, 13, HSV_CHARTREUSE, HSV_BLACK);
        } else {
            draw_lcd_icon(110, 36, current_rgb, 13, HSV_CHARTREUSE, HSV_BLACK);
        }
#    endif
        clear_lcd_logo = false;
    }
}

#    define SHOW_LCD_LOGO 5000
void housekeeping_task_kb(void) {
    if ((timer_elapsed32(lcd_logo_timer) < SHOW_LCD_LOGO)) {
        render_lcd_logo();
    } else {
        clear_lcd_display();
        user_lcd_magic();
    }
}
#endif

void matrix_scan_kb(void) {
    matrix_scan_user();
    toggle_leds(toggle_lwr, toggle_rse);
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
#if defined(CONSOLE_ENABLE)
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

    switch (keycode) {
        case (TT(_LWR)):
            if (!record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
                // This runs before the TT() handler toggles the layer state, so the current layer state is the opposite of the final one after toggle.
                set_led_toggle(_LWR, !layer_state_is(_LWR));
            }
            return true;
        case (TT(_RSE)):
            if (record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
                set_led_toggle(_RSE, !layer_state_is(_RSE));
            }
            return true;
#if defined(RGB_MATRIX_ENABLE) && !defined(RGB_MATRIX_DISABLE_KEYCODES)
#    if defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP)
        case (RGB_HEAT):
            if (record->event.pressed) {
                toggle_heat = !toggle_heat;
                if (toggle_heat) {
                    rgblight_enable_noeeprom();
                    rgblight_mode_noeeprom(RGB_MATRIX_TYPING_HEATMAP);
                } else {
                    rgblight_disable_noeeprom();
                }
            }
            return true;
#    endif
        case (RGB_HUI):
            if (record->event.pressed) {
                rgb_matrix_increase_hue_noeeprom();
            }
            return false;
        case (RGB_HUD):
            if (record->event.pressed) {
                rgb_matrix_decrease_hue_noeeprom();
            }
            return false;
        case (RGB_VAI):
            if (record->event.pressed) {
                rgb_matrix_increase_val_noeeprom();
            }
            return false;
        case (RGB_VAD):
            if (record->event.pressed) {
                rgb_matrix_decrease_val_noeeprom();
            }
            return false;
        case (RGB_SAI):
            if (record->event.pressed) {
                rgb_matrix_increase_sat_noeeprom();
            }
            return false;
        case (RGB_SAD):
            if (record->event.pressed) {
                rgb_matrix_decrease_sat_noeeprom();
            }
            return false;
        case (RGB_TOG):
            if (record->event.pressed) {
                rgb_matrix_toggle_noeeprom();
            }
            return false;
        case (RGB_MOD):
            if (record->event.pressed) {
                rgb_matrix_step_noeeprom();
            }
            return false;
        case (RGB_RMOD):
            if (record->event.pressed) {
                rgb_matrix_step_reverse_noeeprom();
            }
            return false;
        case (RGB_IND):
            if (record->event.pressed) {
                rgblight_enable_noeeprom();
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
                rgb_matrix_set_flags_noeeprom(LED_FLAG_NONE);
                int index;
                for (index = 0; index < RGB_MATRIX_LED_COUNT; ++index) {
                    rgb_matrix_set_color(index, 0, 0, 0);
                }
                rgb_matrix_check_finished_leds(RGB_MATRIX_LED_COUNT);
    rgb_matrix_decrease_val_noeeprom();
    rgb_matrix_decrease_val_noeeprom();
    rgb_matrix_decrease_val_noeeprom();
    rgb_matrix_decrease_val_noeeprom();
            }
            return false;
        case (RGB_LSD):
            if (record->event.pressed) {
                rgblight_enable_noeeprom();
                rgb_matrix_set_flags_noeeprom(LED_FLAG_UNDERGLOW | LED_FLAG_KEYLIGHT | LED_FLAG_INDICATOR);
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
                rgb_matrix_sethsv_noeeprom(HSV_YELLOW);
            }
            return false;
        case (RGB_M_SN):
            if (record->event.pressed) {
                rgblight_enable_noeeprom();
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
                rgb_matrix_sethsv_noeeprom(HSV_GREEN);
            }
            return false;
        case (RGB_OFF_EFF):
            if (record->event.pressed) {
                rgblight_disable_noeeprom();
            }
            return false;
        case (RGB_ON_EFF):
            if (record->event.pressed) {
                rgblight_enable_noeeprom();
            }
            return false;
#endif

        default:
            return true;
    }
    return process_record_user(keycode, record);
}

layer_state_t layer_state_set_kb(layer_state_t state) {
    state = layer_state_set_user(state);

#if defined(RGBLIGHT_ENABLE)

    set_rgb_layers(state);

#endif

    return update_tri_layer_state(state, _LWR, _RSE, _ADJ);
}

#if defined(RGBLIGHT_ENABLE)

layer_state_t default_layer_state_set_kb(layer_state_t state) {
    state = default_layer_state_set_user(state);

    set_default_rgb_layers(state);

    return state;
}

#endif

#if defined(RGB_MATRIX_ENABLE)
// bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {

bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_indicators_user()) {
        return false;
    }

    bool c_caps = host_keyboard_led_state().caps_lock;
    if (c_caps || toggle_caps) {
        toggle_rgb(RGB_MATRIX_CAPSLOCK_I, c_caps, HSV_BLUE);

        toggle_caps = c_caps;
    }
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);

    if (layer != current_rgb_layer || (toggle_rse || toggle_lwr)) {
        switch (layer) {
            case _RSE:
                if (toggle_rse) {
                    toggle_rgb(RGB_MATRIX_LWR_I, false, HSV_PINK);
                    toggle_rgb(RGB_MATRIX_RSE_I, toggle_rse, HSV_GREEN);
                }
                break;
            case _LWR:
                if (toggle_lwr) {
                    toggle_rgb(RGB_MATRIX_LWR_I, toggle_lwr, HSV_RED);
                    toggle_rgb(RGB_MATRIX_RSE_I, false, HSV_PINK);
                }
                break;
            case _ADJ:
                if (toggle_rse && toggle_lwr) {
                    toggle_rgb(RGB_MATRIX_RSE_I, true, HSV_PINK);
                    toggle_rgb(RGB_MATRIX_LWR_I, true, HSV_PINK);
                }
                break;
            default:
                toggle_rgb(RGB_MATRIX_RSE_I, false, HSV_PINK);
                toggle_rgb(RGB_MATRIX_LWR_I, false, HSV_PINK);
                break;
        }
        current_rgb_layer = layer;
    }

    return false;
}
#endif

#if defined(QUANTUM_PAINTER_ENABLE)
void init_lcd_test(void) {
    int  colours[4 * 3]    = {HSV_RED, HSV_GREEN, HSV_BLUE, HSV_YELLOW};
    char colour_name[4][8] = {"red", "green", "blue ", "yellow"};
    int  i;
    char buf[64] = {0};
    for (i = 0; i < 4; ++i) {
        qp_rect(lcd, 15 * i, 15 * i, DISP_WIDTH - 15 * i, DISP_HEIGHT - 15 * i, colours[3 * i], colours[3 * i + 1], colours[3 * i + 2], true);
        snprintf(buf, sizeof(buf), "c: %s", colour_name[i]);
        qp_drawtext_recolor(lcd, 0, 5, qp_fonts[0], buf, HSV_ORANGE, HSV_BLACK);
        wait_ms(500);
    }
    wait_ms(1000);
    qp_rect(lcd, 0, 0, DISP_WIDTH, DISP_HEIGHT, HSV_BLACK, true);
}

bool lcd_sharp_mip_init(void) {
#    if defined(QUANTUM_PAINTER_LS0XX_ENABLE)
    load_qp_resources();

#        if defined(SPI_DISP_BL_PIN)
    setPinOutput(SPI_DISP_BL_PIN);
    writePinHigh(SPI_DISP_BL_PIN);
    wait_ms(300);
#        endif
    // needs spi mode 0
    lcd = qp_ls0xx_device_t_make_spi_device(DISP_WIDTH, DISP_HEIGHT, SPI_DISP_CS_PIN, SPI_DIVISOR, 0, (void *)buf);
    if (!qp_init(lcd, DISP_ROT) || !qp_power(lcd, true)) {
        return false;
    }

    init_lcd_test();
#    endif
    return true;
}

bool lcd_st7735_init(void) {
#    if defined(QUANTUM_PAINTER_ST7735_ENABLE)
    load_qp_resources();
#        if defined(BACKLIGHT_ENABLE)
    // Turn on the LCD backlight, is it really needed?
    backlight_enable();
    backlight_level(BACKLIGHT_LEVELS);
#        endif

    lcd = qp_st7735_make_spi_device(DISP_WIDTH, DISP_HEIGHT, SPI_DISP_CS_PIN, SPI_DISP_DC_PIN, SPI_DISP_RST_PIN, SPI_DIVISOR, SPI_MODE);
    qp_set_viewport_offsets(lcd, 1, 26);

    // if (!qp_init(lcd, QP_ROTATION_0) || !qp_power(lcd, true) || !qp_lvgl_attach(lcd)){
    if (!qp_init(lcd, DISP_ROT) || !qp_power(lcd, true)) {
        return false;
    };

    wait_ms(300);

    qp_comms_start(lcd);
    qp_comms_command(lcd, 0x21);
    qp_comms_stop(lcd);

    init_lcd_test();
#    endif
    return true;
}
#endif

void keyboard_post_init_kb(void) {
#if defined(CONSOLE_ENABLE)
    debug_enable   = true;
    debug_matrix   = true;
    debug_keyboard = true;
#endif

    init_lwr_rse_led();

#if defined(RGB_ENABLE_PIN) && (defined(RGB_MATRIX_ENABLE) || defined(RGBLIGHT_ENABLE))
    setPinOutput(RGB_ENABLE_PIN);
    writePinHigh(RGB_ENABLE_PIN);
    wait_ms(30);
#endif
#if defined(RGB_MATRIX_ENABLE)
    rgblight_enable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_set_flags_noeeprom(LED_FLAG_NONE);
    int index;
    for (index = 0; index < RGB_MATRIX_LED_COUNT; ++index) {
        rgb_matrix_set_color(index, 0, 0, 0);
    }
    rgb_matrix_check_finished_leds(RGB_MATRIX_LED_COUNT);
    rgb_matrix_decrease_val_noeeprom();
    rgb_matrix_decrease_val_noeeprom();
    rgb_matrix_decrease_val_noeeprom();
    rgb_matrix_decrease_val_noeeprom();
#endif

#if defined(OLED_ENABLE)
    init_oled_timer();
#endif

#if defined(QUANTUM_PAINTER_ENABLE)
    lcd_st7735_init();
    lcd_sharp_mip_init();

    init_lcd_timer();
#endif

#if defined(RGBLIGHT_ENABLE)
    // Enable the LED layers
    rgblight_layers = my_rgb();
#endif

    keyboard_post_init_user();
}
