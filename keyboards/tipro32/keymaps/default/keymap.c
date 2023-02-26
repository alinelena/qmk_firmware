
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
enum layer_names {
    _QW = 0,
    _LWR,
    _RSE,
    _ADJ
};

#define LOWER TT(_LWR)
#define RAISE TT(_RSE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QW] = LAYOUT_ortho_8x4(
        KC_ESC  , KC_PGUP, KC_HOME, KC_BSPC ,
        KC_TAB  , KC_PGDN, KC_END , KC_DEL ,
        KC_RIGHT, KC_LEFT, KC_UP  , KC_DOWN,
        KC_7    , KC_8   , KC_9   , KC_PSLS,
        KC_4    , KC_5   , KC_6   , KC_PAST,
        KC_1    , KC_2   , KC_3   , KC_PMNS,
        KC_0    , KC_DOT , KC_EQL , KC_PPLS,
        LOWER   , KC_SPC , KC_ENT , RAISE
    )           ,

    [_LWR] = LAYOUT_ortho_8x4(
        KC_GRV  , KC_A   , KC_B   , KC_C   ,
        KC_D    , KC_E   , KC_F   , KC_G   ,
        KC_H    , KC_I   , KC_J   , KC_K   ,
        KC_L    , KC_M   , KC_N   , KC_O   ,
        KC_P    , KC_Q   , KC_R   , KC_S   ,
        KC_T    , KC_U   , KC_V   , KC_W   ,
        KC_X    , KC_Y   , KC_Z   , KC_SCLN,
        _______ , KC_QUOT, KC_COMM, _______
    )           ,

    [_RSE] = LAYOUT_ortho_8x4(
        KC_NUHS , KC_LBRC, KC_RBRC, KC_BSLS,
        KC_INS  , _______, _______, _______,
        _______ , _______, _______, _______,
        _______ , _______, _______, _______,
        _______ , _______, _______, _______,
        _______ , _______, _______, _______,
        _______ , _______, _______, _______,
        _______ , _______, _______, _______
    )           ,

    [_ADJ] = LAYOUT_ortho_8x4(
        QK_BOOT, KC_Z   , _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______
    )
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LWR, _RSE, _ADJ);
}
