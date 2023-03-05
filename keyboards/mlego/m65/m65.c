// Copyright 2020-2023 alin m elena (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "m65.h"

#if defined(QUANTUM_PAINTER_ENABLE)
#include "color.h"
#include "graphics.h"
#include "qp.h"
#include "qp_eink_panel.h"
#include "qp_surface.h"
#include "version.h"
painter_device_t ssd1680;
uint8_t ssd1680_buffer[EINK_BYTES_REQD(SSD1680_WIDTH, SSD1680_HEIGHT)] = {0};

uint32_t flush_display(uint32_t trigger_time, void *device) {
    qp_flush((painter_device_t *)device);
    return 0;
}
#if defined(CUSTOM_EEPROM_ENABLE)
#    include "custom_eeprom.h"
#endif // CUSTOM_EEPROM_ENABLE


#define QMK_BH  QMK_BUILDDATE "-" QMK_GIT_HASH
char build_date[] = QMK_BUILDDATE;
char commit_hash[] = QMK_GIT_HASH;
char bh[] = QMK_BH;

#endif

uint32_t deferred_init(uint32_t trigger_time, void *cb_arg) {
    dprint("---------- Init phase ----------\n");

#if defined(QUANTUM_PAINTER_ENABLE)


    wait_ms(1500); //Let screens draw some power
    load_qp_resources();


    ssd1680 = qp_ssd1680_make_spi_device(_SSD1680_WIDTH, _SSD1680_HEIGHT, EINK_CS_PIN, EINK_DC_PIN, EINK_RST_PIN, SPI_DIVISOR, SPI_MODE, (void *)ssd1680_buffer);
    load_display(ssd1680);
    qp_init(ssd1680, SSD1680_ROTATION);


    // show EEPROM state (enable features)
#    if defined(CUSTOM_EEPROM_ENABLE)
    // enabled features based on `#define`s on current firmware
    uint32_t current_config = custom_eeprom_generate();

    // if any change has ocurred, we could run some logic
    if (current_config != eeconfig_read_user()) {
        dprintf("EEPROM config has changed\n");
        eeconfig_update_user(current_config);
        custom_eeprom_draw_config((void *)ssd1680);
    }

#    endif // CUSTOM_EEPROM_ENABLE

    dprint("Quantum painter devices initialised\n");
#ifdef EINK_BWR
#define color HSV_RED
#else
#define color HSV_BLACK
#endif

#if (SSD1680_ROTATION == 0 || SSD1680_ROTATION == 2)

    qp_rect(ssd1680, 0, 0, SSD1680_WIDTH-7, SSD1680_HEIGHT-1, HSV_WHITE, true);
    qp_drawimage_recolor(ssd1680, 40, SSD1680_HEIGHT/2-70, qp_images[7], color, HSV_WHITE);
    qp_drawimage_recolor(ssd1680, 0, 110, qp_images[8], HSV_WHITE, HSV_BLACK);
    qp_rect(ssd1680, 0, 0, SSD1680_WIDTH-7, SSD1680_HEIGHT-1, HSV_BLACK, false);
    char hello[] = "QMK";
    int16_t               hello_width = qp_textwidth(qp_fonts[0], hello);
    qp_drawtext_recolor(ssd1680, SSD1680_WIDTH-hello_width-10, 5, qp_fonts[0],hello,color,HSV_WHITE);
    int16_t               hash_width = qp_textwidth(qp_fonts[0], commit_hash);
    qp_drawtext_recolor(ssd1680, SSD1680_WIDTH-hash_width-10, 5+qp_fonts[0]->line_height, qp_fonts[0], commit_hash, HSV_BLACK, HSV_WHITE);
    int16_t               build_width = qp_textwidth(qp_fonts[1], build_date);
    qp_drawtext_recolor(ssd1680, SSD1680_WIDTH-build_width-10, 5+2.25*qp_fonts[1]->line_height,qp_fonts[1], build_date, color, HSV_WHITE);
#else
    qp_rect(ssd1680, 0, 6, SSD1680_WIDTH-1, SSD1680_HEIGHT-1, HSV_WHITE, true);
    qp_rect(ssd1680, 0, 6, SSD1680_HEIGHT-1, SSD1680_WIDTH-1, HSV_BLACK, false);
    qp_rect(ssd1680, 2, 8, SSD1680_HEIGHT-3, SSD1680_WIDTH-3, color, false);
    char hello[] = "QMK";
    int16_t               hello_width = qp_textwidth(qp_fonts[0], hello);
    qp_drawtext_recolor(ssd1680, SSD1680_HEIGHT-hello_width-10, qp_fonts[0]->line_height, qp_fonts[0],hello,color,HSV_WHITE);
    qp_drawimage_recolor(ssd1680, 40, 20, qp_images[7], color, HSV_WHITE);
    qp_drawimage_recolor(ssd1680, 110, 30, qp_images[8], HSV_WHITE, HSV_BLACK);
    int16_t               bh_width = qp_textwidth(qp_fonts[1], bh);
    qp_drawtext_recolor(ssd1680, SSD1680_HEIGHT-bh_width-10, SSD1680_WIDTH-1.25*qp_fonts[1]->line_height , qp_fonts[1], bh, HSV_BLACK, HSV_WHITE);
#endif
    eink_panel_dc_reset_painter_device_t *eink = (eink_panel_dc_reset_painter_device_t *)ssd1680;
    defer_exec(eink->timeout, flush_display, (void *)ssd1680);
    dprint("Quantum painter ready\n");
#endif // QUANTUM_PAINTER_ENABLE


    dprint("\n---------- User code ----------\n");

    // =======
    // Call user code
    keyboard_post_init_user();

    return 0;
}
void toggle_leds(const bool toggle_lwr, const bool toggle_rse) {
#if defined(QUANTUM_PAINTER_ENABLE)
    static uint32_t last_draw = 0;
    if (timer_elapsed32(last_draw) > 10000) { // Throttle to 30fps
        last_draw = timer_read32();
    switch (get_highest_layer(layer_state)) {
        case _QW:
          {
    char hello[] = "Layer: default";
    int16_t               hello_width = qp_textwidth(qp_fonts[1], hello);
    qp_drawtext_recolor(ssd1680, SSD1680_HEIGHT-hello_width-10, SSD1680_WIDTH-5*qp_fonts[1]->line_height , qp_fonts[1], hello, HSV_BLACK, HSV_WHITE);
                 }
    break;
        case _LWR:
            {
    char hello[] = "Layer: lower";
    int16_t               hello_width = qp_textwidth(qp_fonts[1], hello);
    qp_drawtext_recolor(ssd1680, SSD1680_HEIGHT-hello_width-10, SSD1680_WIDTH-5*qp_fonts[1]->line_height , qp_fonts[1], hello, HSV_BLACK, HSV_WHITE);
            }
    break;
        case _RSE:{
    char hello[] = "Layer:raise";
    int16_t               hello_width = qp_textwidth(qp_fonts[1], hello);
    qp_drawtext_recolor(ssd1680, SSD1680_HEIGHT-hello_width-10, SSD1680_WIDTH-5*qp_fonts[1]->line_height , qp_fonts[1], hello, HSV_BLACK, HSV_WHITE);
    }
    break;
        case _ADJ:
            {
    char hello[] = "Layer: adjust";
    int16_t               hello_width = qp_textwidth(qp_fonts[1], hello);
    qp_drawtext_recolor(ssd1680, SSD1680_HEIGHT-hello_width-10, SSD1680_WIDTH-5*qp_fonts[1]->line_height , qp_fonts[1], hello, HSV_BLACK, HSV_WHITE);
   }
    break;
        default:
    }
    }

#endif
    led_lwr(toggle_lwr);
    led_rse(toggle_rse);
    if (layer_state_is(_ADJ)) {
        led_lwr(true);
        led_rse(true);
    }
}

void keyboard_post_init_kb(void) {
    init_lwr_rse_led();

#if defined(QUANTUM_PAINTER_ENABLE)
    defer_exec(INIT_DELAY, deferred_init, NULL);
#endif
    keyboard_post_init_user();
}
