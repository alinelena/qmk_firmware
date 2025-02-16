/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#include "scanfunctions.h"
#include "multiplexer.h"
#include "lut.h"

void get_sensor_offsets(void) {
    const uint16_t rest_adc_value = distance_to_adc(0);
    matrix_scan();
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            analog_key_t *key = &keys[row][col];
            key->offset       = rest_adc_value - key->raw;
        }
    }
}

void update_extremum(analog_key_t *key) {
    key->extremum = key->value;
}

void register_key(matrix_row_t *current_row, uint8_t current_col) {
    *current_row |= (1 << current_col);
}

void deregister_key(matrix_row_t *current_row, uint8_t current_col) {
    *current_row &= ~(1 << current_col);
}

void matrix_read_cols_static_actuation(matrix_row_t *current_row, uint8_t current_col, analog_key_t *key) {
    if (*current_row & (1 << current_col)) {
        if (key->value <= MAX(g_config.actuation_point - g_config.release_hysteresis, 0)) {
            deregister_key(current_row, current_col);
        }
    } else {
        if (key->value >= MIN(g_config.actuation_point + g_config.press_hysteresis, CALIBRATION_RANGE)) {
            register_key(current_row, current_col);
        }
    }
}

/* Extremum is the highest or lowest value of the key reached
this functionality changes based on whether the key is pressed or not,
assuming your sensor value decreases when the key is pressed,
if the key is pressed, the extremum is the lowest value reached,
if the key is not pressed, the extremum is the highest value reached. */

void matrix_read_cols_dynamic_actuation(matrix_row_t *current_row, uint8_t current_col, analog_key_t *key) {
    if (key->value > g_config.actuation_point) {
        /* In DA zone? */
        if (*current_row & (1 << current_col)) {
            /* Key is pressed
            Is key still moving down? */
            if (key->value > key->extremum) {
                update_extremum(key);
            } else if (key->value <= MAX(key->extremum - g_config.release_sensitivity, 0)) {
                /* Has key moved up enough to be released? */
                deregister_key(current_row, current_col);
                update_extremum(key);
            }
        } else {
            /* Key is not pressed
            Is the key still moving up? */
            if (key->value < key->extremum) {
                update_extremum(key);
            } else if (key->value >= MIN(key->extremum + g_config.press_sensitivity, CALIBRATION_RANGE)) {
                /* Has key moved down enough to be pressed? */
                register_key(current_row, current_col);
                update_extremum(key);
            }
        }
    } else {
        /* Out of DA zone
        Always deregister key */
        deregister_key(current_row, current_col);
        if (key->value > key->extremum) {
            update_extremum(key);
        }
    }
}

void matrix_read_cols_continuous_dynamic_actuation(matrix_row_t *current_row, uint8_t current_col, analog_key_t *key) {
    if (key->continuous_dynamic_actuation) {
        if (*current_row & (1 << current_col)) {
            /* Key is pressed
            Is key still moving down? */
            if (key->value > key->extremum) {
                update_extremum(key);

            } else if (key->value <= MAX(key->extremum - g_config.release_sensitivity, 0)) {
                /* Has key moved up enough to be released? */
                deregister_key(current_row, current_col);
                update_extremum(key);
            }
        } else {
            /* Key is not pressed
            Is the key still moving up? */
            if (key->value < key->extremum) {
                update_extremum(key);
            } else if (key->value >= MIN(key->extremum + g_config.press_sensitivity, CALIBRATION_RANGE)) {
                /* Has key moved down enough to be pressed? */
                register_key(current_row, current_col);
                update_extremum(key);
            }
        }
        if (key->value == 0) {
            deregister_key(current_row, current_col);
            update_extremum(key);
            key->continuous_dynamic_actuation = false;
        }
    } else if (key->value > g_config.actuation_point) {
        register_key(current_row, current_col);
        update_extremum(key);
        key->continuous_dynamic_actuation = true;
    }
}
