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

#define LUT_SIZE  153

#define PART_UPDATE_LUT   0x0 , 0x40, 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
  0x80, 0x80, 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
  0x40, 0x40, 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
  0x0 , 0x80, 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
  0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
  0x0A, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x2, 0x1, 0x0, 0x0, 0x0, 0x0, \
  0x0 , 0x0 , 0x1 , 0x0 , 0x0 , 0x0 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
  0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
  0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
  0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
  0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
  0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x0, 0x0, 0x0


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common

// Driver storage
eink_panel_dc_reset_painter_device_t ssd1680_drivers[SSD1680_NUM_DEVICES] = {0};

static inline void wait_for_busy(pin_t pin){
  while(readPin(pin)) {};
}

static inline void hw_reset(pin_t pin) {
  writePinHigh(pin);
  wait_ms(200);
  writePinLow(pin);
  wait_ms(200);
  writePinHigh(pin);
  wait_ms(200);

}

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
/*
    const uint8_t ssd1680_init_sequence[] = {
        // Command,                       Delay, N, Data[0],Data[1],...,Data[N-1]
        SSD1680_SOFT_RESET                    , 250 , 0   , //0x12
        SSD1680_DRIVER_OUTPUT_CONTROL         , 250 , 3   , 0x27, 0x01, 0x00, //0x01
        SSD1680_DATA_ENTRY_MODE               , 0   , 1   , 0x03, //0x11
        SSD1680_BORDER_CONTROL                , 0   , 1   , 0x80, //0x3C
   //     SSD1680_DISPLAY_UPDATE_CONTROL_RAM, 0,2,0x00,0x80,//0x21
        SSD1680_TEMP_SENSOR                   , 0   , 1   , 0x80, //0x18
        SSD1680_RAM_X_SIZE                    , 0   , 2   , 0x00, x_lsb   , //0x44
        SSD1680_RAM_Y_SIZE                    , 0   , 4   , 0x00, 0x00, y_msb, y_lsb, //0x45
        SSD1680_RAM_X_COUNTER                 , 0   , 1   , 0x00, //0x4E
        SSD1680_RAM_Y_COUNTER                 , 0   , 2   , 0x00, 0x00, //0x4F
        SSD1680_WRITE_LUT_REGISTER            , 250 , LUT_SIZE , PART_UPDATE_LUT, //0x32
        SSD1680_END_OPTION,                        0,    1, 0x22, //0x3f
        SSD1680_GATE_DRIVING_VOLTAGE_CONTROL  , 0   , 1   , 0x17, //0x03
        SSD1680_SOURCE_DRIVING_VOLTAGE_CONTROL, 0   , 3   , 0x41, 0x0 , 0x32, //0x04
        SSD1680_WRITE_VCOM_REGISTER           , 0   , 1   , 0x32, //0x2C
        SSD1680_WRITE_REGISTER_DISPLAY_OPTION, 0 , 10, 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x40 , 0x0, 0x0, 0x0, 0x0,  //0x37
        SSD1680_DISPLAY_UPDATE_CONTROL        , 250 , 1   , 0xC0, //0x22
        SSD1680_ACTIVATE_DISPLAY_UPDATE       , 250 , 0   , //0x20
    };
  */
    const uint8_t ssd1680_init_sequence[] = {
        // Command,                       Delay, N, Data[N]
        SSD1680_SOFT_RESET                    , 250 , 0   , //0x12
#ifdef EINK_BWR
        SSD1680_DRIVER_OUTPUT_CONTROL         , 250 , 3   , 0xf9, 0x00, 0x00, //0x01 bwr
#else
        SSD1680_DRIVER_OUTPUT_CONTROL         , 250 , 3   , 0x27, 0x01, 0x00, //0x01 bw
#endif
        SSD1680_DATA_ENTRY_MODE               , 0   , 1   , 0x03, //0x11
        SSD1680_DISPLAY_UPDATE_CONTROL_RAM,  0, 2,0x0,0x80,//0x21
        SSD1680_BORDER_CONTROL                , 0   , 1   , 0x80, //0x3C
        SSD1680_TEMP_SENSOR                   , 0   , 1   , 0x80, //0x18
        SSD1680_RAM_X_SIZE                    , 0   , 2   , 0x00, x_lsb   , //0x44
        SSD1680_RAM_Y_SIZE                    , 0   , 4   , 0x00, 0x00, y_msb, y_lsb, //0x45
        SSD1680_RAM_X_COUNTER                 , 0   , 1   , 0x00, //0x4E
        SSD1680_RAM_Y_COUNTER                 , 0   , 2   , 0x00, 0x00, //0x4F
        SSD1680_DISPLAY_UPDATE_CONTROL        , 250 , 1   , 0xf7, //0x22
     //   SSD1680_ACTIVATE_DISPLAY_UPDATE       , 250 , 0   , //0x20
    };
    // clang-format on
    //hw_reset(EINK_RST_PIN);
    qp_comms_bulk_command_sequence(device, ssd1680_init_sequence, sizeof(ssd1680_init_sequence));
    wait_for_busy(EINK_BUSY_PIN);
    driver->base.rotation = rotation;

    // clear gets the buffers correctly set to 0/1
    return driver->base.driver_vtable->clear(driver);
}
/*
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
    uint8_t              n_bits   = n_pixels % 8;
    if (n_bits) {
        n_bytes++;
    }

    // send data by indexing the correct places in the buffer
    for (uint16_t row = t; row < b; ++row) {
        uint32_t offset  = (row * w) + l;
        qp_comms_send(eink, surface->buffer + offset, n_bytes);
    }
}

bool set_ram_area(painter_device_t device,uint16_t x, uint16_t y, uint16_t w, uint16_t h){

    uint8_t data[4]={0};
 // set ram mode to normal
    qp_comms_command(device, (uint8_t) SSD1680_DATA_ENTRY_MODE);
    qp_comms_send(device, (const void *)0x03, 1);

    qp_comms_command(device, (uint8_t) SSD1680_RAM_X_SIZE);
    data[0] = x/8;
    data[1] = (x+w-1)/8;
    qp_comms_send(device, (const void *) data, 2);

    qp_comms_command(device, (uint8_t) SSD1680_RAM_Y_SIZE);
    data[0] = y%256;
    data[1] = y/256;
    data[2] = (y + h - 1)%256;
    data[3] = (y + h - 1)/256;
    qp_comms_send(device, (const void *) data, 4);

    qp_comms_command(device, (uint8_t) SSD1680_RAM_X_COUNTER);
    qp_comms_send(device, (const void *) ((uint8_t) x/8), 1);

    qp_comms_command(device, (uint8_t) SSD1680_RAM_Y_SIZE);
    data[0] = y%256;
    data[1] = y/256;
    qp_comms_send(device, (const void *) data, 2);

    return true;

}

bool ssd1680_partial_flush(painter_device_t device) {
    eink_panel_dc_reset_painter_device_t *       driver  = (eink_panel_dc_reset_painter_device_t *)device;
    eink_panel_dc_reset_painter_driver_vtable_t *vtable  = (eink_panel_dc_reset_painter_driver_vtable_t *)driver->base.driver_vtable;
    surface_painter_device_t *                   black   = (surface_painter_device_t *)driver->black_surface;
    surface_painter_device_t *                   red     = (surface_painter_device_t *)driver->red_surface;

    qp_dprintf("ssd1680_partial_flush: entry ok\n");
    if (!(black->dirty.is_dirty || red->dirty.is_dirty)) {
        qp_dprintf("ssd1680_partial_flush: done (no changes to be sent)\n");
        return true;
    }

    if (!driver->can_flush) {
        qp_dprintf("ssd1680_partial_flush: fail (can_flush == false)\n");
        return false;
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Code from: <https://discord.com/channels/440868230475677696/1059874295297347694/1081866413758742538>
    //
    // No idea whether it works or if it contains messages that aren't needed
    //
    // Edited logic to also send red buffer
    //



    // send data
    qp_comms_command(device, vtable->opcodes.send_black_data); // is this what `start_data_` does??
    send_dirty_area(device, black);
    qp_comms_command(device, vtable->opcodes.send_red_data);
    send_dirty_area(device, red);
    // qp_comms_command(device, vtable->opcodes.refresh); // WTF does `end_data_` do??

    // commit as partial
    qp_comms_command(device, (uint8_t)SSD1680_DISPLAY_UPDATE_CONTROL);
    qp_comms_send(device, (const void *)0xCF, 1);
    qp_comms_command(device, vtable->opcodes.refresh);
    // while (readPin(BUSY)) {}; // is this what `wait_until_idle_` does??

    // send again
    wait_ms(300); // can be smaller probably, we've already waited for busy pin
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
    qp_dprintf("ssd1680_partial_flush: exit ok\n");

    return true;
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver vtable

const eink_panel_dc_reset_painter_driver_vtable_t ssd1680_driver_vtable = {
    .base =
        {
            .init            = qp_ssd1680_init,
            .power           = qp_eink_panel_power,
            .clear           = qp_eink_panel_clear,
 //           .flush           = ssd1680_partial_flush,
            .flush           = qp_eink_panel_flush,
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
            .refresh = SSD1680_ACTIVATE_DISPLAY_UPDATE,
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
            driver->base.comms_vtable  = (const painter_comms_vtable_t *)&spi_comms_with_dc_vtable;

            driver->base.native_bits_per_pixel = 1;
            driver->base.panel_width           = panel_width;
            driver->base.panel_height          = panel_height;
            driver->base.rotation              = QP_ROTATION_180;
            driver->base.offset_x              = 0;
            driver->base.offset_y              = 0;

            driver->black_surface = qp_make_mono1bpp_surface(panel_width, panel_height, ptr);
            driver->red_surface   = qp_make_mono1bpp_surface(panel_width, panel_height, ptr + SURFACE_REQUIRED_BUFFER_BYTE_SIZE(panel_width, panel_height, 1));

            // set can_flush = false on start and schedule its reset
            driver->timeout   = 15 * 1000; // 2 minutes as seen on WeAct
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
