#ifndef CONFIG_REV2_H
#define CONFIG_REV2_H

#define DEVICE_VER      0x0002

#undef MATRIX_ROWS
#undef MATRIX_COLS

#undef DEBOUNCING_DELAY
#define DEBOUNCING_DELAY 10

//
#define MATRIX_ROWS 5
#define MATRIX_COLS 13
//                          0   1   2    3   4
#define MATRIX_ROW_PINS { B11, B0, B1,  A2, A3 }
//                         0    1   2   3   4   5   6   7   8    9  10  11   12
#define MATRIX_COL_PINS {A10, A15, B3, B4, B5, B9, B8, B7, B6, C15, A0, A7, B10 }
#endif // CONFIG_REV2_H
