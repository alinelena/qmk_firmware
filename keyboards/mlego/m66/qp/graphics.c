// Copyright 2023 Pablo Martinez (@elpekenin) <elpekenin@elpekenin.dev>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "graphics.h"
#include "qp.h"

painter_device_t qp_displays[QUANTUM_PAINTER_NUM_DISPLAYS] = {}; // Has to be filled by the user
painter_font_handle_t qp_fonts[QUANTUM_PAINTER_NUM_FONTS] = {};
painter_image_handle_t qp_images[QUANTUM_PAINTER_NUM_IMAGES] = {};

// Fonts, in alphabetical order to easier checking
#include "fira_code.qff.h"
#include "thintel15.qff.h"

// Images, in alphabetical order to easier checking
#include "caps-lock.qgf.h"
#include "elephant.qgf.h"
#include "fira_code.qff.h"
#include "five.qgf.h"
#include "four.qgf.h"
#include "num-lock.qgf.h"
#include "one.qgf.h"
#include "qmk_icon_48.qgf.h"
#include "scroll-lock.qgf.h"
#include "seven.qgf.h"
#include "six.qgf.h"
#include "thintel15.qff.h"
#include "three.qgf.h"
#include "two.qgf.h"
#include "zero.qgf.h"
#include "led.qgf.h"


// =======
// Load resources
void _load_display(painter_device_t display) {
    static uint8_t i = 0;

    if (i >= QUANTUM_PAINTER_NUM_DISPLAYS) {
        dprintf("' failed, out of bounds defined by QUANTUM_PAINTER_NUM_DISPLAYS\n");
        return;
    }

    dprintf("' at position [%d]\n", i);
    qp_displays[i++] = display;
}

void _load_font(const uint8_t *font) {
    static uint8_t i = 0;

    if (i >= QUANTUM_PAINTER_NUM_FONTS) {
        dprintf("' failed, out of bounds defined by QUANTUM_PAINTER_NUM_FONTS\n");
        return;
    }

    dprintf("' at position [%d]\n", i);
    painter_font_handle_t dummy = qp_load_font_mem(font);
    qp_fonts[i++] = dummy;
}

void _load_image(const uint8_t *img) {
    static uint8_t i = 0;

    if (i >= QUANTUM_PAINTER_NUM_IMAGES) {
        dprintf("' failed, out of bounds defined by QUANTUM_PAINTER_NUM_IMAGES\n");
        return;
    }

    dprintf("' at position [%d]\n", i);
    painter_image_handle_t dummy = qp_load_image_mem(img);
    qp_images[i++] = dummy;
}

void load_qp_resources(void) {
    load_font(font_fira_code);
    load_font(font_thintel15);

    load_image(gfx_qmk_icon_48);
    load_image(gfx_elephant);
    load_image(gfx_caps_lock);
    load_image(gfx_num_lock);
    load_image(gfx_scroll_lock);
    load_image(gfx_zero);
    load_image(gfx_one);
    load_image(gfx_two);
    load_image(gfx_three);
    load_image(gfx_four);
    load_image(gfx_five);
    load_image(gfx_six);
    load_image(gfx_seven);
    load_image(gfx_led);
}

