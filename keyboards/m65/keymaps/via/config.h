
#define DYNAMIC_KEYMAP_LAYER_COUNT 4
#define EEPROM_PAGE_SIZE
#define FEE_PAGE_SIZE (uint16_t)0x800
#define FEE_DENSITY_PAGES 4

#define EEPROM_START_ADDRESS
#define FEE_MCU_FLASH_SIZE                              \
({                                                      \
    uint16_t flash_size = *(uint16_t*)FLASHSIZE_BASE;   \
    (flash_size <= 512) ? flash_size : 512;             \
})



