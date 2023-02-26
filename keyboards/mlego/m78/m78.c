// Copyright 2019-2023 alin elena (@alinelena @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#if defined(RGBLIGHT_ENABLE)

const rgblight_segment_t PROGMEM my_qwerty_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_OFF});
const rgblight_segment_t PROGMEM my_lwr_layer[]    = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_GREEN});
const rgblight_segment_t PROGMEM my_rse_layer[]    = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_RED});
const rgblight_segment_t PROGMEM my_adj_layer[]    = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_CORAL});

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(my_qwerty_layer, my_lwr_layer, my_rse_layer, my_adj_layer);

#endif

// let us assume we start with both layers off
static bool toggle_lwr = false;
static bool toggle_rse = false;

#if defined(RGBLIGHT_ENABLE)

void set_rgb_layers(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _QW));
    rgblight_set_layer_state(1, layer_state_cmp(state, _LWR));
    rgblight_set_layer_state(2, layer_state_cmp(state, _RSE));
    rgblight_set_layer_state(3, layer_state_cmp(state, _ADJ));
}

void set_default_rgb_layers(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _QW));
}

const rgblight_segment_t* const* my_rgb(void) {
    return my_rgb_layers;
}
#endif

void set_led_toggle(const uint8_t layer, const bool state){

    switch (layer) {
        case _LWR:
          toggle_lwr = state;
          break;
        case _RSE:
          toggle_rse = state;
          break;
        default:
          break;
    }
}

void toggle_leds(const bool toggle_lwr, const bool toggle_rse) {
    led_lwr(toggle_lwr);
    led_rse(toggle_rse);
    if (layer_state_is(_ADJ)) {
        led_lwr(true);
        led_rse(true);
    }
}

void matrix_scan_kb(void) {

    matrix_scan_user();
    toggle_leds(toggle_lwr, toggle_rse);

}

layer_state_t layer_state_set_kb(layer_state_t state) {
    state = layer_state_set_user(state);
#if defined(RGBLIGHT_ENABLE)

    set_rgb_layers(state);

#endif

    return update_tri_layer_state(state, _LWR, _RSE, _ADJ);
}

#if defined(RGBLIGHT_ENABLE)

layer_state_t default_layer_state_set_kb(layer_state_t state) {
    state = default_layer_state_set_user(state);

    set_default_rgb_layers(state);

    return state;
}

#endif

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

    switch (keycode) {
        case (TT(_LWR)):
            if (!record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
                // This runs before the TT() handler toggles the layer state, so the current layer state is the opposite of the final one after toggle.
                set_led_toggle(_LWR, !layer_state_is(_LWR));
            }
            return true;
            break;
        case (TT(_RSE)):
            if (record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
                set_led_toggle(_RSE, !layer_state_is(_RSE));
            }
            return true;
            break;
        default:
            return true;
    }
    return process_record_user(keycode, record);
}

void keyboard_post_init_kb(void) {

  keyboard_post_init_user();
  init_lwr_rse_led();

#if defined(RGBLIGHT_ENABLE)
    // Enable the LED layers
    rgblight_layers = my_rgb();
#endif

#ifdef CONSOLE_ENABLE
  debug_enable = true;
  debug_matrix = true;
  debug_keyboard = true;
#endif


}
