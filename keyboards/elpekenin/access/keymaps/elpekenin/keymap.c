// Copyright 2023 Pablo Martinez (@elpekenin) <elpekenin@elpekenin.dev>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "access.h"
#include "elpekenin.h"

#if defined(TOUCH_SCREEN_ENABLE)
#    include "touch_driver.h"
#endif // TOUCH_SCREEN_ENABLE

enum layers {
    _QWERTY,
    _FN1,
    _FN2,
    _RST
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
        // Note: KC_4 has custom logic defined on userspace
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,           KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,           KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,           KC_H,    KC_J,    KC_K,    KC_L,    TD_NTIL, KC_RETN,
        KC_LSFT, TD_Z,    KC_X,    KC_C,    KC_V,    KC_B,           KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_UP,   CKC_VOL,
        KC_LCTL, KC_LGUI, TL_UPPR, KC_LALT,     TD_SPC,                  TD_SPC,       TL_LOWR, KC_LEFT, KC_DOWN, KC_RIGHT
    ),

    [_FN1] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,          KC_F7,   KC_F8,   KC_9,   KC_F10,   KC_F11,  KC_F12,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_VAI, XXXXXXX,
        XXXXXXX, XXXXXXX, _______, XXXXXXX,     XXXXXXX,                 XXXXXXX,      _______, RGB_SPD, RGB_VAD, RGB_SPI
    ),

    [_FN2] = LAYOUT(
        // Note: Using number row keycodes instead of numpad, so we dont care about numlock
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        KC_7,    KC_8,    KC_9,    XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        KC_4,    KC_5,    KC_6,    XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        KC_1,    KC_2,    KC_3,    XXXXXXX, RGB_VAI, XXXXXXX,
        XXXXXXX, XXXXXXX, _______, XXXXXXX,     XXXXXXX,                  KC_0,        _______, RGB_SPD, RGB_VAD, RGB_SPI
    ),

    [_RST] = LAYOUT(
        QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, EE_CLR,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, _______, XXXXXXX,     DB_TOGG,                 DB_TOGG,      _______, XXXXXXX, XXXXXXX, XXXXXXX
    ),
};

// TODO: Change to tri-layer keycodes
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _FN1, _FN2, _RST);
}

#if defined(TOUCH_SCREEN_ENABLE) && defined(INIT_EE_HANDS_RIGHT)
uint32_t touch_timer = 0;
void housekeeping_task_user(void) {
    static bool ili9341_release_notified = true;

    // We only read once in a while
    if (timer_elapsed32(touch_timer) < 1000)
        return;

    touch_timer = timer_read32();

    // Do nothing until sensor initialised or when screen isn't pressed (IRQ high)
    if (ili9341_touch == NULL || ili9341_irq == true) {
        // Notify Tauri about the release if we havent done so yet
        if (!ili9341_release_notified) {
            uint8_t payload[PAYLOAD_SIZE] = { ILI9341_ID }; // x:0, y:0 (no button there) for cleanup
            xap_broadcast(0x03, payload, sizeof(payload));
            ili9341_release_notified = true;
        }
        return;
    }

    // Make a read and send it to Tauri
    touch_report_t ili9341_touch_report = get_spi_touch_report(ili9341_touch, false);
    uint8_t payload[PAYLOAD_SIZE] = { ILI9341_ID, ili9341_touch_report.x & 0xFF, ili9341_touch_report.x >> 8, ili9341_touch_report.y & 0xFF, ili9341_touch_report.y >> 8 };
    xap_broadcast(0x03, payload, sizeof(payload)); // 3 -> user-level message

    ili9341_release_notified = false;

#    if defined(ONE_HAND_ENABLE)
    screen_one_hand(touch_report);
#    endif // ONE_HAND_ENABLE
}
#endif // TOUCH_SCREEN_ENABLE && INIT_EE_HANDS_RIGHT
