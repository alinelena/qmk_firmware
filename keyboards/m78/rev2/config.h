#ifndef CONFIG_REV2_H
#define CONFIG_REV2_H

#define DEVICE_VER      0x0002

#undef MATRIX_ROWS
#undef MATRIX_COLS
/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 13

/*
  [_QW] = KEYMAP(
         0                1               2       3         4      5      6        7       8       9       10             11           12
       KC_ESC,           KC_F1,        KC_F2,   KC_F3,    KC_F4, KC_F5,  KC_F6,   KC_F7,  KC_F8,   KC_F9,  KC_F10,      KC_F11,      KC_F12,
       KC_GRV,            KC_1,         KC_2,    KC_3,     KC_4,  KC_5,   KC_6,    KC_7,   KC_8,    KC_9,    KC_0,     KC_MINS,      KC_EQL,
       KC_TAB,            KC_Q,         KC_W,    KC_E,     KC_R,  KC_T,   KC_Y,    KC_U,   KC_I,    KC_O,    KC_P, KC_LBRACKET, KC_RBRACKET,
      KC_SLSH,            KC_A,         KC_S,    KC_D,     KC_F,  KC_G,   KC_H,    KC_J,   KC_K,    KC_L, KC_SCLN,     KC_QUOT,      KC_ENT,
      KC_LSFT, KC_NONUS_BSLASH,         KC_Z,    KC_X,     KC_C,  KC_V,   KC_B,    KC_N,   KC_M, KC_COMM,  KC_DOT,       KC_UP,     KC_NUHS,
      KC_LCTL,         KC_LGUI, TT(_Fortran), KC_LALT, TT(_CPP),KC_SPC, KC_SPC, KC_RALT, KC_DEL, KC_BSPC, KC_LEFT,     KC_DOWN,      KC_RGHT),
*/
   /*

3V3 GNG  5V  B9  B8  B7  B6  B5  B4  B3 A15 A12 A11 A10  A9  A8 B15 B14 B13 B12
                                                                               USB connector
VBT C13 C14 C15  A0  A1  A2  A3  A4  A5  A6  A7  B0  B1 B10 B11 RST 3V3 GND GND

B12 LED
B13 LED
A1 avoid
A4 avoid
A6 avoid
*/

//                          0    1   2   3   4   5   6
//#define MATRIX_ROW_PINS { C14, C15, A0, A2, A3, A5}
#define MATRIX_ROW_PINS { C14, C15, A0, A2, A3, A7}
//#define MATRIX_COL_PINS { B12, B13, B14, B15,  B6,  B5,  B4,  B3, A10,  A9,  A8,  B0, B1}
//#define MATRIX_COL_PINS { A8, A9, B14, B15,  B6,  B5,  B4,  B3, A15,  B0, B1, B10, B11}
#define MATRIX_COL_PINS { B15, A8,  A9, A10, A15, B3, B4, B5, B6, B11, B10, B1, B0}
//                          0    1   2    3    4   5   6   7   8   9   10   11  12
//#define ENCODERS_PAD_A { B12 }
//#define ENCODERS_PAD_B { B13 }

//#define DIP_SWITCH_PINS { B14, A15, A0, B9 }


#endif
