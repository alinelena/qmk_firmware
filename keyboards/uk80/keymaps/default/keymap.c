/* Copyright 2018 REPLACE_WITH_YOUR_NAME
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
  QMKBEST = SAFE_RANGE,
  QMKURL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * ,------------------------------------------------------------
 * |Esc|   | F1| F2| F3| F4| | F5| f6| F7| F8| | F9|F10|Ins|F12|
 * ,------------------------------------------------------------
 * |`¬ |  1|  2|  3|  4|  5|  6|  7|  8|  9|  0| -_| =+|Del|Bsp|
 * |------------------------------------------------------------
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P| [{| ]}|Enter|
 * |------------------------------------------------------.    -
 * |Caps  |  A|  S|  D|  F|  G|  H|  J|  K|  L| ;:| '@| #~|    |
 * |------------------------------------------------------------
 * |Sft| \||  Z|  X|  C|  V|  B|  N|  M| ,<| .>| /?| Up| Shift |
 * |------------------------------------------------------------
 * |Ctrl|Mo1)| Alt| space | space | space | Alt|Lft|Dwn|Rgh|Ctl|
 * `------------------------------------------------------------
*/
[0]=LAYOUT_80_iso(
    KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_INS, KC_F12,
    KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_DEL, KC_BSPC,
    KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC,
    KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NONUS_HASH, KC_ENT,
    KC_LSPO, KC_NUBS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_UP, KC_RSPC,
    KC_LCTL, TG(1), KC_LALT, KC_SPC, KC_SPC, KC_SPC, KC_RALT, KC_LEFT, KC_DOWN, KC_RGHT, KC_RCTL),
/*
 * ,------------------------------------------------------------
 * |   |   |   |   |   |   | | Hm|End|PgU|PgD| |   |   |F11|   |
 * ,------------------------------------------------------------
 * | x |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * |------------------------------------------------------------
 * |     |   |   |   |   |   |   |   |   |   |   |   |   |     |
 * |------------------------------------------------------.    -
 * |      |   |   |   |   |   |   |   |   |   |   |   |  ~|    |
 * |------------------------------------------------------------
 * |  |   |   |   |   |   |   |   |   |   |    |   |   |       |
 * |------------------------------------------------------------
 * |    |    |    |       | reset |       |    |   |   |   |   |
 * `------------------------------------------------------------
*/

[1]=LAYOUT_80_iso(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_END, KC_PGUP, KC_PGDN, KC_TRNS, KC_TRNS, KC_F11, KC_TRNS,
    KC_X, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RESET, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

/*
 * ,------------------------------------------------------------
 * |   |   |   |   |   |   | |   |   |   |   | |   |   |   |   |
 * ,------------------------------------------------------------
 * |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * |------------------------------------------------------------
 * |     |   |   |   |   |   |   |   |   |   |   |   |   |     |
 * |------------------------------------------------------.    -
 * |      |   |   |   |   |   |   |   |   |   |   |   |  ~|    |
 * |------------------------------------------------------------
 * |  |   |   |   |   |   |   |   |   |   |    |   |   |       |
 * |------------------------------------------------------------
 * |    |    |    |       |       |       |    |   |   |   |   |
 * `------------------------------------------------------------
*/
[2]=LAYOUT_80_iso(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

/*
 * ,------------------------------------------------------------
 * |   |   |   |   |   |   | |   |   |   |   | |   |   |   |   |
 * ,------------------------------------------------------------
 * |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * |------------------------------------------------------------
 * |     |   |   |   |   |   |   |   |   |   |   |   |   |     |
 * |------------------------------------------------------.    -
 * |      |   |   |   |   |   |   |   |   |   |   |   |  ~|    |
 * |------------------------------------------------------------
 * |  |   |   |   |   |   |   |   |   |   |    |   |   |       |
 * |------------------------------------------------------------
 * |    |    |    |       |       |       |    |   |   |   |   |
 * `------------------------------------------------------------
*/
[3]=LAYOUT_80_iso(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return true;
}

void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

void led_set_user(uint8_t usb_led) {
/*
  if (usb_led & (1 << USB_LED_CAPS_LOCK)) {
    DDRE |= (1 << 0); 
    PORTE &= ~(1 << 0);
  } else {
    DDRE &= ~(1 << 0); 
    PORTE &= ~(1 << 0);
  }
*/
}
