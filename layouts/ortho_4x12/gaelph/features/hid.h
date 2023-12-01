// Copyright 2023 Gaël Philippe
// SPDX-Licence-Identifier : MIT
#include QMK_KEYBOARD_H
#include <raw_hid.h>
#include <print.h>

void layer_state_set_hid(layer_state_t state);
void send_event_to_hid(uint16_t keycode, keyevent_t event);

enum raw_hid_commands {
    HID_CMD_UNKNOWN,
    HID_CMD_GET_LAYERS, // Host ask for layer data
    // Host asks for layer metadata (height, width, and count)
    HID_CMD_GET_LAYERS_METADATA,
    HID_EVENT // Keyboard emits key event
};

void raw_hid_receive(uint8_t *data, uint8_t length);

uint16_t next_call_id(void);
