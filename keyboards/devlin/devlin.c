// Copyright 2020-2023 alin m elena (@alinelena, @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

void keyboard_post_init_kb(void) {
#ifdef CONSOLE_ENABLE
    debug_enable   = true;
    debug_matrix   = true;
    debug_keyboard = true;
#endif
    keyboard_post_init_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
    return process_record_user(keycode, record);
}
