#include "action.h"
#include "keycode.h"
#include QMK_KEYBOARD_H
#include <keymap_french_mac_iso.h>
#include <sendstring_french_mac_iso.h>

typedef void (*custom_shift_handler_t)(uint8_t mods);

typedef struct custom_shift {
    uint16_t               keycode;
    uint16_t               replacement;
    uint8_t                mod_mask;
    bool                   exact_mod_match;
    uint8_t                exclude_mod_mask;
    custom_shift_handler_t handler;
} custom_shift_t;

void comma_asterisk(uint8_t mods) {
    bool shift = (mods & MOD_MASK_SHIFT) != 0;
    bool alt   = (mods & MOD_MASK_ALT) != 0;
    bool gui   = (mods & MOD_MASK_GUI) != 0;
    bool ctrl  = (mods & MOD_MASK_CTRL) != 0;

    if (shift && (!ctrl && !alt && !gui)) {
        SEND_STRING(SS_DOWN(X_LSFT) SS_TAP(X_RBRC) SS_UP(X_LSFT));
    } else if ((ctrl && alt) || (ctrl && alt && gui)) {
        SEND_STRING(SS_TAP(X_M));
    }
}

custom_shift_t shift_table[] = {
    //
    {
        //
        .keycode         = FR_COMM,
        .replacement     = LSFT(KC_RBRC),
        .mod_mask        = MOD_MASK_SHIFT,
        .exact_mod_match = true,
    },
    {
        //
        .keycode          = FR_COMM,
        .replacement      = KC_M,
        .mod_mask         = MOD_MASK_CTRL | MOD_MASK_ALT,
        .exclude_mod_mask = MOD_MASK_SHIFT //
    }                                      //

};

bool check_match(custom_shift_t cs, uint8_t mods) {
    bool match = false;

    if (cs.exact_mod_match) {
        match = (mods & cs.mod_mask) == cs.mod_mask;
        return match;
    }

    match = (mods & cs.mod_mask) != 0;
    if (match && (cs.exclude_mod_mask != 0)) {
        match = match && ((mods & cs.exclude_mod_mask) == 0);
    }

    return match;
}

bool process_custom_shift(uint16_t keycode, keyrecord_t *record) {
    uint8_t mods = get_mods();

    if (record->event.pressed) {
        for (uint8_t i = 0; i < sizeof(shift_table); i++) {
            if (shift_table[i].keycode == keycode) {
                if (check_match(shift_table[i], mods)) {
                    register_code16(shift_table[i].replacement);
                }
            }
        }
    }
}
