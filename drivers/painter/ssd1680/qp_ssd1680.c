// Copyright 2023 Pablo Martinez (@elpekenin)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "qp_internal.h"
#include "qp_comms.h"
#include "qp_ssd1680.h"
#include "qp_ssd1680_opcodes.h"
#include "qp_eink_panel.h"
#include "qp_surface.h"
#include "qp_surface_internal.h"

#ifdef QUANTUM_PAINTER_SSD1680_SPI_ENABLE
#    include <qp_comms_spi.h>
#endif // QUANTUM_PAINTER_SSD1680_SPI_ENABLE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common

// Driver storage
eink_panel_dc_reset_painter_device_t ssd1680_drivers[SSD1680_NUM_DEVICES] = {0};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialization

bool qp_ssd1680_init(painter_device_t device, painter_rotation_t rotation) {
    eink_panel_dc_reset_painter_device_t *driver = (eink_panel_dc_reset_painter_device_t *)device;

    uint16_t x = driver->base.panel_width;
    uint16_t y = driver->base.panel_height;

    // maybe we need to adjust it when rotated?
    // if (rotation == QP_ROTATION_90 || rotation == QP_ROTATION_270) {
    //     uint16_t temp = x;
    //     x = y;
    //     y = temp;
    // }

    // x is shifted 3 places to fit in 8 bits (??)
    uint8_t x_lsb = ((x - 1) >> 3) & 0xFF;
    uint8_t y_msb = ((y - 1) >> 8) & 0xFF;
    uint8_t y_lsb = ((y - 1) >> 0) & 0xFF;

    // clang-format off
    const uint8_t ssd1680_init_sequence[] = {
        // Command,                       Delay, N, Data[N]
        SSD1680_SOFT_RESET,                 250, 0,
        SSD1680_DATA_ENTRY_MODE,              0, 1, 0x03,
        SSD1680_RAM_X_SIZE,                   0, 2, 0x00, x_lsb,
        SSD1680_RAM_Y_SIZE,                   0, 4, 0x00, 0x00, y_msb, y_lsb,
        SSD1680_BORDER_CONTROL,               0, 1, 0x80,
        SSD1680_TEMP_SENSOR,                  0, 1, 0x80,
    };
    // clang-format on
    qp_comms_bulk_command_sequence(device, ssd1680_init_sequence, sizeof(ssd1680_init_sequence));
    driver->base.rotation = rotation;

    // clear gets the buffers correctly set to 0/1
    return driver->base.driver_vtable->clear(driver);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Screen-specific patched functions
static inline void send_dirty_area(painter_device_t eink, surface_painter_device_t *surface) {
    surface_dirty_data_t dirty    = surface->dirty;
    uint16_t             l        = dirty.l;
    uint16_t             t        = dirty.t;
    uint16_t             r        = dirty.r;
    uint16_t             b        = dirty.b;
    uint16_t             w        = surface->base.panel_width;
    // data being sent on each row
    // FIXME: cases where n_pixels is not a multiple of 8
    uint32_t             n_pixels = (r - l);
    uint32_t             n_bytes  = n_pixels / 8;

    // send data by indexing the correct places in the buffer
    for (uint16_t row = t; row < b; ++row) {
        uint32_t offset  = (row * w) + l;
        qp_comms_send(eink, surface->buffer + offset, n_bytes);
    }
}

bool ssd1680_partial_flush(painter_device_t device) {
    eink_panel_dc_reset_painter_device_t *       driver  = (eink_panel_dc_reset_painter_device_t *)device;
    eink_panel_dc_reset_painter_driver_vtable_t *vtable  = (eink_panel_dc_reset_painter_driver_vtable_t *)driver->base.driver_vtable;
    surface_painter_device_t *                   black   = (surface_painter_device_t *)driver->black_surface;
    surface_painter_device_t *                   red     = (surface_painter_device_t *)driver->red_surface;

    if (!(black->dirty.is_dirty || red->dirty.is_dirty)) {
        qp_dprintf("ssd1680_partial_flush: done (no changes to be sent)\n");
        return true;
    }

    if (!driver->can_flush) {
        qp_dprintf("ssd1680_partial_flush: fail (can_flush == false)\n");
        return false;
    }

    /* ----------------------------------------------------------------------------------------------------------------
     * Code from: <https://discord.com/channels/440868230475677696/1059874295297347694/1081866413758742538>
     *
     * No idea whether it works or if it contains messages that aren't needed
     *
     * Edited logic to also send red buffer
     */
    // send data
    qp_comms_command(device, vtable->opcodes.send_black_data); // is this what `start_data_` does??
    send_dirty_area(device, black);
    qp_comms_command(device, vtable->opcodes.send_red_data);
    send_dirty_area(device, red);
    // qp_comms_command(device, vtable->opcodes.refresh); // WTF does `end_data_` do??

    // commit as partial
    qp_comms_command(device, (uint8_t)SSD1680_UPDATE_MODE);
    qp_comms_send(device, (const void *)0xCF, 1);
    qp_comms_command(device, vtable->opcodes.refresh);
    // while (readPin(BUSY)) {}; // is this what `wait_until_idle_` does??

    // send again
    wait_ms(300); // can be smaller probably, we've already waited for busy pin
    qp_comms_command(device, vtable->opcodes.send_black_data); // is this what `start_data_` does??
    qp_comms_command(device, vtable->opcodes.send_black_data); // is this what `start_data_` does??
    send_dirty_area(device, black);
    qp_comms_command(device, vtable->opcodes.send_red_data);
    send_dirty_area(device, red);
    // qp_comms_command(device, vtable->opcodes.refresh); // WTF does `end_data_` do??
    wait_ms(300); // i dont think we need to wait after sending data
    // ----------------------------------------------------------------------------------------------------------------

    // this will prevent you from drawing again, you may want to edit the "constructor" with a smaller value
    qp_eink_update_can_flush(device);

    // we've send data, dirty has to be reset
    qp_flush(driver->black_surface);
    qp_flush(driver->red_surface); // just in case

    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver vtable

const eink_panel_dc_reset_painter_driver_vtable_t ssd1680_driver_vtable = {
    .base =
        {
            .init            = qp_ssd1680_init,
            .power           = qp_eink_panel_power,
            .clear           = qp_eink_panel_clear,
            .flush           = ssd1680_partial_flush,
            .pixdata         = qp_eink_panel_pixdata,
            .viewport        = qp_eink_panel_viewport,
            .palette_convert = qp_eink_panel_palette_convert,
            .append_pixels   = qp_eink_panel_append_pixels,
            .append_pixdata  = qp_eink_panel_append_pixdata
        },
    .opcodes =
        {
            .display_on  = SSD1680_NOP, // Couldnt find a turn-on command
            .display_off = SSD1680_NOP, // There is a cmd to go into sleep mode, but requires HW reset in order to wake up
            .send_black_data = SSD1680_SEND_BLACK,
            .send_red_data = SSD1680_SEND_RED,
            .refresh = SSD1680_DISPLAY_REFRESH,
        }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SPI

#ifdef QUANTUM_PAINTER_SSD1680_SPI_ENABLE

// Factory function for creating a handle to the SSD1680 device
painter_device_t qp_ssd1680_make_spi_device(uint16_t panel_width, uint16_t panel_height, pin_t chip_select_pin, pin_t dc_pin, pin_t reset_pin, uint16_t spi_divisor, int spi_mode, void *ptr) {
    for (uint32_t i = 0; i < SSD1680_NUM_DEVICES; ++i) {
        eink_panel_dc_reset_painter_device_t *driver = &ssd1680_drivers[i];
        if (!driver->base.driver_vtable) {
            driver->base.driver_vtable = (const painter_driver_vtable_t *)&ssd1680_driver_vtable;
            driver->base.comms_vtable  = (const painter_comms_vtable_t *)&spi_comms_with_dc_single_byte_vtable;

            driver->base.native_bits_per_pixel = 1;
            driver->base.panel_width           = panel_width;
            driver->base.panel_height          = panel_height;
            driver->base.rotation              = QP_ROTATION_0;
            driver->base.offset_x              = 0;
            driver->base.offset_y              = 0;

            driver->black_surface = qp_make_mono1bpp_surface(panel_width, panel_height, ptr);
            driver->red_surface   = qp_make_mono1bpp_surface(panel_width, panel_height, ptr + SURFACE_REQUIRED_BUFFER_BYTE_SIZE(panel_width, panel_height, 1));

            // set can_flush = false on start and schedule its reset
            driver->timeout   = 2 * 60 * 1000; // 2 minutes as seen on WeAct
            qp_eink_update_can_flush((painter_device_t *)driver);

            driver->invert_mask = 0b10;

            // SPI and other pin configuration
            driver->base.comms_config                              = &driver->spi_dc_reset_config;
            driver->spi_dc_reset_config.spi_config.chip_select_pin = chip_select_pin;
            driver->spi_dc_reset_config.spi_config.divisor         = spi_divisor;
            driver->spi_dc_reset_config.spi_config.lsb_first       = false;
            driver->spi_dc_reset_config.spi_config.mode            = spi_mode;
            driver->spi_dc_reset_config.dc_pin                     = dc_pin;
            driver->spi_dc_reset_config.reset_pin                  = reset_pin;

            return (painter_device_t)driver;
        }
    }
    return NULL;
}
#endif // QUANTUM_PAINTER_SSD1680_SPI_ENABLE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
