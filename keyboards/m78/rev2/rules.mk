#OPT_DEFS += -DNO_SUSPEND_POWER_DOWN

# Build Options
#   comment out to disable the options.
#
#BACKLIGHT_ENABLE = no              # Enable keyboard backlight functionality
#BOOTMAGIC_ENABLE = lite            # Virtual DIP switch configuration
#MOUSEKEY_ENABLE = yes              # Mouse keys
#EXTRAKEY_ENABLE = yes              # Audio control and System control
##CONSOLE_ENABLE = no                # Console for debug
#COMMAND_ENABLE = no                # Commands for debug and configuration
#SLEEP_LED_ENABLE = no              # Breathing sleep LED during USB suspend
#NKRO_ENABLE = yes                  # USB Nkey Rollover
#AUDIO_ENABLE = no                  # Audio output on port C6
#NO_USB_STARTUP_CHECK = no          # Disable initialization only when usb is plugged in
#SERIAL_LINK_ENABLE = yes


#LAYOUTS = 65_ansi
# MCU name
MCU = STM32F303

# Build Options
#   change to "no" to disable the options, or define them in the Makefile in
#   the appropriate keymap folder that will get included automatically
#
BOOTMAGIC_ENABLE = yes      # Virtual DIP switch configuration
## (Note that for BOOTMAGIC on Teensy LC you have to use a custom .ld script.)
MOUSEKEY_ENABLE = yes      # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes         # Console for debug
COMMAND_ENABLE = yes       # Commands for debug and configuration
NKRO_ENABLE = yes            # Nkey Rollover - if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
BACKLIGHT_ENABLE = no      # Enable keyboard backlight functionality
MIDI_ENABLE = no            # MIDI controls
AUDIO_ENABLE = yes           # Audio output
UNICODE_ENABLE = no         # Unicode
BLUETOOTH_ENABLE = no       # Enable Bluetooth with the Adafruit EZ-Key HID
RGBLIGHT_ENABLE = no        # Enable WS2812 RGB underlight.
#WS2812_DRIVER = pwm
API_SYSEX_ENABLE = no

# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no    # Breathing sleep LED during USB suspend

# RGB_MATRIX_ENABLE = WS2812
# SERIAL_LINK_ENABLE = yes
ENCODER_ENABLE = no
DIP_SWITCH_ENABLE = no

#LAYOUTS = ortho_4x12 planck_mit
#LAYOUTS_HAS_RGB = no

