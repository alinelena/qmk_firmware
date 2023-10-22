// Copyright 2023 (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include QMK_KEYBOARD_H

#define LOWER TT(_LWR)
#define RAISE TT(_RSE)
#define TEST TT(_TST)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QW] = LAYOUT_ortho_7x14(
    KC_ESC, KC_EQL , KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U  , KC_I, KC_O   , KC_P   , KC_MINS, KC_BSPC,
    KC_TAB, KC_BSLS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J  , KC_K, KC_L   , KC_DOT , KC_ENT ,
    KC_SPC, KC_Z   , KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_SPC,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, LOWER, KC_6, RAISE, KC_8, KC_9  , KC_0, TEST),
[_LWR] = LAYOUT_ortho_7x14(
    KC_ESC, KC_EQL , KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U  , KC_I, KC_O   , KC_P   , KC_MINS, KC_BSPC,
    KC_TAB, KC_BSLS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J  , KC_K, KC_L   , KC_DOT , KC_ENT ,
    KC_SPC, KC_Z   , KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_SPC,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, _______, KC_6, _______, KC_8, KC_9  , KC_0, _______),
[_RSE] = LAYOUT_ortho_7x14(
    KC_ESC, KC_EQL , KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U  , KC_I, KC_O   , KC_P   , KC_MINS, KC_BSPC,
    KC_TAB, KC_BSLS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J  , KC_K, KC_L   , KC_DOT , KC_ENT ,
    KC_SPC, KC_Z   , KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_SPC,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, _______, KC_6, _______, KC_8, KC_9  , KC_0, _______),
[_ADJ] = LAYOUT_ortho_7x14(
    KC_ESC, KC_EQL , KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U  , KC_I, KC_O   , KC_P   , KC_MINS, KC_BSPC,
    KC_TAB, KC_BSLS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J  , KC_K, KC_L   , KC_DOT , KC_ENT ,
    KC_SPC, KC_Z   , KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_SPC,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, _______, KC_6, _______, KC_8, KC_9  , KC_0, _______),
[_TST] = LAYOUT_ortho_7x14(
    KC_ESC, KC_EQL , KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U  , KC_I, KC_O   , KC_P   , KC_MINS, KC_BSPC,
    KC_TAB, KC_BSLS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J  , KC_K, KC_L   , KC_DOT , KC_ENT ,
    KC_SPC, KC_Z   , KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_SPC,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9  , KC_0, KC_COMM, KC_SCLN,
    KC_1  , KC_2   , KC_3, KC_4, _______, KC_6, _______, KC_8, KC_9  , KC_0, _______)
};
// clang-format on
