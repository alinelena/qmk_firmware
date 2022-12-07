// Copyright 2022 alin m elena (@alinelena)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

#include "wait.h"
#include "spi_master.h"

static const uint16_t col_values[MATRIX_COLS] = SR_COLS;

static inline uint8_t read_rows(void) {
  return ( (readPin(ROW_2) << 1)
         | (readPin(ROW_1) ));
}

static inline void shift_out(uint16_t value) {

  uint8_t message[MATRIX_COLS]  = {(value >> 8) & 0xFF ,(uint8_t)(value & 0xFF) };
  spi_start(SPI_LATCH_PIN, true, 1, 4);
  spi_transmit(message,MATRIX_COLS);
  spi_stop();

  wait_us(1);
}

static inline void select_col(uint8_t col) {
    shift_out(col_values[col]);
}

void matrix_init_custom(void) {
  setPinInputLow(ROW_1);
  setPinInputLow(ROW_2);

  spi_init();

  setPinOutput(SPI_LATCH_PIN);

  wait_ms(50);
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;

    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        select_col(col);
        wait_us(3);
        uint8_t rows = read_rows();
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            bool prev_bit = (current_matrix[row] & (matrix_row_t)(1 << col))? 1 : 0;
            bool curr_bit = (rows & (1 << row))? 1 : 0;
            if (prev_bit != curr_bit) {
                current_matrix[row] = current_matrix[row] ^ (1 << col);
                changed = true;
            }
        }
    }

    return changed;
}

