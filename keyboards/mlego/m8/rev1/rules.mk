CUSTOM_MATRIX = lite
#ifdef OLED_ENABLE
OLED_DRIVER = SSD1306      # Enable Support for SSD1306 or SH1106 OLED Displays; Communicating over I2C
#endif

WS2812_DRIVER = vendor        #
SRC += matrix.c
QUANTUM_LIB_SRC += spi_master.c

#if defined(QUANTUM_PAINTER_ENABLE)
QUANTUM_PAINTER_DRIVERS += ssd1680_spi
USER_NAME := elpekenin
#endif
