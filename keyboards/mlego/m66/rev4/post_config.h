// Copyright 2022-2023 Pablo Martinez (@elpekenin)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define SSD1680_ROTATION 1
#define _SSD1680_WIDTH 128
#define _SSD1680_HEIGHT 250

#if !(SSD1680_ROTATION == 0 || SSD1680_ROTATION == 1 || SSD1680_ROTATION == 2 || SSD1680_ROTATION == 3)
#    error SSD1680_ROTATION has to be within 0-3 (both included)
#endif

//#if (SSD1680_ROTATION % 2 == 0)
#    define SSD1680_WIDTH  _SSD1680_WIDTH
#    define SSD1680_HEIGHT _SSD1680_HEIGHT
//#else
//#    define SSD1680_WIDTH  _SSD1680_HEIGHT
//#    define SSD1680_HEIGHT _SSD1680_WIDTH
//#endif

// =======
// - Default values
#if !defined(INIT_DELAY)
#    define INIT_DELAY 5
#endif // INIT_DELAY
