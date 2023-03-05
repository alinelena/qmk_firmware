// Copyright 2020-2023 alin m elena (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

#if defined(QUANTUM_PAINTER_ENABLE)
#include "qp.h"
extern painter_device_t ssd1680;
#endif // QUANTUM_PAINTER_ENABLE

enum layer_names {
    _QW = 0,
    _LWR,
    _RSE,
    _ADJ
};

void toggle_leds(const bool, const bool);
void set_led_toggle(const uint8_t, const bool);

static inline void init_lwr_rse_led(void) {
#ifdef LED_LWR_PIN
    setPinOutput(LED_LWR_PIN);
    writePin(LED_LWR_PIN, false);
    wait_ms(30);
#endif

#ifdef LED_RSE_PIN
    setPinOutput(LED_RSE_PIN);
    writePin(LED_RSE_PIN, false);
    wait_ms(30);
#endif

}

static inline void led_lwr(const bool on) {
#ifdef LED_LWR_PIN
    if ((PRODUCT_ID == 0x6064) || (PRODUCT_ID == 0x6065) ||
        (PRODUCT_ID == 0x6066) || (PRODUCT_ID == 0x6067) ) {
        writePin(LED_LWR_PIN, !on);
    }else{
        writePin(LED_LWR_PIN, on);
    }
#endif
}

static inline void led_rse(const bool on) {
#ifdef LED_RSE_PIN
    if ((PRODUCT_ID == 0x6064) || (PRODUCT_ID == 0x6065) ||
        (PRODUCT_ID == 0x6066) || (PRODUCT_ID == 0x6067) ) {
        writePin(LED_RSE_PIN, !on);
    }else{
        writePin(LED_RSE_PIN, on);
    }
#endif
}
