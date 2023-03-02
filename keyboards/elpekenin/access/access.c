// Copyright 2023 Pablo Martinez (@elpekenin) <elpekenin@elpekenin.dev>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "access.h"
#include "color.h"
#include "custom_spi_master.h"
#include "graphics.h"
#include "qp.h"
#include "sipo_pins.h"
#include "version.h"

#if defined(CUSTOM_EEPROM_ENABLE)
#    include "custom_eeprom.h"
#endif // CUSTOM_EEPROM_ENABLE

#if defined(ONE_HAND_ENABLE)
uint8_t one_hand_col;
uint8_t one_hand_row;
one_hand_movement_t one_hand_movement;
#endif // ONE_HAND_ENABLE

// eInk is on left side, dont allocate framebuffer on right
#if defined(INIT_EE_HANDS_LEFT)
#    include "qp_eink_panel.h"
painter_device_t il91874;
uint8_t il91874_buffer[EINK_BYTES_REQD(IL91874_WIDTH, IL91874_HEIGHT)];

void draw_commit(painter_device_t device) {
    painter_driver_t *    driver     = (painter_driver_t *)device;
    painter_font_handle_t font       = qp_fonts[0];
    uint16_t              dev_width  = driver->panel_width;
    uint16_t              dev_height = driver->panel_height;
    int16_t               hash_width = qp_textwidth(font, QMK_GIT_HASH);

    uint16_t x = dev_width - hash_width;
    uint16_t y = dev_height-font->line_height;

    qp_drawtext_recolor(device, x, y, font, QMK_GIT_HASH, HSV_BLACK, HSV_WHITE);
}

uint32_t flush_display(uint32_t trigger_time, void *device) {
    qp_flush((painter_device_t *)device);
    return 0;
}
#else
painter_device_t ili9163;
painter_device_t ili9341;
#endif // INIT_EE_HANDS_LEFT


#if defined (TOUCH_SCREEN_ENABLE) && defined(INIT_EE_HANDS_RIGHT)
#    include "touch_driver.h"
touch_device_t ili9341_touch;
bool ili9341_irq = true;
#endif // TOUCH_SCREEN_ENABLE && INIT_EE_HANDS_RIGHT

uint32_t deferred_init(uint32_t trigger_time, void *cb_arg) {
    dprint("---------- Init phase ----------\n");

    // =======
    // Power indicator
    setPinOutput(POWER_LED_PIN);
    writePinHigh(POWER_LED_PIN);

    // =======
    // QP
    load_qp_resources();
    wait_ms(150); //Let screens draw some power

#if defined(INIT_EE_HANDS_LEFT)
    configure_sipo_pins(
        __PADDING__,
        IL91874_RST_PIN,
        IL91874_SRAM_CS_PIN,
        SCREENS_DC_PIN,
        IL91874_CS_PIN
    );

    il91874 = qp_il91874_make_spi_device(_IL91874_WIDTH, _IL91874_HEIGHT, IL91874_CS_PIN, SCREENS_DC_PIN, IL91874_RST_PIN, SCREENS_SPI_DIV, SCREENS_SPI_MODE, (void *)il91874_buffer);
    load_display(il91874);
    qp_init(il91874, IL91874_ROTATION);

    // draw on it after timeout, preventing damage if replug fast
    eink_panel_dc_reset_painter_device_t *eink = (eink_panel_dc_reset_painter_device_t *)il91874;
    defer_exec(eink->timeout, flush_display, (void *)eink);

    // show EEPROM state (enabled features)
#    if defined(CUSTOM_EEPROM_ENABLE)
    // see features on current firmware
    uint32_t current_config = custom_eeprom_generate();

    // if current firmware contains any change, we could run some logic here
    // so far, we just update EEPROM value
    if (current_config != eeconfig_read_user()) {
        dprintf("EEPROM config has changed\n");
        eeconfig_update_user(current_config);
    }

    custom_eeprom_draw_config((void *)il91874);
#    endif // CUSTOM_EEPROM_ENABLE

    draw_commit(il91874);

    dprint("IL91874 initialised\n");
#else // --------------------  Right half --------------------
    configure_sipo_pins(
        ILI9341_TOUCH_CS_PIN,
        SCREENS_DC_PIN,
        ILI9163_RST_PIN,
        ILI9163_CS_PIN,
        ILI9341_RST_PIN,
        ILI9341_CS_PIN
    );

    ili9163 = qp_ili9163_make_spi_device(_ILI9163_WIDTH, _ILI9163_HEIGHT, ILI9163_CS_PIN, SCREENS_DC_PIN, ILI9163_RST_PIN, SCREENS_SPI_DIV, SCREENS_SPI_MODE);
    load_display(ili9163);
    qp_init(ili9163, ILI9163_ROTATION);

    ili9341 = qp_ili9341_make_spi_device(_ILI9341_WIDTH, _ILI9341_HEIGHT, ILI9341_CS_PIN, SCREENS_DC_PIN, ILI9341_RST_PIN, SCREENS_SPI_DIV, SCREENS_SPI_MODE);
    load_display(ili9341);
    qp_init(ili9341, ILI9341_ROTATION);

    qp_rect(ili9163, 0, 0, ILI9163_WIDTH, ILI9163_HEIGHT, HSV_BLACK, true);
    qp_rect(ili9341, 0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, HSV_BLACK, true);

    dprint("ILI9163 and ILI9341 initialised\n");
#endif


#if defined (TOUCH_SCREEN_ENABLE) && defined(INIT_EE_HANDS_RIGHT)
    // Calibration isn't very precise
    static touch_driver_t ili9341_touch_driver = {
        .width = _ILI9341_WIDTH,
        .height = _ILI9341_HEIGHT,
        .scale_x = 0.07,
        .scale_y = -0.09,
        .offset_x = -26,
        .offset_y = 345,
        .rotation = ILI9341_ROTATION,
        .upside_down = false,
        .spi_config = {
            .chip_select_pin = ILI9341_TOUCH_CS_PIN,
            .divisor = SCREENS_SPI_DIV,
            .lsb_first = false,
            .mode = SCREENS_SPI_MODE,
            .irq_pin = NO_PIN,
            .x_cmd = 0xD0,
            .y_cmd = 0x90
        },
    };
    ili9341_touch = &ili9341_touch_driver;
    touch_spi_init(ili9341_touch);

    dprint("ILI9341 touch device initialised\n");
#endif // TOUCH_SCREEN_ENABLE && INIT_EE_HANDS_RIGHT

    dprint("\n---------- User code ----------\n");

    // =======
    // Call user code
    keyboard_post_init_user();

    return 0;
}

void keyboard_post_init_kb(void) {
    debug_enable = true;
    debug_matrix = true;

    defer_exec(INIT_DELAY, deferred_init, NULL);
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) {
        return false;
    }

    return true;
}

void matrix_init_custom(void) {
    setPinOutput(PISO_CS_PIN);
    writePinHigh(PISO_CS_PIN);
    custom_spi_init(REGISTERS_SPI_DRIVER_ID);
}

#if defined(ONE_HAND_ENABLE) && defined (TOUCH_SCREEN_ENABLE) && defined(INIT_EE_HANDS_RIGHT)
void screen_one_hand(touch_report_t touch_report) {
    int16_t x = touch_report.x - ILI9341_WIDTH / 2;
    int16_t y = touch_report.y - ILI9341_HEIGHT / 2;

    if (x > 30) {
        one_hand_movement = y > 0 ? DIRECTION_RIGHT : DIRECTION_LEFT;
    } else if (x < -30){
        one_hand_movement = y > 0 ? DIRECTION_UP : DIRECTION_DOWN;
    }
}
#endif // ONE_HAND_ENABLE && TOUCH_SCREEN_ENABLE && INIT_EE_HANDS_RIGHT
