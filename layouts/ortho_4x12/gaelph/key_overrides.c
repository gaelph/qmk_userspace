// Copyright 2023 Gaël Philippe
// SPDX-Licence-Identifier : MIT
#include QMK_KEYBOARD_H
#include "./defs.h"
#include "./custom_keycodes.h"

// NOTE: Since this is the only key override that worked,
// maybe we should move it to separate feature module
// allong with the the other custom shift keys
//
// print em dash on shift+en dash
const key_override_t ndash_mdash_override = ko_make_with_layers(MOD_MASK_SHIFT, FR_NDSH, FR_MDSH, _FRENCH);

const key_override_t **key_overrides = (const key_override_t *[]){&ndash_mdash_override, NULL};
