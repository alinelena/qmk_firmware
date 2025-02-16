// Copyright 2020-2025 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#define HAL_USE_ADC TRUE


#if defined(QUANTUM_PAINTER_ENABLE)

#define HAL_USE_SPI TRUE
#define SPI_USE_WAIT TRUE
#define SPI_SELECT_MODE SPI_SELECT_MODE_PAD

#endif

#ifdef RGB_MATRIX_ENABLE

#define HAL_USE_PWM TRUE

#endif

#include_next <halconf.h>
