// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_6x4(
        KC_MUTE,
        KC_0,   KC_PSLS, KC_PAST, KC_PMNS,
        KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_P4,   KC_P5,   KC_P6,   KC_PERC,
        KC_P1,   KC_P2,   KC_P3,   KC_EQL,
        TT(_LWR),   KC_X,  TT(_RSE), KC_PENT
    ),
    [1] = LAYOUT_ortho_6x4(
        _______,
        _______, _______, _______, _______,
        KC_HOME, KC_UP,   KC_PGUP, _______,
        KC_LEFT, _______, KC_RGHT, _______,
        KC_END,  KC_DOWN, KC_PGDN, _______,
        _______, _______, _______, _______
    ),
    [2] = LAYOUT_ortho_6x4(
        _______,
        _______, _______, _______, _______,
        KC_HOME, KC_UP,   KC_PGUP, _______,
        KC_LEFT, _______, KC_RGHT, _______,
        KC_END,  KC_DOWN, KC_PGDN, _______,
        _______, _______, _______, _______
    ),
    [3] = LAYOUT_ortho_6x4(
        _______,
        _______, _______, _______, _______,
        KC_HOME, KC_UP,   KC_PGUP, _______,
        KC_LEFT, _______, KC_RGHT, _______,
        KC_END,  KC_DOWN, KC_PGDN, _______,
        _______, _______, _______, _______
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QW] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
#    if defined(RGBLIGHT_ENABLE)
    [_LWR] = {ENCODER_CCW_CW(RGB_HUD, RGB_HUI)},
    [_RSE] = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
    [_ADJ] = {ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)},
#    else
    [_LWR] = {ENCODER_CCW_CW(KC_MNXT, KC_MPRV)},
    [_RSE] = {ENCODER_CCW_CW(KC_MFFD, KC_MRWD)},
    [_ADJ] = {ENCODER_CCW_CW(KC_PGDN, KC_PGUP)},
#    endif
};
#endif
