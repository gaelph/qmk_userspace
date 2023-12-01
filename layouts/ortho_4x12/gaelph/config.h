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

#pragma once

// Unfortunately, some applications drop or misorder fast key events. This is a
// partial fix to slow down the rate at which macros are sent.
// #define TAP_CODE_DELAY 12

// Disable quick tap term so that numbers can be typed rapidely
#define QUICK_TAP_TERM 0

// Tap-Hold config for Home row mods
#define TAPPING_TERM 300
// #define IGNORE_MOD_TAP_INTERRUPT // That should be the default, in a future version?
#define TAPPING_TERM_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define PERMISSIVE_HOLD
#define PERMISSIVE_HOLD_PER_KEY

#define COMBO_COUNT 4
#define COMBO_TERM 25

// This is the default, used for hid, i.e. Keystats
#define RAW_USAGE_PAGE 0xFF60
#define RAW_USAGE 0x61

#define KEYSTAT_ENABLE
