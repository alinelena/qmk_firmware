CUSTOM_MATRIX = lite
OLED_DRIVER = SSD1306      # Enable Support for SSD1306 or SH1106 OLED Displays; Communicating over I2C

WS2812_DRIVER = vendor        #
SRC += matrix.c
QUANTUM_LIB_SRC += spi_master.c
QUANTUM_PAINTER_DRIVERS += ssd1680_spi


USER_NAME := elpekenin
