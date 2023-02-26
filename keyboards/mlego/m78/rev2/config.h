// Copyright 2019-2023 alin elena (@alinelena @drFaustroll)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* Double tap reset button to enter bootloader */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

#if defined(CONSOLE_ENABLE)
#define DEBUG_MATRIX_SCAN_RATE
#endif
