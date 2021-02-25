![M65 Layout Image](https://i.imgur.com/NexMpvg.png)

# Default M65 Layout

This is the default layout for M65. For the most
part it's a straightforward and easy to follow layout inspired by ISO with a UK turn.
The only unusual key is the key in the upper left, which sends Escape normally, but Grave when in
layer 1 aka LWR.
Similarly number row in layer 1 give the usual audio
KC_GRV, KC_AUDIO_MUTE,  KC_AUDIO_VOL_UP,  KC_AUDIO_VOL_DOWN,   KC_MEDIA_PREV_TRACK, KC_MEDIA_PLAY_PAUSE,  KC_MEDIA_NEXT_TRACK, M_EXTDISP,  KC_SYSTEM_SLEEP, KC_SYSTEM_WAKE, KC_PSCREEN, KC_DEL, KC_EQL
in layer 2 aka RSE one gets F1-F12
mouse emulation can be used in layer 1 and 2.

flashing is done by LWR+RSE+r

full layout below

```
  [_QW] = LAYOUT(
       KC_ESC,            KC_1,         KC_2,    KC_3,     KC_4,  KC_5,   KC_6,    KC_7,   KC_8,    KC_9,    KC_0,     KC_MINS,    KC_BSPC,
       KC_TAB,            KC_Q,         KC_W,    KC_E,     KC_R,  KC_T,   KC_Y,    KC_U,   KC_I,    KC_O,    KC_P, KC_LBRACKET, KC_RBRACKET,
      KC_NUHS,            KC_A,         KC_S,    KC_D,     KC_F,  KC_G,   KC_H,    KC_J,   KC_K,    KC_L, KC_SCLN,     KC_QUOT,      KC_ENT,
      KC_LSPO, KC_NONUS_BSLASH,         KC_Z,    KC_X,     KC_C,  KC_V,   KC_B,    KC_N,   KC_M, KC_COMM,  KC_DOT,       KC_UP,     KC_SLSH,
      KC_LCTL,         KC_LGUI, TT(_LWR), KC_LALT, TT(_RSE),KC_SPC, KC_SPC, KC_SPC, KC_RALT, KC_RSPC, KC_LEFT,     KC_DOWN,    KC_RGHT),

  [_LWR] = LAYOUT(
      KC_GRV, KC_AUDIO_MUTE,  KC_AUDIO_VOL_UP,  KC_AUDIO_VOL_DOWN,   KC_MEDIA_PREV_TRACK, KC_MEDIA_PLAY_PAUSE,  KC_MEDIA_NEXT_TRACK, LGUI(KC_P),  KC_SYSTEM_SLEEP, KC_SYSTEM_WAKE, KC_PSCREEN, KC_DEL, KC_EQL,
      KC_BTN3,  KC_TRNS,  KC_TRNS,    KC_TRNS,  KC_TRNS,  KC_TRNS,     KC_TRNS,  KC_TRNS,   KC_TRNS,    KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS,
      KC_BTN2,  KC_TRNS,  KC_TRNS,    KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS,  KC_BTN1,  KC_TRNS, KC_TRNS,    KC_TRNS,    KC_TRNS,   KC_TRNS,     KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_U, KC_TRNS,
      KC_TRNS,  KC_BTN4,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R),

  [_RSE] = LAYOUT(
      KC_ESC,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
      KC_TRNS, KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,     KC_TRNS,     KC_TRNS,     LALT(KC_F2),     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,    KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_U, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_L, KC_WH_D, KC_WH_R),

  [_ADJ] = LAYOUT(
      KC_TRNS, KC_TRNS,     M_FIND,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,     KC_TRNS,     KC_TRNS,     RESET,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,     M_FIND,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS,    KC_TRNS,     KC_TRNS,     KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_U, KC_TRNS,
      KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_L, KC_WH_D, KC_WH_R),
```
