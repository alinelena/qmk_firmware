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

#pragma once

#include "quantum.h"

#define LED_SL 1
#define LED_CL 2
#define LED_NL 4
#define LED_L4 8
#define LED_L3 16
#define LED_L2 32
#define LED_L1 64

enum layer_names {
    _QW = 0,
    _LWR,
    _RSE,
    _ADJ,
    _TST
};

#if defined(RGBLIGHT_ENABLE)
void                             set_rgb_layers(layer_state_t);
const rgblight_segment_t* const* my_rgb(void);
void                             set_default_rgb_layers(layer_state_t);
#endif


void shiftOutShort(pin_t, pin_t, uint8_t);
void leds_off(void);
void count_leds(void);
void setup_leds(void);
void led_on(uint8_t);
void led_toggle(uint8_t);
