/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "quantum.h"
#include "custom_analog.h"
#include "lut.h"
#include "multiplexer.h"
#include "scanfunctions.h"
#include <ch.h>
#include <hal.h>
#include "gpio.h"

// External definitions
extern ADCManager  adcManager;
extern const mux_t mux_index[MUXES][MUX_CHANNELS];

analog_key_t    keys[MATRIX_ROWS][MATRIX_COLS] = {0};
static uint16_t pressedAdcValue                = 0;
static uint16_t restAdcValue                   = 0;

void matrix_init_custom(void) {
    gpio_set_pin_input_high(ENCODER_BUTTON_PIN);
    gpio_set_pin_input_high(ESC_PIN);
    generate_lut();
    pressedAdcValue = distance_to_adc(255);
    restAdcValue    = distance_to_adc(0);
    multiplexer_init();
    initADCGroups(&adcManager);
    wait_ms(100);
    get_sensor_offsets();
}

matrix_row_t previous_matrix[MATRIX_ROWS];

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    memcpy(previous_matrix, current_matrix, sizeof(previous_matrix));

    for (uint8_t channel = 0; channel < MUX_CHANNELS; ++channel) {
        uint8_t channel_greycoded = (channel >> 1) ^ channel;
        select_mux(channel_greycoded);
        adcStartAllConversions(&adcManager);

        for (uint8_t mux = 0; mux < MUXES; ++mux) {
            const mux_t *mux_idx = &mux_index[mux][channel_greycoded];

            if (mux_idx->row == 255 && mux_idx->col == 255) continue; // NC mux pin

            analog_key_t *key = &keys[mux_idx->row][mux_idx->col];
            key->raw          = getADCSample(&adcManager, mux);
            key->value        = lut[key->raw + key->offset];

            switch (g_config.mode) {
                case dynamic_actuation:
                    matrix_read_cols_dynamic_actuation(&current_matrix[mux_idx->row], mux_idx->col, key);
                    break;
                case continuous_dynamic_actuation:
                    matrix_read_cols_continuous_dynamic_actuation(&current_matrix[mux_idx->row], mux_idx->col, key);
                    break;
                case static_actuation:
                    matrix_read_cols_static_actuation(&current_matrix[mux_idx->row], mux_idx->col, key);
                    break;
                case flashing:
                default:
                    bootloader_jump();
                    break;
            }
        }
    }
    // read esc
    bool esc_button_pressed = gpio_read_pin(ESC_PIN);
    if (current_matrix[ESC_ROW] & (1 << ESC_COL)) {
        if (!esc_button_pressed) {
            deregister_key(&current_matrix[ESC_ROW], ESC_COL);
        }
    } else {
        if (esc_button_pressed) {
            register_key(&current_matrix[ESC_ROW], ESC_COL);
        }
    }
#ifdef ENCODER_ENABLE
    bool encoder_button_pressed = gpio_read_pin(ENCODER_BUTTON_PIN);
    if (current_matrix[ENCODER_ROW] & (1 << ENCODER_COL)) {
        if (!encoder_button_pressed) {
            deregister_key(&current_matrix[ENCODER_ROW], ENCODER_COL);
        }
    } else {
        if (encoder_button_pressed) {
            register_key(&current_matrix[ENCODER_ROW], ENCODER_COL);
        }
    }
#endif
    return memcmp(previous_matrix, current_matrix, sizeof(previous_matrix)) != 0;
}
