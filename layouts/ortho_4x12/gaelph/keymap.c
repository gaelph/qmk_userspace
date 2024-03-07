/* Copyright 2023 Gaël Philippe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "action_tapping.h"
#include <keymap_french_mac_iso.h>
#include <sendstring_french_mac_iso.h>
#include <print.h>

#include "./features/hid.h"
#ifdef KEYBOARD_dm9records_plaid
#    include "./features/plaid_led.h"
#endif

#include "./defs.h"
#include "./combos.h"
#include "./custom_keycodes.h"

extern uint16_t g_tapping_term;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Annie Layeer
     * ,-----------------------------------------------------------------------------------------------------------.
     * | Tab              |   Q  |   C   |   L  |   P  |   B   |   K   |   F  |   O  |   Y  |   K  | Bksp          |
     * |------------------+------+-------+------+------+-------+-------+------+------+------+------+---------------|
     * | Esc or AnnieAlt  |   A  |   S   |   R  |   T  |   G   |   M   |   N  |   E  |   I  |   U  | AnneAlt       |
     * |------------------+------+-------+------+------+-------|-------+------+------+------+------+---------------|
     * | Ctrl + Alt       |   Z  |   V   |   J  |   D  |   W   |   X   |   H  |   ,  |   .  |   /  | Ctrl + Alt    |
     * |------------------+------+-------+------+------+-------+-------+------+------+------+------+---------------|
     * | RAlt             | Ctrl | LALT  | LGUI | LSFT | NMSBC | CDENT | RSFT | RGUI | RALT | Ctrl | RAlt          |
     * `--------------------------------------------------------------------------------------------------------'
     */
    // clang-format off
[_ANNIE] = LAYOUT_ortho_4x12(
  KC_TAB,  FR_Q,    FR_C,    FR_L,    FR_P,    FR_B,   FR_K,   FR_F,    FR_O,    FR_Y,    FR_MINS, KC_BSPC,
  FR_ESC,  LCTL_A,  LALT_S,  LGUI_R,  FR_T,    FR_G,   FR_M,   FR_N,    RGUI_E,  RALT_I,  RCTL_U,  MO(_FRENCH),
  CT_ALT,  FR_Z,    FR_V,    FR_J,    FR_D,    FR_W,   FR_X,   FR_H,    FR_COMM, FR_DOT,  FR_SLSH, CT_ALT,
  KC_RALT, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, NM_SPC, CD_ENT, KC_RSFT, KC_RGUI, KC_RALT, KC_RCTL, KC_RALT
),

[_QWERTY] = LAYOUT_ortho_4x12(
  KC_TAB,  FR_Q,    FR_W,    FR_E,    FR_R,    FR_T,    FR_Y,    FR_U,    FR_I,    FR_O,    FR_P,    KC_BSPC,
  KC_ESC,  FR_A,    FR_S,    FR_D,    FR_F,    FR_G,    FR_H,    FR_J,    FR_K,    FR_L,    FR_SCLN, FR_QUOT,
  CT_ALT,  FR_Z,    FR_X,    FR_C,    FR_V,    FR_B,    FR_N,    FR_M,    FR_COMM, FR_DOT,  FR_SLSH, CT_ALT,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

/* French Layer
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   œ  |   «  |   ê  |   ù  |   °  |      |   ‘  |   ’  |   –  |   –  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | ___  |   à  |   ’  |   é  |   è  |   ç  |      |   :  |   ;  |   …  |   @  |  $   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | ___  |   æ  |   »  |   ¨  |   ˆ  |   €  |   +  |   -  |   #  |   !  |   ?  | ___  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | ___  | ___  | ___  | ____ | ____ | NBSP | ____ | ____ | ____ | ____ | ____ | ____ |
 * `-----------------------------------------------------------------------------------'
 */
[_FRENCH] = LAYOUT_ortho_4x12(
  FR_GRV,  FR_OE,   FR_LDAQ, FR_ECIR, FR_LUGR, FR_DEG,         XXXXXXX, FR_LSQU, FR_LRQU, XXXXXXX, FR_NDSH, _______,
  _______, FR_LAGR, FR_LRQU, FR_LEAC, FR_LEGR, FR_LCCE,        XXXXXXX, FR_COLN, FR_SCLN, FR_ELLP, FR_AT,   FR_DLR,
  _______, FR_AE,   FR_RDAQ, FR_DIAE, FR_CIRC, FR_EURO,        FR_PLUS, FR_MINS, FR_HASH, FR_EXLM, FR_QUES, _______,
  _______, _______, _______, _______, _______, RALT(KC_SPACE), _______, _______, _______, _______, _______, _______
),

/* Code Layer
 *
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   &  |   <  |   [  |   ]  |   /  |      |   "  |   '  |      |      | DEL  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | ESC  |   `  |   =  |   (  |   )  |   !  |   ^  |   :  |   ;  |      |   @  |  $   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | ____ |   |  |   >  |   {  |   }  |   %  |   +  |   -  |   #  |   …  |   \  | ____ |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | ____ | ____ | ____ | ___  | ____ | ____ | ____ | ____ | ____ | ____ | ____ | ____ |
 * `-----------------------------------------------------------------------------------'
 */
[_CODE] = LAYOUT_ortho_4x12(
  __TILD,  FR_AMPR, FR_LABK, FR_LBRC, FR_RBRC, FR_SLSH, XXXXXXX, FR_QUOT, FR_DQUO, XXXXXXX, FR_UNDS, KC_DEL,
  KC_ESC,  __GRV,   FR_EQL,  FR_LPRN, FR_RPRN, FR_EXLM, __CIRC,  FR_COLN, FR_SCLN, XXXXXXX, FR_AT,   FR_DLR,
  _______, FR_PIPE, FR_RABK, FR_LCBR, FR_RCBR, FR_PERC, FR_PLUS, FR_MINS, FR_HASH, FR_ELLP, FR_BSLS, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

/* Numbers Layer
 * ,-------------------------------------------------------------------------------------------.
 * | ____ |   1  |   2  |    3    |     4     |   5  |   6  |   7  |   8  |   9  |   0  | ____ |
 * |------+------+------+---------+-----------+------+------+------+------+------+------+------|
 * | ____ |      | home | page up | page down | end  | Left | Down | Up   | Right|   *  | ____ |
 * |------+------+------+---------+-----------+------|------+------+------+------+------+------|
 * | ____ |      |      |         |           |      |   +  |   -  |   ,  |   .  |   /  | ____ |
 * |------+------+------+---------+-----------+------+------+------+------+------+------+------|
 * | ____ | ____ | ____ | _______ | _________ | ____ | ____ | ____ | ____ | ____ | ____ | ____ |
 * `-------------------------------------------------------------------------------------------'
 */
[_NUMBERS] = LAYOUT_ortho_4x12(
    _______, FR_1,    FR_2,    FR_3,    FR_4,    FR_5,    FR_6,    FR_7,    FR_8,    FR_9,    FR_0,    _______,
    _______, XXXXXXX, KC_HOME, KC_PGUP, KC_PGDN, KC_END,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, FR_ASTR, _______,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, FR_PLUS, FR_MINS, FR_COMM, FR_DOT,  FR_SLSH, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),


/* Adjust (Lower + Raise)
 * ,-------------------------------------------------------------------------------------.
 * | BOOT   |      |      | MGT  | MUGE |      |      |      |      |      |      |      |
 * |--------+------+------+------+------+-------------+------+------+------+------+------|
 * | ANNIE  | F1   | F2   | F3   | F4   | F5   | F6   | F7   | F8   | F9   | F10  |      |
 * |--------+------+------+------+------+------|------+------+------+------+------+------|
 * | QWERTY |      | BGT- | BGT+ |      |      | Mute | Next | Vol- | Vol+ | Play |      |
 * |--------+------+------+------+------+------+------+------+------+------+------+------|
 * |        |      |      |      |      |             |      |      |      |      |      |
 * `-------------------------------------------------------------------------------------'
 */
#ifdef KEYBOARD_boardsource_equals_48
[_ADJUST] = LAYOUT_ortho_4x12(
    QK_BOOT, RGB_TOG, RGB_MOD, RGB_RMOD, RGB_HUD, RGB_HUI, RGB_SAD, RGB_SAI, RGB_VAD, RGB_VAI, _______, _______,
    ANNIE,   KC_F1,   KC_F2,   KC_F3,    KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  RGB_SPD,
    QWERTY,  DT_DOWN, KC_BRID, KC_BRIU,  DT_UP,   DT_PRNT, KC_MUTE, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY, RGB_SPI,
    _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______
)
#endif
#ifdef KEYBOARD_dm9records_plaid
[_ADJUST] = LAYOUT_plaid_grid(
    QK_BOOT, LED_1,   LED_2,   LED_3,   LED_4,   LED_5,   LED_6,   LED_7,   LED_8,   LED_9,   LED_0,   _______,
    ANNIE,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
    QWERTY,  DT_DOWN, KC_BRID, KC_BRIU, DT_UP,   DT_PRNT, KC_MUTE, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
)
#endif
    // clang-format on
};

// For keystats
const size_t N_LAYERS = sizeof(keymaps) / (MATRIX_ROWS * MATRIX_COLS * sizeof(uint16_t));

// This is used to know if modifiers are pressed
// when we handle custom shift behaviors
#ifdef KEYBOARD_dm9records_plaid
const uint16_t modifiers[]      = {KC_LCTL, KC_RCTL, KC_LALT, KC_RALT, KC_LSFT, KC_RSFT, KC_LGUI, KC_RGUI, CODE, NUMBERS, FRENCH, MO(_FRENCH)};
const size_t   modifiers_length = sizeof(modifiers) / sizeof(modifiers[0]);

#endif
/**
 * Optionally enable debugging on start
 */
void keyboard_post_init_user(void) {
    debug_enable   = true;
    debug_matrix   = true;
    debug_keyboard = true;

#ifdef KEYBOARD_dm9records_plaid
    plaid_led_init();
    plaid_led_set_modifiers(modifiers, modifiers_length);
#endif
}

void eeconfig_init_user(void) {
#ifdef KEYBOARD_dm9records_plaid
    plaid_led_eeconfig();
#endif
}

/**
 * Used to trigger the layer with Fn keys with
 * both space and enter pressed
 */
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _CODE, _NUMBERS, _ADJUST);
}

/**
 * Implements custom keycodes and overrides some behaviors
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t mods = get_mods();

    // current state of modifiers
    bool shift = (mods & MOD_MASK_SHIFT) != 0;
    bool alt   = (mods & MOD_MASK_ALT) != 0;
    bool gui   = (mods & MOD_MASK_GUI) != 0;
    bool ctrl  = (mods & MOD_MASK_CTRL) != 0;

#ifdef CONSOLE_ENABLE
    uprintf("KC: kc 0x%X, c: %2u, r: %2u, pressed: %u, mods: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, mods);
#endif

#ifdef KEYSTAT_ENABLE
    send_event_to_hid(keycode, record->event);
#endif

#ifdef KEYBOARD_dm9records_plaid
    if (plaid_led_process_record(keycode, record)) {
        return true;
    }
#endif

    switch (keycode) {
        // Default layer
        case ANNIE:
            if (record->event.pressed) {
                print("mode just switched to annie and this is a huge strig\n");
                set_single_persistent_default_layer(_ANNIE);
            }
            return false;
            break;

        case QWERTY:
            if (record->event.pressed) {
                print("mode just switched to qwerty and this is a huge string\n");
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
            break;

        // The code layer
        case CODE:
            if (record->event.pressed) {
                layer_on(_CODE);
            } else {
                layer_off(_CODE);
            }
            return false;
            break;

        // The number layer
        case NUMBERS:
            if (record->event.pressed) {
                layer_on(_NUMBERS);
            } else {
                layer_off(_NUMBERS);
            }
            return false;
            break;

        // The french layer
        case FRENCH:
            if (record->event.pressed) {
                layer_on(_FRENCH);
            } else {
                layer_off(_FRENCH);
            }
            return false;
            break;

        // ,*
        // we want an asterisk when shifted,
        // but we still want ctrl+alt(+shift)(+gui) and gui+(+shift) + comma
        // to work as usual
        case FR_COMM:
            if (record->event.pressed) {
                if (shift && (!ctrl && !alt && !gui)) {
                    unregister_mods(MOD_MASK_SHIFT);
                    SEND_STRING(SS_DOWN(X_LSFT) SS_TAP(X_RBRC) SS_UP(X_LSFT));
                    register_mods(mods);

                    return false;
                } else if ((ctrl && alt) || (ctrl && alt && gui)) {
                    SEND_STRING(SS_TAP(X_M));
                    return false;
                }
            }
            break;

        // .!
        // dot is shifted key on the azerty layout,
        // so we un shift it for ctrl+alt(+gui)
        // combinations
        case FR_DOT:
            if (record->event.pressed) {
                if (shift && (!ctrl && !alt && !gui)) {
                    unregister_mods(MOD_MASK_SHIFT);
                    SEND_STRING(SS_TAP(X_8));
                    // put back the mods as they were
                    register_mods(mods);

                    return false;
                } else if ((ctrl && alt) || (ctrl && alt && gui)) {
                    SEND_STRING(SS_TAP(X_DOT));

                    return false;
                }
            }
            break;

        case FR_SLSH:
            if (record->event.pressed) {
                if (shift && (!ctrl && !alt && !gui)) {
                    unregister_mods(MOD_MASK_SHIFT);
                    SEND_STRING(SS_DOWN(X_LSFT) SS_TAP(X_M) SS_UP(X_LSFT));
                    register_mods(mods);
                    return false;
                } else if ((ctrl && alt) || (ctrl && alt && gui)) {
                    SEND_STRING(SS_TAP(X_SLSH));
                    return false;
                }
            }
            break;

        // French special characters
        case __TILD: // ~ (not dead)
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LALT) SS_TAP(X_N) SS_UP(X_LALT) SS_TAP(X_SPC));
            }
            return false;
            break;

        case __CIRC: // ^ (not dead)
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_LBRC) SS_TAP(X_SPC));
            }
            return false;
            break;

        case __GRV: // ` (not dead)
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_BSLS) SS_DELAY(100) SS_TAP(X_SPC));
            }

            return false;
            break;

        case FR_NDSH:
            if (record->event.pressed) {
                if (shift) {
                    unregister_mods(MOD_MASK_SHIFT);
                    register_code16(FR_MDSH);
                    register_mods(mods);
                    return false;
                }
            } else if (shift) {
                unregister_code16(FR_MDSH);
            }
            break;

        case FR_LCCE: // çÇ
            if (record->event.pressed) {
                if (shift) {
                    unregister_mods(MOD_MASK_SHIFT);
                    register_code16(FR_CCCE);
                    register_mods(mods);
                    return false;
                }
            } else if (shift) {
                unregister_code16(FR_CCCE);
            }
            break;

        case FR_LEGR: // èÈ
            if (record->event.pressed) {
                if (shift) {
                    unregister_mods(MOD_MASK_SHIFT);
                    register_code16(FR_CEGR);
                    register_mods(mods);
                    return false;
                }
            } else if (shift) {
                unregister_code16(FR_CEGR);
            }

            break;

        case FR_LEAC: // éÉ
            if (record->event.pressed) {
                if (shift) {
                    // the acute accent requires alt+shift, but shift is already pressed
                    // so that shift+alt+&, then shift+e
                    SEND_STRING(SS_DOWN(X_RALT) SS_TAP(X_1) SS_UP(X_RALT) SS_TAP(X_E));
                    return false;
                }
                return true;
            }
            break;

        case FR_ECIR: // êÊ
            if (record->event.pressed) {
                if (shift) {
                    // Ê requires shift+alt+e, put shift is already pressed
                    SEND_STRING(SS_RALT("e"));
                    return false;
                }

                return true;
            }
            break;

        case FR_LAGR: // àÀ
            if (record->event.pressed) {
                if (!shift) {
                    return true;
                }

                // capital À requires ` (dead), and that is X_BSLS with no
                // shift on an AZERTY mac layout
                unregister_mods(MOD_MASK_SHIFT);
                SEND_STRING(SS_TAP(X_BSLS));
                register_mods(mods);

                SEND_STRING("a");

                return false;
            }
            break;

        case FR_LUGR: // ùÙ
            if (record->event.pressed) {
                if (shift) {
                    unregister_mods(MOD_MASK_SHIFT);
                    register_code16(FR_CUGR);
                    register_mods(mods);
                    return false;
                }
            }
            break;

            // ’ typographic apostrophe, missing from the QMK headers
        case FR_LRQU:
            if (record->event.pressed) {
                if (!shift) {
                    register_code(KC_LSFT);
                }
                SEND_STRING(SS_RALT("\'"));
                if (!shift) {
                    unregister_code(KC_LSFT);
                }

                return false;
            }
            break;

        // «“ make the shift of « be “
        case FR_LDAQ:
            if (record->event.pressed) {
                if (shift) {
                    unregister_mods(MOD_MASK_SHIFT);
                    SEND_STRING(SS_RALT("\""));
                    return false;
                }

            } else {
                if (shift) {
                    register_mods(mods);
                    return false;
                }
            }
            break;

        // »”
        case FR_RDAQ:
            if (record->event.pressed) {
                if (shift) {
                    SEND_STRING(SS_DOWN(X_RALT) SS_TAP(X_3) SS_UP(X_RALT));
                    return false;
                }

                return true;
            }
            break;

        // …·
        case FR_ELLP:
            if (record->event.pressed) {
                if (shift) {
                    SEND_STRING(SS_DOWN(X_RALT) SS_TAP(X_F) SS_UP(X_RALT));
                    return false;
                }
                return true;
            }
            break;
    }
    return true;
}

// We don’t want code to have roling habilities
// so if there is a key interrupting it while pressed
// we trigger the code layer immediately
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CD_ENT:
        case FR_ESC:
        case MO(_FRENCH):
            return true;
    }
    return false;
}

// Never allow permissive holds
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case FR_ESC:
        case NM_SPC:
            return true;
    }
    return false;
}

// Tapping term ajustements depending on which tap-mod
// it pressed
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    uint16_t term = g_tapping_term;

    switch (keycode) {
        // escape layer require a swift mod change
        case FR_ESC:
            term = g_tapping_term - 80;
            break;

        // space/num, enter/code require an even swifter one
        case NM_SPC:
        case CD_ENT:
            term = g_tapping_term - 100;
            break;

        // a, u, s and i require some lagging
        // because the pinky and ring fingers
        // can lag a bit
        // Same goes for the french MO layer
        case LCTL_A:
        case RCTL_U:
        case LALT_S:
        case RALT_I:
        case MO(_FRENCH):
            term = g_tapping_term + 50;
            break;

        default:
            break;
    }

    return term;
}
