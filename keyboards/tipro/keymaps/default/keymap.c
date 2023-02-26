/*
Copyright 2021-2022 Alin M Elena <alinm.elena@gmail.com>

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

#include QMK_KEYBOARD_H


#define LOWER TT(_LWR)
#define RAISE TT(_RSE)
#define TEST TT(_TST)

#if defined(RGBLIGHT_ENABLE) && !defined(LED_SHIFT_REGISTER)
static bool toggle_caps=false;
static bool toggle_num=false;
static bool toggle_scroll=false;
#endif
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QW] = LAYOUT_ortho_8x16(
    TEST   , KC_BRID   , KC_BRIU    , KC_D   , KC_SLEP    , KC_WAKE  , KC_F  , KC_G  , KC_H   , KC_I   , KC_J   , KC_K   , KC_VOLD, KC_MPLY, KC_VOLU, KC_PWR   ,
    KC_GRV , KC_F13 , KC_F14  , KC_F15 , KC_F16  , KC_F17, KC_F18, KC_F19, KC_F20 , KC_F21 , KC_F22 , KC_F23 , KC_F24 , KC_NUM , KC_R   , KC_S   ,
    KC_ESC , KC_F1  , KC_F2   , KC_F3  , KC_F4   , KC_F5 , KC_F6 , KC_F7 , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 , KC_M   , KC_INS , KC_HOME,
    KC_GRV , KC_1   , KC_2    , KC_3   , KC_4    , KC_5  , KC_6  , KC_7  , KC_8   , KC_9   , KC_0   , KC_MINS, KC_EQL , KC_BSPC, KC_PSLS, KC_PAST,
    KC_TAB , KC_Q   , KC_W    , KC_E   , KC_R    , KC_T  , KC_Y  , KC_U  , KC_I   , KC_O   , KC_P   , KC_LBRC, KC_RBRC, KC_P7  , KC_P8  , KC_P9  ,
    KC_NUHS, KC_A   , KC_S    , KC_D   , KC_F    , KC_G  , KC_H  , KC_J  , KC_K   , KC_L   , KC_SCLN, KC_QUOT, KC_ENT , KC_P4  , KC_P5  , KC_P6  ,
    SC_LSPO, KC_NUHS, KC_Z    , KC_X   , KC_C    , KC_V  , KC_B  , KC_N  , KC_M   , KC_COMM, KC_DOT , KC_UP  , KC_SLSH, KC_P1  , KC_P2  , KC_P4  ,
    KC_LCTL, KC_LGUI, TT(_LWR), KC_LALT, TT(_RSE), KC_SPC, KC_SPC, KC_SPC, KC_RALT, SC_RSPC, KC_LEFT, KC_DOWN, KC_RGHT, KC_APP , KC_P0  , KC_PDOT
    ) ,

[_LWR] = LAYOUT_ortho_8x16(
    KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,  KC_DEL, _______, _______, _______, _______,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, _______, _______, _______, _______,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,S(KC_NUHS),S(KC_NUBS),_______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

[_RSE] = LAYOUT_ortho_8x16(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_DEL, _______, _______, _______, _______,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, _______, _______, _______, _______,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NUHS, KC_NUBS, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

[_ADJ] =  LAYOUT_ortho_8x16(
    _______, QK_BOOT, _______, _______, _______, _______, RGB_TOG, RGB_MOD, RGB_RMOD,RGB_M_G, QK_BOOT, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, RGB_HUD, RGB_SAD, RGB_VAD, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),
[_TST] = LAYOUT_ortho_8x16(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_DEL, _______, _______, _______, _______,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, _______, _______, _______, _______,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NUHS, KC_NUBS, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
)

};
// clang-format on
static inline void led_caps(const bool on) {

#if defined(LED_SHIFT_REGISTER)
   if (on) {
     led_toggle(LED_CL);
   }
#endif
}


void matrix_scan_user(void) {

    led_t led_state = host_keyboard_led_state();
    led_caps(led_state.caps_lock);
#if defined(RGBLIGHT_ENABLE) && !defined(LED_SHIFT_REGISTER)
    bool caps = led_state.caps_lock;
    if ((caps != toggle_caps) && caps){
      rgblight_sethsv_at(HSV_TEAL, 4); // led 2
      toggle_caps = caps;
     }
    if ((caps != toggle_caps) && !caps){
      rgblight_sethsv_at(HSV_TEAL, 4); // led 2
      toggle_caps = caps;
     }
    if (led_state.num_lock || toggle_num){
      rgblight_sethsv_at(HSV_PINK, 5); // led 2
      toggle_num = led_state.num_lock;
    }
    if (led_state.scroll_lock || toggle_scroll){
      rgblight_sethsv_at(HSV_TEAL, 6); // led 2
      toggle_scroll = led_state.scroll_lock;
    }
#endif
}

bool led_update_user(led_t led_state) {
    return false;
}


bool process_record_user(uint16_t keycode, keyrecord_t* record) {
#if defined(CONSOLE_ENABLE)
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
    switch (keycode) {
#if defined(LED_SHIFT_REGISTER)
        case (KC_CAPS):
            led_toggle(LED_CL);
            return true;
            break;
        case (KC_NUM):
            led_toggle(LED_NL);
            return true;
            break;
        case (KC_SCRL):
            led_toggle(LED_SL);
            return true;
            break;
        case (TT(_LWR)):
            if (!record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
                // This runs before the TT() handler toggles the layer state, so the current layer state is the opposite of the final one after toggle.
//                toggle_lwr = !layer_state_is(_LWR);
             led_toggle(LED_L1);
            }
            return true;
            break;
        case (TT(_RSE)):
            if (record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
//                toggle_rse = !layer_state_is(_RSE);
             led_toggle(LED_L2);
            }
            return true;
            break;
        case (TT(_TST)):
            if (record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
//                toggle_rse = !layer_state_is(_RSE);
             led_toggle(LED_L3);
            }
            return true;
            break;
#endif
        default:
            return true;
    }
}
