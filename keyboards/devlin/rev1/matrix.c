// Copyright 2020-2023 alin m elena (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

#include "wait.h"

// #include "spi_master.h"

#if defined(CONSOLE_ENABLE)
#    include "print.h"
#endif

static const uint16_t col_values[MATRIX_COLS]  = COLS;
static const pin_t    rows_values[MATRIX_ROWS] = ROWS;
static const int      msize                    = MATRIX_ROWS * sizeof(matrix_row_t);
static matrix_row_t   prev_matrix[MATRIX_ROWS];

static inline void shift_out_byte(const pin_t data_pin, const pin_t clock_pin, bool bit_order, uint8_t value) {
    for (uint8_t i = 0; i < 8; ++i) {
        // LSBFIRST true
        if (bit_order) {
            writePin(data_pin, !!(value & (1 << (7 - i))));
        } else {
            writePin(data_pin, !!(value & (1 << i)));
        }
        writePinHigh(clock_pin);
        writePinLow(clock_pin);
    }
}

static inline uint8_t shift_in_byte(const pin_t data_pin, const pin_t clock_pin, bool bit_order) {
    uint8_t rows = 0;
    for (uint8_t i = 0; i < 8; ++i) {
        // LSBFIRST true
        if (bit_order) {
            rows |= readPin(data_pin) << (7 - i);
        } else {
            rows |= readPin(data_pin) << i;
        }
        writePinHigh(clock_pin);
        writePinLow(clock_pin);
    }
    return rows;
}

static inline uint8_t read_rows(void) {
    writePinLow(SPI_PL_PIN);

    writePinLow(SPI_MATRIX_CS_PIN);
    writePinHigh(SPI_PL_PIN);

    wait_us(1);

    return shift_in_byte(SPI_MISO_PIN, SPI_SCK_PIN, SPI_lsbFirst);
}

static inline void shift_out(uint16_t value) {
    uint8_t message[N595] = {(value >> 8) & 0xFF, (uint8_t)(value & 0xFF)};

    for (size_t i = 0; i < N595; ++i) {
        shift_out_byte(SPI_MOSI_PIN, SPI_SCK_PIN, SPI_lsbFirst, message[i]);
    }

    writePinHigh(SPI_MATRIX_CS_PIN);
    matrix_output_select_delay();
}

void matrix_init_custom(void) {
    matrix_io_delay();
    // spi_init();
    matrix_io_delay();
    // sipo
    //
    setPinOutput(SPI_MATRIX_CS_PIN);
    writePinLow(SPI_MATRIX_CS_PIN);
    setPinOutput(SPI_SCK_PIN);
    setPinOutput(SPI_MOSI_PIN);

    setPinInput(SPI_MISO_PIN);

    setPinOutput(SPI_PL_PIN);
    writePinHigh(SPI_PL_PIN);

    matrix_io_delay();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    uint8_t crow;
    memset(current_matrix, 0, msize);

    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        shift_out(col_values[col]);

        uint8_t rows = read_rows();
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            crow = rows_values[row];
            current_matrix[row] |= (((rows & (1 << crow)) ? 1 : 0) << col);
        }
    }
    bool changed = (memcmp(current_matrix, prev_matrix, msize) != 0);
    memcpy(prev_matrix, current_matrix, msize);
    return changed;
}
