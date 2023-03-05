CUSTOM_MATRIX = lite
SRC += matrix.c graphics.c lightbulb_off_outline.qgf.c ab-testing.qgf.c elephant.qgf.c fira_code.qff.c lightbulb_on_20.qgf.c lightbulb_on_40.qgf.c lightbulb_on_60.qgf.c lightbulb_on_80.qgf.c lightbulb_on.qgf.c qmk_icon_48.qgf.c   
WS2812_DRIVER = vendor        #
QUANTUM_LIB_SRC += spi_master.c
QUANTUM_PAINTER_DRIVERS += ssd1680_spi
