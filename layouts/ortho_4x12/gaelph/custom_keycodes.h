// Copyright 2023 Gaël Philippe
// SPDX-Licence-Identifier : MIT
#pragma once
#include QMK_KEYBOARD_H
#include "keymap_french_mac_iso.h"

enum plaid_keycodes {
    ANNIE = SAFE_RANGE,
    QWERTY,
    FRENCH,
    CODE,
    NUMBERS,
    LED_1,
    LED_2,
    LED_3,
    LED_4,
    LED_5,
    LED_6,
    LED_7,
    LED_8,
    LED_9,
    LED_0,
    __CIRC, // ^
    __GRV,  // `
    __TILD, // ~ not dead
    FR_LRQU // missing typographic quote
};

// Keycode shortcuts
// Default Layer
#define FR_ESC LT(_FRENCH, KC_ESC) // tap: esc, hold: french layer
#define FR_QUO LT(_FRENCH, KC_DQUO)
#define CT_ALT LCTL(KC_RALT)          // control+alt
#define NM_SPC LT(_NUMBERS, KC_SPACE) // tap: space, hold: number layer
#define CD_ENT LT(_CODE, KC_ENT)      // tap: enter, hold: number layer

// Home Row mods
// left hand
#define LCTL_A LCTL_T(FR_A)
#define LCTL_I LCTL_T(FR_I)
#define LALT_S LALT_T(FR_S)
#define LGUI_R LGUI_T(FR_R)
// qwerty
#define LGUI_D LGUI_T(FR_D)
// right hand
#define RGUI_E RGUI_T(FR_E)
#define RALT_I RALT_T(FR_I)
#define RALT_A RALT_T(FR_A)
#define RCTL_U RCTL_T(FR_U)
// qwerty
#define RGUI_K RGUI_T(KC_K)
#define RALT_L RALT_T(KC_L)
#define RCTL_SC RCTL_T(KC_SCLN)

// Code Layer
#define TH_DOT RALT(KC_SCLN) // …
