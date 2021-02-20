#ifndef CONFIG_REV3_H
#define CONFIG_REV3_H

#define DEVICE_VER      0x0003

#undef MATRIX_ROWS
#undef MATRIX_COLS

#undef DEBOUNCING_DELAY
#define DEBOUNCING_DELAY 10

//
#define MATRIX_ROWS 5
#define MATRIX_COLS 13
//                          0   1   2    3   4
#define MATRIX_ROW_PINS { A7, B0, A5,  A2, A1 }
//                         0    1   2   3   4   5   6   7   8    9  10  11   12
#define MATRIX_COL_PINS {A10, A15, B3, B4, B5, B9, B8, B7, B6, C15, A0, A6, B1 }

//#define MATRIX_ROW_PINS { A6, A5, A4, A3, A2 }
//#define MATRIX_COL_PINS { B10, B1, B0, B15, A8, B3, B4, B5, B6, B7, B8, B9 ,A0}

#define UNUSED_PINS

#endif // CONFIG_REV3_H
