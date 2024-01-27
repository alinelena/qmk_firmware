CUSTOM_MATRIX = lite
SRC += matrix.c
QUANTUM_LIB_SRC += spi_master.c

#QUANTUM_PAINTER_DRIVERS = st7735_spi
#QUANTUM_PAINTER_DRIVERS = ls0xx_spi
QUANTUM_PAINTER_DRIVERS += ssd1680_spi
#QUANTUM_PAINTER_LVGL_INTEGRATION = yes

VPATH += keyboards/mlego/m66/qp
SRC += caps-lock.qgf.c \
			 elephant.qgf.c  \
			 elephant.qgf.c  \
			 fira_code.qff.c  \
			 five.qgf.c  \
			 four.qgf.c  \
			 graphics.c  \
			 num-lock.qgf.c  \
			 one.qgf.c  \
			 qmk_icon_48.qgf.c  \
			 scroll-lock.qgf.c  \
			 seven.qgf.c  \
			 six.qgf.c  \
			 thintel15.qff.c  \
			 three.qgf.c  \
			 two.qgf.c  \
			 led.qgf.c  \
			 zero.qgf.c
