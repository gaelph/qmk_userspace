// Copyright 2023 Gaël Philippe
// SPDX-Licence-Identifier : MIT
#include "keycodes.h"
#include QMK_KEYBOARD_H
#include "keymap_french_mac_iso.h"
#include "sendstring_french_mac_iso.h"
#include "./custom_keycodes.h"

// COMBOS !
const uint16_t PROGMEM CB_ESC[]   = {KC_N, RGUI_E, COMBO_END};
const uint16_t PROGMEM CB_TAB[]   = {KC_T, LGUI_R, COMBO_END};
const uint16_t PROGMEM CB_BSPC[]  = {KC_F, KC_O, COMBO_END};
const uint16_t PROGMEM CB_CMD_U[] = {KC_L, KC_P, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    COMBO(CB_ESC, KC_ESC),
    COMBO(CB_TAB, KC_TAB),
    COMBO(CB_BSPC, KC_BSPC),
    COMBO(CB_CMD_U, LGUI(KC_U)),
};
