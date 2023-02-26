/* Copyright 2021-2022 alin elena <alin@elena.space>
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

#if defined(RGBLIGHT_ENABLE)

const rgblight_segment_t PROGMEM my_qwerty_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 4, HSV_OFF});
const rgblight_segment_t PROGMEM my_lwr_layer[]    = RGBLIGHT_LAYER_SEGMENTS({0, 1, HSV_GREEN});
const rgblight_segment_t PROGMEM my_rse_layer[]    = RGBLIGHT_LAYER_SEGMENTS({1, 2, HSV_RED});
const rgblight_segment_t PROGMEM my_adj_layer[]    = RGBLIGHT_LAYER_SEGMENTS({2, 3, HSV_TEAL});
const rgblight_segment_t PROGMEM my_tst_layer[]    = RGBLIGHT_LAYER_SEGMENTS({3, 4, HSV_CORAL});

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(my_qwerty_layer, my_lwr_layer, my_rse_layer, my_adj_layer);

#endif


#if defined(LED_SHIFT_REGISTER)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define LED_DELAY 100

uint8_t PROGMEM led_s = 0;

void pulseHighLow(pin_t pin){
  writePinHigh(pin);
  wait_ms(LED_DELAY);
  writePinLow(pin);

}

void shiftOutShort(pin_t dataPin, pin_t clockPin, uint8_t val){
  for (uint8_t i = 0; i < 8; i++)  {
    writePin(dataPin,!!(led_s & (1 << i)));
    pulseHighLow(clockPin);
  }
}

void leds_off(void) {

  led_s = 0;
  writePinLow(SR_LATCH_PIN);
  shiftOutShort(SR_DATA_PIN, SR_CLOCK_PIN, led_s);
  writePinHigh(SR_LATCH_PIN);
}

void led_on(uint8_t led) {

  led_s |= led;
  writePinLow(SR_LATCH_PIN);
  shiftOutShort(SR_DATA_PIN, SR_CLOCK_PIN, led_s);
  writePinHigh(SR_LATCH_PIN);

}

void led_toggle(uint8_t led) {

  led_s ^= led;
  writePinLow(SR_LATCH_PIN);
  shiftOutShort(SR_DATA_PIN, SR_CLOCK_PIN, led_s);
  writePinHigh(SR_LATCH_PIN);

}


void setup_leds(void) {

  writePinLow(SR_LATCH_PIN);
  setPinOutput(SR_LATCH_PIN);

  writePinLow(SR_DATA_PIN);
  setPinOutput(SR_DATA_PIN);

  writePinLow(SR_CLOCK_PIN);
  setPinOutput(SR_CLOCK_PIN);
}

void count_leds(void){
  uint8_t leds = 0;
  for (uint8_t i=0; i<8; i++ ) {
    bitSet(leds,i);
    led_on(leds);
    wait_ms(LED_DELAY);
  }
  leds_off();
}

#endif

#if defined(RGBLIGHT_ENABLE)

void set_rgb_layers(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _QW));
    rgblight_set_layer_state(1, layer_state_cmp(state, _LWR));
    rgblight_set_layer_state(2, layer_state_cmp(state, _RSE));
    rgblight_set_layer_state(3, layer_state_cmp(state, _ADJ));
    rgblight_set_layer_state(4, layer_state_cmp(state, _TST));
}

void set_default_rgb_layers(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _QW));
}

const rgblight_segment_t* const* my_rgb(void) {
    return my_rgb_layers;
}
#endif

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

void matrix_init_kb(void) {
#if defined(LED_SHIFT_REGISTER)
  setup_leds();
  leds_off();
  count_leds();
#endif
#if defined(CONSOLE_ENABLE)
    debug_enable   = true;
    debug_matrix   = true;
    debug_keyboard = true;
#endif
#if defined(LED_LWR_PIN)
    setPinOutput(LED_LWR_PIN);
    writePin(LED_LWR_PIN, true);
    wait_ms(30);
#endif

#if defined(LED_RSE_PIN)
    setPinOutput(LED_RSE_PIN);
    writePin(LED_RSE_PIN, true);
    wait_ms(30);
#endif
#if defined(LED_CAPS_PIN)
    setPinOutput(LED_CAPS_PIN);
    writePin(LED_CAPS_PIN, true);
    wait_ms(30);
#endif
#if defined(RGBLIGHT_ENABLE)
    // Enable the LED layers
    rgblight_layers = my_rgb();

    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_setrgb(RGB_TEAL);
    wait_ms(1000);
    rgblight_sethsv_at(HSV_RED, 0); // led 0
    wait_ms(1000);
rgblight_sethsv_at(HSV_GREEN,   1); // led 1
    wait_ms(1000);
rgblight_sethsv_at(HSV_BLUE, 2); // led 2
    wait_ms(1000);
rgblight_sethsv_at(HSV_PINK, 3); // led 2
    wait_ms(1000);
rgblight_sethsv_at(HSV_YELLOW, 4); // led 2
    wait_ms(1000);
rgblight_sethsv_at(HSV_TEAL, 5); // led 2
    wait_ms(1000);
rgblight_sethsv_at(HSV_ORANGE, 6); // led 2
    wait_ms(1000);
#endif
    keyboard_post_init_user();
}
