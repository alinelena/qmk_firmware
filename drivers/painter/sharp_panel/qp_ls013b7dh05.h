// Copyright 2023 Pablo Martinez (@elpekenin) <elpekenin@elpekenin.dev>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "gpio.h"
#include "qp_internal.h"
#include "qp_sharp_helper.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quantum Painter LS013B7DH03 configurables (add to your keyboard's config.h)

#ifndef LS013B7DH05_NUM_DEVICES
#    define LS013B7DH05_NUM_DEVICES 1
#endif

// "translation" for compatibility with string'ified name
#define ls013b7dh05_NUM_DEVICES LS013B7DH05_NUM_DEVICES

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quantum Painter LS013B7DH03 device factory
#if defined(QUANTUM_PAINTER_LS013B7DH05_SPI_ENABLE)
SHARP_PROTOTYPE(ls013b7dh05, 144, 168)
#endif // defined(QUANTUM_PAINTER_LS013B7DH05_SPI_ENABLE)
