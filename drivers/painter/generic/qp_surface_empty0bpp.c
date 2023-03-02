// Copyright 2022 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "color.h"
#include "qp_draw.h"
#include "qp_surface_internal.h"
#include "qp_comms_dummy.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Surface driver impl: empty0bpp

// Stream pixel data to the current write position in GRAM
static bool qp_surface_pixdata_empty0bpp(painter_device_t device, const void *pixel_data, uint32_t native_pixel_count) {
    // Just increment position.
    painter_driver_t *        driver  = (painter_driver_t *)device;
    surface_painter_device_t *surface = (surface_painter_device_t *)driver;
    qp_surface_increment_pixdata_location(&surface->viewport);
    return true;
}

// Pixel colour conversion
static bool qp_surface_palette_convert_empty0bpp(painter_device_t device, int16_t palette_size, qp_pixel_t *palette) {
    // No-op.
    return true;
}

// Append pixels to the target location, keyed by the pixel index
static bool qp_surface_append_pixels_empty0bpp(painter_device_t device, uint8_t *target_buffer, qp_pixel_t *palette, uint32_t pixel_offset, uint32_t pixel_count, uint8_t *palette_indices) {
    // No-op.
    return true;
}

static bool qp_surface_append_pidata_empty0bpp(painter_device_t device, uint8_t *target_buffer, uint32_t pixdata_offset, uint8_t pixdata_byte) {
    // No-op.
    return false;
}

static bool empty0bpp_target_pixdata_transfer(painter_driver_t *surface_driver, painter_driver_t *target_driver, uint16_t x, uint16_t y, bool entire_surface) {
    // No-op.
    return true;
}

const surface_painter_driver_vtable_t empty0bpp_surface_driver_vtable = {
    .base =
        {
            .init            = qp_surface_init,
            .power           = qp_surface_power,
            .clear           = qp_surface_clear,
            .flush           = qp_surface_flush,
            .pixdata         = qp_surface_pixdata_empty0bpp,
            .viewport        = qp_surface_viewport,
            .palette_convert = qp_surface_palette_convert_empty0bpp,
            .append_pixels   = qp_surface_append_pixels_empty0bpp,
            .append_pixdata  = qp_surface_append_pidata_empty0bpp
        },
    .target_pixdata_transfer = empty0bpp_target_pixdata_transfer,
};

SURFACE_FACTORY_FUNCTION_IMPL(qp_make_empty0bpp_surface_internal, empty0bpp_surface_driver_vtable, 0);

painter_device_t qp_make_empty0bpp_surface(uint16_t panel_width, uint16_t panel_height) {
    return qp_make_empty0bpp_surface_internal(panel_width, panel_height, NULL);
}
