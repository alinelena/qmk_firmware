/*
Copyright 2021 Alin M Elena <alinm.elena@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "m65.h"

#define TAP(keycode) tap_code16(keycode)

enum custom_keycodes {
  M_EXTDISP = SAFE_RANGE,
  M_FIND
};

  enum layer_names {
    _QW = 0,
    _LWR,
    _RSE,
    _ADJ
    };


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QW] = LAYOUT(
       KC_ESC,            KC_1,         KC_2,    KC_3,     KC_4,  KC_5,   KC_6,    KC_7,   KC_8,    KC_9,    KC_0,     KC_MINS,    KC_BSPC,
       KC_TAB,            KC_Q,         KC_W,    KC_E,     KC_R,  KC_T,   KC_Y,    KC_U,   KC_I,    KC_O,    KC_P, KC_LBRACKET, KC_RBRACKET,
      KC_NUHS,            KC_A,         KC_S,    KC_D,     KC_F,  KC_G,   KC_H,    KC_J,   KC_K,    KC_L, KC_SCLN,     KC_QUOT,      KC_ENT,
      KC_LSPO, KC_NONUS_BSLASH,         KC_Z,    KC_X,     KC_C,  KC_V,   KC_B,    KC_N,   KC_M, KC_COMM,  KC_DOT,       KC_UP,     KC_SLSH,
      KC_LCTL,         KC_LGUI, TT(_LWR), KC_LALT, TT(_RSE),KC_SPC, KC_SPC, KC_SPC, KC_RALT, KC_RSPC, KC_LEFT,     KC_DOWN,    KC_RGHT),

  [_LWR] = LAYOUT(
      KC_GRV, KC_AUDIO_MUTE,  KC_AUDIO_VOL_UP,  KC_AUDIO_VOL_DOWN,   KC_MEDIA_PREV_TRACK, KC_MEDIA_PLAY_PAUSE,  KC_MEDIA_NEXT_TRACK, M_EXTDISP,  KC_SYSTEM_SLEEP, KC_SYSTEM_WAKE, KC_PSCREEN, KC_DEL, KC_EQL,
      KC_BTN3,  KC_TRNS,  KC_TRNS,    KC_TRNS,  KC_TRNS,  KC_TRNS,     KC_TRNS,  KC_TRNS,   KC_TRNS,    KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS,
      KC_BTN2,  KC_TRNS,  KC_TRNS,    KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS,  KC_BTN1,  KC_TRNS, KC_TRNS,    KC_TRNS,    KC_TRNS,   KC_TRNS,     KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_U, KC_TRNS,
      KC_TRNS,  KC_BTN4,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R),

  [_RSE] = LAYOUT(
      KC_ESC,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
      KC_TRNS, KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,     M_FIND,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,    KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_U, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_L, KC_WH_D, KC_WH_R),

  [_ADJ] = LAYOUT(
      KC_TRNS, KC_TRNS,     M_FIND,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,     KC_TRNS,     KC_TRNS,     RESET,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,     M_FIND,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,    KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_U, KC_TRNS,
      KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_L, KC_WH_D, KC_WH_R),
};

static inline void led_lwr(const bool on){

  if (on){
#ifdef KEYBOARD_m65_rev1
    writePinHigh(B13);
#endif
  } else {
#ifdef KEYBOARD_m65_rev1
    writePinLow(B13);
#endif
  }

}

static inline void led_rse(const bool on){

  if (on){
#ifdef KEYBOARD_m65_rev1
    writePinHigh(B12);
#endif
  } else {
#ifdef KEYBOARD_m65_rev1
    writePinLow(B12);
#endif
  }

}

void matrix_init_user(void) {
#ifdef KEYBOARD_m65_rev1
  setPinOutput(B13);
  setPinOutput(B12);
#endif
}

void matrix_scan_user(void) {

  led_lwr(IS_LAYER_ON(_LWR));
  led_rse(IS_LAYER_ON(_RSE));
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  if (!record->event.pressed) return true; /* not entirely sure you should always be returning `true` here */

  switch (keycode) {
    case M_EXTDISP:
        TAP(LGUI(KC_P));
      break;
    case M_FIND:
        TAP(LALT(KC_F2));
      break;
  }
  return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LWR, _RSE, _ADJ);
}
