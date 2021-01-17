

#ifndef CONFIG_REV3_H
#define CONFIG_REV3_H

#define DEVICE_VER      0x0002

#undef MATRIX_ROWS
#undef MATRIX_COLS

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 13


#define DIODE_DIRECTION COL2ROW
//                          0    1    2    3   4   5
//pcb
#define MATRIX_ROW_PINS { B11, B1, B0,  A7, A2, A0 }


//                         0   1   2   3   4   5    6    7   8   9  10   11   12
#define MATRIX_COL_PINS { A9, A10, A15, B3, B4, B5, B6, B7, B8, B9, C15, A3, B10 }
//wired

#define DEBOUNCE    5
#define LOCKING_SUPPORT_ENABLE
#define LOCKING_RESYNC_ENABLE
#endif
