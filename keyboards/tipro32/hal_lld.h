#pragma once

#include <stm32_registry.h>

#undef STM32_FLASH_NUMBER_OF_BANKS
#define STM32_FLASH_NUMBER_OF_BANKS 1

#undef STM32_FLASH_SECTORS_PER_BANK
#define STM32_FLASH_SECTORS_PER_BANK 64

#include_next "hal_lld.h"
