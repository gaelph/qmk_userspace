#include "./plaid_led.h"
#include "../custom_keycodes.h"
#include <stddef.h>
#include <stdint.h>

#ifdef KEYBOARD_dm9records_plaid
uint16_t *plaid_led_modifiers;
size_t    plaid_led_modifiers_length;

// Set leds to saved state during powerup
void plaid_led_init(void) {
    // set LED pin modes
    setPinOutput(LED_RED);
    setPinOutput(LED_GREEN);

    // Call the post init code.
    led_config.raw = eeconfig_read_user();

    if (led_config.red_mode == LEDMODE_ON) {
        writePinHigh(LED_RED);
    }

    if (led_config.green_mode == LEDMODE_ON) {
        writePinHigh(LED_GREEN);
    }
}

void plaid_led_eeconfig(void) { // EEPROM is getting reset!
    led_config.raw        = 0;
    led_config.red_mode   = LEDMODE_ON;
    led_config.green_mode = LEDMODE_MODS;
    eeconfig_update_user(led_config.raw);
    eeconfig_update_user(led_config.raw);
}

void plaid_led_set_modifiers(const uint16_t *modifiers, size_t size) {
    plaid_led_modifiers        = (uint16_t *)modifiers;
    plaid_led_modifiers_length = size;
}

void plaid_led_keypress_update(uint8_t led, uint8_t led_mode, uint16_t keycode, keyrecord_t *record) {
    switch (led_mode) {
        case LEDMODE_MODS:
            for (int i = 0; i < plaid_led_modifiers_length; i++) {
                if (keycode == plaid_led_modifiers[i]) {
                    if (record->event.pressed) {
                        writePinHigh(led);
                    } else {
                        writePinLow(led);
                    }
                }
            }
            break;
        case LEDMODE_BLINKIN:
            if (record->event.pressed) {
                if (rand() % 2 == 1) {
                    if (rand() % 2 == 0) {
                        writePinLow(led);
                    } else {
                        writePinHigh(led);
                    }
                }
            }
            break;
        case LEDMODE_KEY:
            if (record->event.pressed) {
                writePinHigh(led);
                return;
            } else {
                writePinLow(led);
                return;
            }
            break;
        case LEDMODE_ENTER:
            if (keycode == KC_ENT) {
                writePinHigh(led);
            } else {
                writePinLow(led);
            }
            break;
    }
}

bool plaid_led_process_record(uint16_t keycode, keyrecord_t *record) {
    /* If the either led mode is keypressed based, call the led updater
   then let it fall through the keypress handlers. Just to keep
   the logic out of this procedure */
    if (led_config.red_mode >= LEDMODE_MODS && led_config.red_mode <= LEDMODE_ENTER) {
        plaid_led_keypress_update(LED_RED, led_config.red_mode, keycode, record);
    }
    if (led_config.green_mode >= LEDMODE_MODS && led_config.green_mode <= LEDMODE_ENTER) {
        plaid_led_keypress_update(LED_GREEN, led_config.green_mode, keycode, record);
    }

    switch (keycode) {
        case LED_1:
            if (record->event.pressed) {
                if (led_config.red_mode == LEDMODE_ON) {
                    led_config.red_mode = LEDMODE_OFF;
                    writePinLow(LED_RED);
                } else {
                    led_config.red_mode = LEDMODE_ON;
                    writePinHigh(LED_RED);
                }
            }
            eeconfig_update_user(led_config.raw);
            return false;
            break;

        case LED_2:
            if (record->event.pressed) {
                if (led_config.green_mode == LEDMODE_ON) {
                    led_config.green_mode = LEDMODE_OFF;
                    writePinLow(LED_GREEN);
                } else {
                    led_config.green_mode = LEDMODE_ON;
                    writePinHigh(LED_GREEN);
                }
            }
            eeconfig_update_user(led_config.raw);
            return false;
            break;
        case LED_3:
            led_config.red_mode = LEDMODE_MODS;
            eeconfig_update_user(led_config.raw);
            return false;
            break;
        case LED_4:
            led_config.green_mode = LEDMODE_MODS;
            eeconfig_update_user(led_config.raw);
            return false;
            break;
        case LED_5:
            led_config.red_mode = LEDMODE_BLINKIN;
            eeconfig_update_user(led_config.raw);
            return false;
            break;
        case LED_6:
            led_config.green_mode = LEDMODE_BLINKIN;
            eeconfig_update_user(led_config.raw);
            return false;
            break;
        case LED_7:
            led_config.red_mode = LEDMODE_KEY;
            eeconfig_update_user(led_config.raw);
            return false;
            break;
        case LED_8:
            led_config.green_mode = LEDMODE_KEY;
            eeconfig_update_user(led_config.raw);
            return false;
            break;
        case LED_9:
            led_config.red_mode = LEDMODE_ENTER;
            eeconfig_update_user(led_config.raw);
            return false;
            break;
        case LED_0:
            led_config.green_mode = LEDMODE_ENTER;
            eeconfig_update_user(led_config.raw);
            return false;
            break;
    }

    return false;
}
#endif
