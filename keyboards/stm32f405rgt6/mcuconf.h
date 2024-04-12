#pragma once

#include_next <mcuconf.h>

#undef STM32_PLLM_VALUE
#define STM32_PLLM_VALUE 8

#undef STM32_PLLI2SM_VALUE
#define STM32_PLLI2SM_VALUE 8

#undef STM32_PLLSAIM_VALUE
#define STM32_PLLSAIM_VALUE 8
