#pragma once

#include_next <board.h>

#undef STM32_HSECLK
#define STM32_HSECLK 8000000U

//#define STM32_HSE_BYPASS
//#undef BOARD_OTG_NOVBUSSENS
//#define BOARD_OTG_NOVBUSSENS 1
