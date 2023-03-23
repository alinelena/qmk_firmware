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


enum unicode_names {
    la = 0,
    lA,
    lb,
    lB,
    lc,
    lC,
    ld,
    lD,
    le,
    lE,
    lf,
    lF,
    lg,
    lG,
    lh,
    lH,
    li,
    lI,
    lj,
    lJ,
    lk,
    lK,
    ll,
    lL,
    lm,
    lM,
    ln,
    lN,
    lo,
    lO,
    lp,
    lP,
    lq,
    lQ,
    lr,
    lR,
    ls,
    lS,
    lt,
    lT,
    lu,
    lU,
    lv,
    lV,
    lw,
    lW,
    lx,
    lX,
    ly,
    lY,
    lz,
    lZ,
    lc1,
    lC1,
    lp1,
    lP1,
    lq1,
    lQ1,
    ll1,
    lL1,
    lk1,
    lK1,
    lbo1,
    lbo2,
    lbc1,
    lbc2,
    ls1,
    ls2,
    ra,
    rA,
    ra1,
    rA1,
    rc,
    rC,
    ri,
    rI,
    rs,
    rS,
    rt,
    rT,
    ro,
    rO,
    ra2,
    rA2,
    ra3,
    rA3,
    rx,
    rX,
    rv,
    rV,
    rb,
    rB,
    rn,
    rN
};

enum layer_names {
    _QW = 0,
    _LWR,
    _RSE,
    _ADJ
};

#ifdef OLED_ENABLE
void user_oled_magic(void);
void render_logo(void);
void clear_screen(void);
void init_timer(void);
#endif

#ifdef RGBLIGHT_ENABLE
void set_rgb_layers(layer_state_t);
const rgblight_segment_t * const*  my_rgb(void);
void set_default_rgb_layers(layer_state_t);
#endif


void toggle_leds(void);
void set_led_toggle(const uint8_t, const bool);

static inline void init_lwr_rse_led(void) {
#ifdef LED_LWR_PIN
    setPinOutput(LED_LWR_PIN);
    writePin(LED_LWR_PIN, 1);
    wait_ms(30);
#endif

#ifdef LED_RSE_PIN
    setPinOutput(LED_RSE_PIN);
    writePin(LED_RSE_PIN, 1);
    wait_ms(30);
#endif
}

static inline void led_lwr(const bool on) {
#ifdef LED_LWR_PIN
    writePin(LED_LWR_PIN, !on);
#endif
}

static inline void led_rse(const bool on) {
#ifdef LED_RSE_PIN
    writePin(LED_RSE_PIN, !on);
#endif
}
static inline void led_caps(const bool on) {
#ifdef LED_CAPS_LOCK_PIN
    writePin(LED_CAPS_LOCK_PIN, !on);
#endif
}
