// Copyright 2019-2023 alin elena (@alinelena @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later
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

void toggle_leds(const bool , const bool ) ;
void set_led_toggle(const uint8_t, const bool);

static inline void init_lwr_rse_led(void) {
#if defined(LED_LWR_PIN)
    setPinOutput(LED_LWR_PIN);
    writePin(LED_LWR_PIN, false);
    wait_ms(30);
#endif

#if defined(LED_RSE_PIN)
    setPinOutput(LED_RSE_PIN);
    writePin(LED_RSE_PIN, false);
    wait_ms(30);
#endif
}

static inline void led_lwr(const bool on) {
#if defined(LED_LWR_PIN)
        writePin(LED_LWR_PIN, on);
#endif
}

static inline void led_rse(const bool on) {
#if defined(LED_RSE_PIN)
        writePin(LED_RSE_PIN, on);
#endif
}
