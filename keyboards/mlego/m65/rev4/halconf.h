// Copyright 2020-2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define HAL_USE_PWM TRUE

#define HAL_USE_SERIAL TRUE

#define SERIAL_USB_BUFFERS_SIZE 256

#define HAL_USE_I2C TRUE

#include_next <halconf.h>
