#ifndef CONFIG_REV1_H
#define CONFIG_REV1_H

#define DEVICE_VER      0x0001

#undef MATRIX_ROWS
#undef MATRIX_COLS
/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 13

/* key matrix pins */
#define MATRIX_ROW_PINS { B0, B1, B2, B3, B7, D0 }
#define MATRIX_COL_PINS { D7, B4, B5, B6, F7, F6, F5, F4, C7, C6, D3, D2, D1 }
//                         0   1   2   3   4   5   6   7   8    9  10  11  12

#endif
