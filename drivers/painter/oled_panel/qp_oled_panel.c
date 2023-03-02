// Copyright 2023 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "color.h"
#include "qp_internal.h"
#include "qp_comms.h"
#include "qp_draw.h"
#include "qp_oled_panel.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quantum Painter API implementations

// Power control
bool qp_oled_panel_power(painter_device_t device, bool power_on) {
    struct painter_driver_t *                  driver = (struct painter_driver_t *)device;
    struct oled_panel_painter_driver_vtable_t *vtable = (struct oled_panel_painter_driver_vtable_t *)driver->driver_vtable;
    qp_comms_command(device, power_on ? vtable->opcodes.display_on : vtable->opcodes.display_off);
    return true;
}

// Screen clear
bool qp_oled_panel_clear(painter_device_t device) {
    struct painter_driver_t *driver = (struct painter_driver_t *)device;
    driver->driver_vtable->init(device, driver->rotation); // Re-init the display
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Surface passthru
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool qp_oled_panel_passthru_pixdata(painter_device_t device, const void *pixel_data, uint32_t native_pixel_count) {
    oled_panel_painter_device_t *driver = (oled_panel_painter_device_t *)device;
    return driver->surface.base.validate_ok && driver->surface.base.driver_vtable->pixdata(&driver->surface.base, pixel_data, native_pixel_count);
}

bool qp_oled_panel_passthru_viewport(painter_device_t device, uint16_t left, uint16_t top, uint16_t right, uint16_t bottom) {
    oled_panel_painter_device_t *driver = (oled_panel_painter_device_t *)device;
    return driver->surface.base.validate_ok && driver->surface.base.driver_vtable->viewport(&driver->surface.base, left, top, right, bottom);
}

bool qp_oled_panel_passthru_palette_convert(painter_device_t device, int16_t palette_size, qp_pixel_t *palette) {
    oled_panel_painter_device_t *driver = (oled_panel_painter_device_t *)device;
    return driver->surface.base.validate_ok && driver->surface.base.driver_vtable->palette_convert(&driver->surface.base, palette_size, palette);
}

bool qp_oled_panel_passthru_append_pixels(painter_device_t device, uint8_t *target_buffer, qp_pixel_t *palette, uint32_t pixel_offset, uint32_t pixel_count, uint8_t *palette_indices) {
    oled_panel_painter_device_t *driver = (oled_panel_painter_device_t *)device;
    return driver->surface.base.validate_ok && driver->surface.base.driver_vtable->append_pixels(&driver->surface.base, target_buffer, palette, pixel_offset, pixel_count, palette_indices);
}

bool qp_oled_panel_passthru_append_pixdata(painter_device_t device, uint8_t *target_buffer, uint32_t pixdata_offset, uint8_t pixdata_byte) {
    oled_panel_painter_device_t *driver = (oled_panel_painter_device_t *)device;
    return driver->surface.base.validate_ok && driver->surface.base.driver_vtable->append_pixdata(&driver->surface.base, target_buffer, pixdata_offset, pixdata_byte);
}
