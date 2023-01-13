// Copyright 2022 Pablo Martinez (@elpekenin)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "access.h"
#include "version.h"

#if defined(SIPO_PINS_ENABLE)
#    include "sipo_pins.h"
#endif // SIPO_PINS_ENABLE

#if defined(QUANTUM_PAINTER_ENABLE)
#    include "color.h"
#    include "graphics.h"
#    include "qp.h"
#    include "qp_eink_panel.h"
#    include "qp_surface.h"
painter_device_t il91874;
painter_device_t ili9163;
painter_device_t ili9341;
painter_device_t ili9486;
painter_device_t ssd1680;
uint8_t il91874_buffer[EINK_BYTES_REQD(IL91874_WIDTH, IL91874_HEIGHT)] = {0};
uint8_t ssd1680_buffer[EINK_BYTES_REQD(SSD1680_WIDTH, SSD1680_HEIGHT)] = {0};
#endif // QUANTUM_PAINTER_ENABLE

#if defined(ONE_HAND_ENABLE)
uint8_t one_hand_col;
uint8_t one_hand_row;
one_hand_movement_t one_hand_movement;
#endif // ONE_HAND_ENABLE

#if defined (TOUCH_SCREEN_ENABLE)
#    include "touch_driver.h"
touch_device_t ili9341_touch;
touch_device_t ili9486_touch;
#endif // TOUCH_SCREEN_ENABLE

// Version info
char build_date [] = QMK_BUILDDATE;
char commit_hash [] = QMK_GIT_HASH;

void keyboard_post_init_kb(void) {
    debug_enable = true;

#if defined(DEFERRED_EXEC_ENABLE)
    // Define function so `defer_exec` doesn't crash the compiling
    uint32_t deferred_init(uint32_t trigger_time, void *cb_arg);

    // Defer init code so USB has started and we can receive console messages
    defer_exec(INIT_DELAY, deferred_init, NULL);
}

uint32_t deferred_init(uint32_t trigger_time, void *cb_arg) {
#endif // DEFERRED_EXEC_ENABLE

    dprint("---------- Init phase ----------\n");
    printf("firmware built on %s with commit: %s\n", build_date, commit_hash);

    // =======
    // Power indicator
    setPinOutput(POWER_LED_PIN);
    writePinHigh(POWER_LED_PIN);

#if defined(QUANTUM_PAINTER_ENABLE)
    // // Virtual pins names
    // configure_sipo_pins(
    //     SCREEN_SPI_DC_PIN,
    //     IL91874_CS_PIN, IL91874_RST_PIN,
    //     ILI9163_CS_PIN, ILI9163_RST_PIN,
    //     ILI9341_CS_PIN, ILI9341_RST_PIN,
    //     ILI9486_CS_PIN, ILI9486_RST_PIN
    //     // touch screen code isn't adjusted for SIPO yet
    //     // ,ILI9341_TOUCH_CS_PIN, ILI9486_TOUCH_CS_PIN
    // );

    load_qp_resources();

    wait_ms(150); //Let screens draw some power

    // ----- Init screens
    // il91874 = qp_il91874_make_spi_device(_IL91874_WIDTH, _IL91874_HEIGHT, IL91874_CS_PIN, SCREEN_SPI_DC_PIN, IL91874_RST_PIN, SPI_DIV, SPI_MODE, (void *)il91874_buffer);
    // qp_init(il91874, IL91874_ROTATION);

    // ili9163 = qp_ili9163_make_spi_device(ILI9163_WIDTH, ILI9163_HEIGHT, ILI9163_CS_PIN, SCREEN_SPI_DC_PIN, ILI9163_RST_PIN, SPI_DIV, SPI_MODE);
    // qp_init(ili9163, ILI9163_ROTATION);

    // ili9341 = qp_ili9341_make_spi_device(_ILI9341_WIDTH, _ILI9341_HEIGHT, ILI9341_CS_PIN, SCREEN_SPI_DC_PIN, ILI9341_RST_PIN, SPI_DIV, SPI_MODE);
    // qp_init(ili9341, ILI9341_ROTATION);

    // ili9486 = qp_ili9486_make_spi_waveshare_device(_ILI9486_WIDTH, _ILI9486_HEIGHT, ILI9486_CS_PIN, SCREEN_SPI_DC_PIN, ILI9486_RST_PIN, SPI_DIV_9486, SPI_MODE);
    // qp_init(ili9486, ILI9486_ROTATION);

    ssd1680 = qp_ssd1680_make_spi_device(_SSD1680_WIDTH, _SSD1680_HEIGHT, SSD1680_CS_PIN, SSD1680_DC_PIN, SSD1680_RST_PIN, SPI_DIV, SPI_MODE, (void *)ssd1680_buffer);
    qp_init(ssd1680, SSD1680_ROTATION);

    // ----- Fill them black
    // qp_rect(il91874, 0, 0, IL91874_WIDTH, IL91874_HEIGHT, HSV_WHITE, true);
    // qp_drawimage(il91874, 0, 0, qp_xap_images[0]);
    // qp_flush(il91874);

    // qp_rect(ili9163, 0, 0, ILI9163_WIDTH, ILI9163_HEIGHT, HSV_BLACK, true);
    // qp_drawimage(ili9163, 0, 0, qp_xap_images[1]);

    // qp_rect(ili9341, 0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, HSV_BLACK, true);
    // qp_drawimage(ili9341, 0, 0, qp_xap_images[2]);

    // qp_rect(ili9486, 0, 0, ILI9486_WIDTH, ILI9486_HEIGHT, HSV_BLACK, true);
    // qp_drawimage(ili9486, 0, 0, qp_xap_images[3]);

    qp_rect(ssd1680, 0, 0, SSD1680_WIDTH, SSD1680_HEIGHT, HSV_BLACK, true);
    qp_flush(ssd1680);

    dprint("Quantum painter ready\n");
#endif // QUANTUM_PAINTER_ENABLE

#if defined (TOUCH_SCREEN_ENABLE)
    // Calibration isn't very precise
    static touch_driver_t ili9341_touch_driver = {
        .width = _ILI9341_WIDTH,
        .height = _ILI9341_HEIGHT,
        .measurements = 1,
        .scale_x = 0.07,
        .scale_y = -0.09,
        .offset_x = -26,
        .offset_y = 345,
        .rotation = ILI9341_ROTATION,
        .upside_down = false,
        .spi_config = {
            .chip_select_pin = ILI9341_TOUCH_CS_PIN,
            .divisor = SPI_DIV,
            .lsb_first = false,
            .mode = SPI_MODE,
            .irq_pin = ILI9341_TOUCH_IRQ_PIN,
            .x_cmd = 0xD0,
            .y_cmd = 0x90
        },
    };
    ili9341_touch = &ili9341_touch_driver;
    touch_spi_init(ili9341_touch);

    static touch_driver_t ili9486_touch_driver = {
        .width = _ILI9486_WIDTH,
        .height = _ILI9486_HEIGHT,
        .measurements = 1,
        .scale_x = 0.095,
        .scale_y = 0.12,
        .offset_x = -44,
        .offset_y = -23,
        .rotation = (ILI9486_ROTATION+2)%4,
        .upside_down = false,
        .spi_config = {
            .chip_select_pin = ILI9486_TOUCH_CS_PIN,
            .divisor = SPI_DIV,
            .lsb_first = false,
            .mode = SPI_MODE,
            .irq_pin = ILI9486_TOUCH_IRQ_PIN,
            .x_cmd = 0xD0,
            .y_cmd = 0x90
        },
    };
    ili9486_touch = &ili9486_touch_driver;
    touch_spi_init(ili9486_touch);

    dprint("Touch devices ready\n");
#endif // TOUCH_SCREEN_ENABLE

    dprint("\n---------- User code ----------\n");

    // =======
    // Call user code
    keyboard_post_init_user();

#if defined(DEFERRED_EXEC_ENABLE)
    return 0; //don't repeat the function
#endif // DEFERRED_EXEC_ENABLE
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) {
        return false;
    }

    return true;
}

#if defined(ONE_HAND_ENABLE)
#    if defined (TOUCH_SCREEN_ENABLE)
void screen_one_hand(touch_report_t touch_report) {
    if (!touch_report.pressed) {
        return;
    }

    int16_t x = touch_report.x - ILI9486_WIDTH/2;
    int16_t y = touch_report.y - ILI9486_HEIGHT/2;

    if (x > 30) {
        one_hand_movement = y > 0 ? DIRECTION_RIGHT : DIRECTION_LEFT;
    } else if (x < -30){
        one_hand_movement = y > 0 ? DIRECTION_UP : DIRECTION_DOWN;
    }
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    // Wait until it is initialized
    if (ili9486_touch == NULL){
        return false;
    }

    // Get touchscreen status
    touch_report_t touch_report = get_spi_touch_report(ili9486_touch);

    // Convert left-based to center-based coord
    int16_t x = touch_report.x - ILI9486_WIDTH/2;

    // Store previous state for comparations
    matrix_row_t previous = current_matrix[one_hand_row];

    // If screen is not pressed, neither will the key
    if (!touch_report.pressed) {
        current_matrix[one_hand_row] &= ~(1 << one_hand_col);
    }
    // If pressed and in zone, press the key
    else if (-30 < x && x < 30) {
        current_matrix[one_hand_row] |= 1 << one_hand_col;
    }

    return previous != current_matrix[one_hand_row];
}
#    endif // TOUCH_SCREEN_ENABLE
#endif // ONE_HAND_ENABLE
