// Copyright 2023 Pablo Martinez (@elpekenin)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "qp_comms_spi.h"
#include "qp_eink_panel.h"
#include "qp_internal.h"
#include "qp_surface.h"
#include "spi_master.h"


bool sram_spi_start(painter_device_t device) {
    struct eink_panel_dc_reset_painter_device_t *driver       = (struct eink_panel_dc_reset_painter_device_t *)device;
    struct qp_comms_spi_config_t *               comms_config = (struct qp_comms_spi_config_t *)driver->base.comms_config;

    spi_init();

    // close SPI, in case it was init'ed before
    spi_stop();

    if (!spi_start(driver->ram_chip_select_pin, comms_config->lsb_first, comms_config->mode, comms_config->divisor)) {
        qp_dprintf("sram_spi_start: error, couldnt start SPI\n");
        return false;
    }

    // Set up CS as output high
    setPinOutput(driver->ram_chip_select_pin);
    writePinHigh(driver->ram_chip_select_pin);

    return true;
}

// write/read functions assume SPI is initialized & started already
uint8_t sram_read_byte(painter_device_t device, uint16_t address) {
    struct eink_panel_dc_reset_painter_device_t *driver  = (struct eink_panel_dc_reset_painter_device_t *)device;
    uint8_t                                      array[] = {driver->ram_opcodes.read_data, address >> 8, address & 0xFF};

    // set up read mode and address
    writePinLow(driver->ram_chip_select_pin);
    spi_transmit(array, ARRAY_SIZE(array));

    // read a byte
    uint8_t ret = spi_write(0x00);

    // reset CS
    writePinHigh(driver->ram_chip_select_pin);

    return ret;
}

void sram_write(painter_device_t device, uint16_t address, uint8_t *data, uint16_t length) {
    struct eink_panel_dc_reset_painter_device_t *driver  = (struct eink_panel_dc_reset_painter_device_t *)device;
    uint8_t                                      array[] = {driver->ram_opcodes.write_data, address >> 8, address & 0xFF};

    // set up write mode and address
    writePinLow(driver->ram_chip_select_pin);
    spi_transmit(array, ARRAY_SIZE(array));

    // send data
    spi_transmit(data, length);

    // reset CS
    writePinHigh(driver->ram_chip_select_pin);
}

void sram_spi_stop(painter_device_t device) {
    struct eink_panel_dc_reset_painter_device_t *driver = (struct eink_panel_dc_reset_painter_device_t *)device;

    writePinHigh(driver->ram_chip_select_pin);
    spi_stop();
}

void sram_init(painter_device_t device) {
    struct eink_panel_dc_reset_painter_device_t *driver  = (struct eink_panel_dc_reset_painter_device_t *)device;
    uint8_t                                      array[] = {driver->ram_opcodes.write_status, driver->ram_opcodes.sequential};

    // Start SPI
    if (!sram_spi_start(device)) {
        qp_dprintf("sram_init: fail\n");
        return;
    }

    // Init sequence
    writePinLow(driver->ram_chip_select_pin);
    for (int i=0; i<3; ++i) {
        spi_write(0xFF);
    }
    writePinHigh(driver->ram_chip_select_pin);

    // Setup sequential mode
    writePinLow(driver->ram_chip_select_pin);
    spi_transmit(array, ARRAY_SIZE(array));

    sram_spi_stop(device);
}

void sram_to_display(painter_device_t device, bool black) {
    struct eink_panel_dc_reset_painter_device_t *       driver       = (struct eink_panel_dc_reset_painter_device_t *)device;
    struct eink_panel_dc_reset_painter_driver_vtable_t *vtable       = (struct eink_panel_dc_reset_painter_driver_vtable_t *)driver->base.driver_vtable;
    struct qp_comms_spi_dc_reset_config_t *             comms_config = (struct qp_comms_spi_dc_reset_config_t *)driver->base.comms_config;
    uint32_t                                            n_bytes      = EINK_BW_BYTES_REQD(driver->base.panel_width, driver->base.panel_height);
    uint16_t                                            address      = black ? 0 : n_bytes;
    uint8_t                                             cmd          = black ? vtable->opcodes.send_black_data : vtable->opcodes.send_red_data;

    // Select SRAM and put in read mode
    writePinLow(driver->ram_chip_select_pin);
    uint8_t array[] = {driver->ram_opcodes.read_data, address >> 8, address & 0xFF};
    spi_transmit(array, ARRAY_SIZE(array));

    // Sending command to display triggers SRAM to send 1st byte over MISO
    writePinLow(comms_config->dc_pin);
    writePinLow(comms_config->spi_config.chip_select_pin);
    uint8_t byte = spi_write(cmd);
    writePinHigh(comms_config->spi_config.chip_select_pin);

    // Send
    writePinHigh(comms_config->dc_pin);
    for (uint32_t i=0; i < n_bytes; ++i) {
        writePinLow(comms_config->spi_config.chip_select_pin);

        // red data for a display without 3 color support, empty
        if (!black && !driver->has_3color)
            spi_write(driver->invert_red ? 0xFF : 0x00);

        // SRAM is on sequential mode, thus each time we send a byte
        // to the display, as SRAM's CS is also pulled low, it
        // will send us the next byte over MISO
        else
            byte = spi_write(byte);

        writePinHigh(comms_config->spi_config.chip_select_pin);
    }
    writePinHigh(driver->ram_chip_select_pin);
}

void sram_flush(painter_device_t device) {
    struct eink_panel_dc_reset_painter_device_t *       driver = (struct eink_panel_dc_reset_painter_device_t *)device;
    struct eink_panel_dc_reset_painter_driver_vtable_t *vtable = (struct eink_panel_dc_reset_painter_driver_vtable_t *)driver->base.driver_vtable;

    if (!sram_spi_start(device)) {
        qp_dprintf("sram_flush: fail\n");
        return;
    }

    // Stream black and red data
    sram_to_display(device, true);
    sram_to_display(device, false);

    writePinLow(driver->spi_dc_reset_config.spi_config.chip_select_pin);
    writePinLow(driver->spi_dc_reset_config.dc_pin);
    spi_write(vtable->opcodes.refresh);
    writePinHigh(driver->spi_dc_reset_config.spi_config.chip_select_pin);

    sram_spi_stop(device);
}

static inline uint32_t get_pixel(surface_painter_device_t *surface) {
    uint16_t x = 0;
    uint16_t y = 0;

    switch (surface->base.rotation) {
        case QP_ROTATION_0:
            x = surface->pixdata_x;
            y = surface->pixdata_y;
            break;

        case QP_ROTATION_90:
            x = surface->base.panel_height - surface->pixdata_y;
            y = surface->pixdata_x;
            break;

        case QP_ROTATION_180:
            x = surface->base.panel_width - surface->pixdata_x;
            y = surface->base.panel_height - surface->pixdata_y;
            break;

        case QP_ROTATION_270:
            x = surface->pixdata_y;
            y = surface->base.panel_width - surface->pixdata_x;
            break;
    }

    return x + y * surface->base.panel_width;
}

bool sram_pixdata(painter_device_t device, const void *pixel_data, uint32_t native_pixel_count) {
    struct eink_panel_dc_reset_painter_device_t *driver     = (struct eink_panel_dc_reset_painter_device_t *)device;
    struct surface_painter_device_t *            black      = (struct surface_painter_device_t *)driver->black_surface;
    uint32_t                                     red_offset = EINK_BW_BYTES_REQD(driver->base.panel_width, driver->base.panel_height);
    uint8_t *                                    pixels     = (uint8_t *)pixel_data;

    // mini-buffers to send pixel data
    uint8_t black_data;
    uint8_t red_data;

    // both displays will get drawn at the same position
    // set/get-ting the address on one of them is enough
    uint32_t pixel     = get_pixel(black);
    uint16_t byte      = pixel / 8;
    uint16_t last_byte = UINT16_MAX;
    uint8_t  bit       = pixel % 8;

    if (!sram_spi_start(device)) {
        qp_dprintf("sram_pixdata: fail\n");
        return false;
    }

    for (uint32_t i = 0; i < native_pixel_count; ++i) {
        // if we move to another byte, read its content to only modify the bits we want
        if (byte != last_byte) {
            black_data = sram_read_byte(device, byte);

            if (driver->has_3color) {
                red_data = sram_read_byte(device, red_offset + byte);
            }

            last_byte = byte;
        }

        // add pixel info to buffer(s)
        if (pixels[i] & 1)
            black_data |=  (1 << (7-bit));
        else
            black_data &= ~(1 << (7-bit));

        if (driver->has_3color) {
            if (pixels[i] & 2)
                red_data |=  (1 << (7-bit));
            else
                red_data &= ~(1 << (7-bit));
        }

        // update position with dummy value, used to compute address
        black->base.driver_vtable->pixdata(black, (const void *)0, 1);
        pixel = get_pixel(black);
        byte  = pixel / 8;
        bit   = pixel % 8;

        // next pixel will be on another byte, send this byte
        if (byte != last_byte) {
            sram_write(device, byte, &black_data, 1);
            if (driver->has_3color)
                sram_write(device, red_offset + byte, &red_data, 1);
        }
    }

    sram_spi_stop(device);

    return true;
}
