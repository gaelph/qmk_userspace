// Copyright 2026 Gaël Philippe
// SPDX-License-Identifier: GPL-2.0-or-later
#include "pc_azerty.h"
#include "../defs.h"
#include "../custom_keycodes.h"

// The keymap uses Mac French aliases as its common symbol vocabulary. Do not
// include keymap_french.h here: it redefines the same FR_* names.
// Letters, home-row mods, numbers, and most punctuation match on both hosts.

// Remember the actual output, not the current Shift state, for key release.
// This also lets a held remapped key be released after switching host modes.
static uint16_t held_keys[MATRIX_ROWS][MATRIX_COLS];

typedef struct {
    uint8_t mods;
    uint8_t weak_mods;
    uint8_t oneshot_mods;
} saved_mods_t;

static saved_mods_t save_mods(void) {
    saved_mods_t saved = {get_mods(), get_weak_mods(), get_oneshot_mods()};
    clear_mods();
    clear_weak_mods();
    clear_oneshot_mods();
    send_keyboard_report();
    return saved;
}

static void restore_mods(saved_mods_t saved) {
    set_mods(saved.mods);
    set_weak_mods(saved.weak_mods);
    set_oneshot_mods(saved.oneshot_mods);
    send_keyboard_report();
}

// Classic Windows French lacks these characters. Leading-zero decimal Alt
// codes use Windows-1252 (not OEM codes), with no registry change or helper app.
static void tap_alt_code(uint8_t value) {
    saved_mods_t saved = save_mods();
    bool num_lock = host_keyboard_led_state().num_lock;
    if (!num_lock) {
        tap_code16_delay(KC_NUM_LOCK, 10);
        wait_ms(20);
    }

    register_code(KC_LALT);
    wait_ms(10);
    tap_code16_delay(KC_KP_0, 10);
    for (uint16_t divisor = 100; divisor > 0; divisor /= 10) {
        uint8_t digit = (value / divisor) % 10;
        tap_code16_delay(digit ? KC_KP_1 + digit - 1 : KC_KP_0, 10);
    }
    unregister_code(KC_LALT);

    if (!num_lock) {
        tap_code16_delay(KC_NUM_LOCK, 10);
    }
    restore_mods(saved);
}

// Like the existing Mac punctuation macros, emit once with Shift suppressed,
// keeping Ctrl/Alt/GUI shortcuts and restoring all modifier state afterwards.
static void tap_without_shift(uint16_t keycode) {
    saved_mods_t saved = save_mods();
    set_mods(saved.mods & ~MOD_MASK_SHIFT);
    set_weak_mods(saved.weak_mods & ~MOD_MASK_SHIFT);
    tap_code16_delay(keycode, 10);
    restore_mods(saved);
}

bool process_pc_azerty(uint16_t keycode, keyrecord_t *record) {
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;
    // Combo-generated events need not have a physical matrix position.
    bool physical = IS_KEYEVENT(record->event) && row < MATRIX_ROWS && col < MATRIX_COLS;
    if (!record->event.pressed && physical && held_keys[row][col] != KC_NO) {
        uint16_t held = held_keys[row][col];
        held_keys[row][col] = KC_NO;
        if (held != KC_TRNS) {
            unregister_code16(held);
        }
        return false;
    }

    if (!(default_layer_state & ((layer_state_t)1 << _ANNIE_PC))) {
        return true;
    }

    uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
    bool shift = (mods & MOD_MASK_SHIFT) != 0;
    bool shift_only = shift && !(mods & (MOD_MASK_CTRL | MOD_MASK_ALT | MOD_MASK_GUI));
    bool ctrl_alt = (mods & MOD_MASK_CTRL) && (mods & MOD_MASK_ALT);
    uint16_t output = keycode;
    uint8_t alt_code = 0;
    bool unshift = false;

    switch (keycode) {
        // Keep the custom ,* .! /? shifts and the existing raw-key shortcuts.
        case FR_COMM:
            if (shift_only) {
                output = KC_NUHS;
                unshift = true;
            } else if (ctrl_alt) {
                output = KC_M;
            }
            break;
        case FR_DOT:
            if (shift_only) {
                output = KC_SLSH;
                unshift = true;
            } else if (ctrl_alt) {
                output = KC_DOT;
            }
            break;
        case FR_SLSH:
            if (shift_only) {
                output = S(KC_M);
                unshift = true;
            } else if (ctrl_alt) {
                output = KC_SLSH;
            }
            break;
        case FR_MINS:
            output = shift ? KC_8 : KC_6; // - / _
            unshift = shift;
            break;
        case FR_UNDS:
            output = KC_8;
            unshift = shift;
            break;
        case FR_EQL:
            output = KC_EQL;
            break;
        case FR_PLUS:
            output = S(KC_EQL);
            break;
        case FR_EXLM:
            output = shift ? S(KC_8) : KC_SLSH;
            break;
        case FR_ASTR:
            output = KC_NUHS;
            unshift = shift;
            break;
        case FR_AT: // @ / #
            output = shift ? RALT(KC_3) : RALT(KC_0);
            unshift = shift;
            break;
        case FR_HASH:
            output = RALT(KC_3);
            unshift = shift;
            break;
        case FR_LCBR: // { / [
            output = shift ? RALT(KC_5) : RALT(KC_4);
            unshift = shift;
            break;
        case FR_LBRC:
            output = RALT(KC_5);
            unshift = shift;
            break;
        case FR_RBRC:
            output = RALT(KC_MINS);
            unshift = shift;
            break;
        case FR_RCBR: // } / ]
            output = shift ? RALT(KC_MINS) : RALT(KC_EQL);
            unshift = shift;
            break;
        case FR_PIPE:
            output = RALT(KC_6);
            unshift = shift;
            break;
        case FR_BSLS:
            output = RALT(KC_8);
            unshift = shift;
            break;
        case FR_EURO: // € / ¥
            output = RALT(KC_E);
            alt_code = shift ? 165 : 0;
            break;
        case FR_GRV: // dead ` / £
            output = shift ? S(KC_RBRC) : RALT(KC_7);
            break;
        case __GRV: // literal `
            alt_code = 96;
            break;
        case __TILD: // literal ~
            alt_code = 126;
            break;
        case __CIRC: // literal ^
            alt_code = 94;
            break;
        case FR_LEAC: // é / É
            alt_code = shift ? 201 : 0;
            break;
        case FR_LEGR: // è / È
            alt_code = shift ? 200 : 0;
            break;
        case FR_LCCE: // ç / Ç
            alt_code = shift ? 199 : 0;
            break;
        case FR_LAGR: // à / À
            alt_code = shift ? 192 : 0;
            break;
        case FR_LUGR: // ù / Ù
            alt_code = shift ? 217 : 0;
            break;
        case FR_ECIR: // ê / Ê
            alt_code = shift ? 202 : 234;
            break;
        case FR_OE: // œ / Œ
            alt_code = shift ? 140 : 156;
            break;
        case FR_AE: // æ / Æ
            alt_code = shift ? 198 : 230;
            break;
        case FR_LSQU: // ‘ / ’
            alt_code = shift ? 146 : 145;
            break;
        case FR_LRQU: // ’
            alt_code = 146;
            break;
        case FR_LDAQ: // « / “
            alt_code = shift ? 147 : 171;
            break;
        case FR_RDAQ: // » / ”
            alt_code = shift ? 148 : 187;
            break;
        case FR_ELLP: // … / ·
            alt_code = shift ? 183 : 133;
            break;
        case FR_NDSH: // – / —
            alt_code = shift ? 151 : 150;
            break;
        case FR_MDSH:
            alt_code = 151;
            break;
        case RALT(KC_SPACE): // non-breaking space
            alt_code = 160;
            break;
        default:
            return true;
    }

    if (record->event.pressed) {
        if (alt_code) {
            tap_alt_code(alt_code);
        } else if (unshift) {
            tap_without_shift(output);
        } else {
            register_code16(output);
        }
        if (physical) {
            held_keys[row][col] = (alt_code || unshift) ? KC_TRNS : output;
        }
    } else if (!alt_code && !unshift) {
        unregister_code16(output);
    }
    return false;
}
