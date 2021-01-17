#ifndef CONFIG_REV3_H
#define CONFIG_REV3_H

#define DEVICE_VER      0x0003
#undef MANUFACTURER
#define MANUFACTURER    alinb

#undef MATRIX_ROWS
#undef MATRIX_COLS

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 13

/*
3V3 GNG  5V  B9  B8  B7  B6  B5  B4  B3 A15 A12 A11 A10  A9  A8 B15 B14 B13 B12
                                                                               USB connector
VBT C13 C14 C15  A0  A1  A2  A3  A4  A5  A6  A7  B0  B1 B10 B11 RST 3V3 GND GND

B6 LED
B7 LED
A1 avoid
A4 avoid
A6 avoid
A11 avoid usb
A12 avoid usb
*/

//                          0    1    2    3   4   5
//wired
//#define MATRIX_ROW_PINS { B12, B13, B14, B15, A8, A9 }

//pcb
#define MATRIX_ROW_PINS { B11, B1, B0,  A7, A2, A0 }


//                         0   1   2   3   4   5    6    7   8   9  10   11   12
//wired
//#define MATRIX_COL_PINS { A0, A2, A3, A7, B0, B1, B10, B11, B5, B4, B3, A15, A10 }

//pcb
#define MATRIX_COL_PINS { A9, A10, A15, B3, B4, B5, B6, B7, B8, B9, C15, A3, B10 }
//
#endif // CONFIG_REV2_H
