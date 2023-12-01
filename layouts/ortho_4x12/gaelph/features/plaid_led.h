#include QMK_KEYBOARD_H

#ifdef KEYBOARD_dm9records_plaid

// Setup consts for LED modes
#    define LEDMODE_ON 1      // always on
#    define LEDMODE_OFF 0     // always off
#    define LEDMODE_MODS 2    // On with modifiers
#    define LEDMODE_BLINKIN 3 // blinkinlights - % chance toggle on keypress
#    define LEDMODE_KEY 4     // On with any keypress, off with key release
#    define LEDMODE_ENTER 5   // On with enter key

// Setup config struct for LED
typedef union {
    uint32_t raw;
    struct {
        uint8_t red_mode : 8;
        uint8_t green_mode : 8;
    };
} led_config_t;
led_config_t led_config;

void plaid_led_init(void);
void plaid_led_eeconfig(void);
void plaid_led_set_modifiers(const uint16_t *modifiers, size_t size);
void plaid_led_keypress_update(uint8_t led, uint8_t led_mode, uint16_t keycode, keyrecord_t *record);
bool plaid_led_process_record(uint16_t keycode, keyrecord_t *record);

#endif
