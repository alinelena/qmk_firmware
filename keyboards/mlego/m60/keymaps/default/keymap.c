// Copyright 2021-2023 alin m elena (@alinelena)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#define LOWER TT(_LWR)
#define RAISE TT(_RSE)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QW] = LAYOUT_ortho_5x12(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
    KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL,
    KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT ,
    KC_LCTL, KC_LGUI, KC_LALT, KC_APP,  LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

[_LWR] = LAYOUT_ortho_5x12(
    KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,S(KC_NUHS),S(KC_NUBS),_______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

[_RSE] = LAYOUT_ortho_5x12(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NUHS, KC_NUBS, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

[_ADJ] =  LAYOUT_ortho_5x12(
    _______, QK_BOOT, _______, _______, _______, _______, UG_TOGG, UG_NEXT, UG_PREV, RGB_M_G, QK_BOOT, _______,
    _______, _______, _______, _______, _______, _______, UG_HUEU, UG_SATU, UG_VALU, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, UG_HUED, UG_SATD, UG_VALD, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
)

};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QW]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
#    ifdef RGBLIGHT_ENABLE
    [_LWR] = {ENCODER_CCW_CW(RM_HUED, RM_HUEU)},
    [_RSE] = {ENCODER_CCW_CW(RM_VALD, RM_VALU)},
    [_ADJ] = {ENCODER_CCW_CW(RM_PREV, RM_NEXT)},
#    else
    [_LWR] = {ENCODER_CCW_CW(KC_MNXT, KC_MPRV)},
    [_RSE] = {ENCODER_CCW_CW(KC_MFFD, KC_MRWD)},
    [_ADJ] = {ENCODER_CCW_CW(KC_PGDN, KC_PGUP)},
#    endif
};
#endif
