#pragma once
#include "config_common.h"


/* USB Device descriptor parameter */
#define VENDOR_ID       0xB16B
#define PRODUCT_ID      0x00B5
#define DEVICE_VER      0x0001
#define MANUFACTURER Ramon Imbao
#define PRODUCT fAuxLICE
#define DESCRIPTION A handwired 3D-printed ergonomic keyboard


/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 14
#define DIODE_DIRECTION COL2ROW

#define MATRIX_ROW_PINS { B11, B10, B1, B0, A7 }
#define MATRIX_COL_PINS { B6, B7, B8, B9, C13, C14, C15, A0, A1, A2, A3, A4, A5, A6 }


/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE
