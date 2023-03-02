CUSTOM_MATRIX = lite
SRC += matrix.c

ifeq ($(strip $(MAKECMDGOALS)), uf2-split-left)
    QUANTUM_PAINTER_DRIVERS += il91874_spi
else ifeq ($(strip $(MAKECMDGOALS)), uf2-split-right)
    QUANTUM_PAINTER_DRIVERS += ili9163_spi ili9341_spi
endif

RGB_MATRIX_CUSTOM_KB = yes
RGB_MATRIX_DRIVER = WS2812
WS2812_DRIVER = vendor

SPLIT_KEYBOARD = yes
SERIAL_DRIVER = vendor

XAP_ENABLE = yes

# Custom features
CUSTOM_EEPROM = yes
TOUCH_SCREEN  = yes
ONE_HAND      = no
SIPO_PINS     = yes
