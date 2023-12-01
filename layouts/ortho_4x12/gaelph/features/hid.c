// Copyright 2023 Gaël Philippe
// SPDX-Licence-Identifier : MIT
#include QMK_KEYBOARD_H
#include "./hid.h"
#include <raw_hid.h>
#include <stdint.h>
#include <string.h>
#include <print.h>
#include "./defs.h"

extern const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS];
extern const size_t           N_LAYERS;

int8_t   layer           = 0;
uint16_t current_call_id = 0;

const uint16_t MAX_CALL_ID = 0xFFFF;
#define HID_MESSAGE_LENGTH 32;
#define HID_PACKET_SIZE 32

uint16_t next_call_id(void) {
    if (current_call_id == MAX_CALL_ID) {
        current_call_id = 0;
    } else {
        current_call_id++;
    }

    return current_call_id;
}

typedef struct list_layer_data {
    uint8_t n_layers;
    uint8_t n_rows;
    uint8_t n_cols;
    uint8_t layer_data[6 * MATRIX_ROWS * MATRIX_COLS];
} list_layer_data_t;

typedef __attribute__((scalar_storage_order("big-endian"))) struct {
    uint8_t  report_id;
    uint8_t  command_code;
    uint16_t call_id;
    uint8_t  packet_number;
    uint8_t  total_packets;
} hid_header_t;

/*
 * send_hid_data
 *
 * Sends the given data as a sequence of packets to an HID device, where each packet can contain up to 32 bytes of data.
 * The function adds a header to each packet, which contains a 8-bit command code, a 16-bit call ID, the 8-bit packet number, and the 8-bit total number of packets.
 *
 * Parameters:
 * - data: pointer to the data to be sent
 * - data_len: length of the data in bytes
 *
 * Returns:
 * - None
 */
void send_hid_data(uint8_t cmd, uint16_t call_id, uint8_t *data, uint16_t length) {
    hid_header_t header;
    int          header_size         = sizeof(header);
    int          usable_payload_size = HID_PACKET_SIZE - header_size;
    uint8_t      packet[HID_PACKET_SIZE];
    uint16_t     bytes_sent    = 0;
    uint8_t      packet_number = 0;
    uint8_t      total_packets = (length + usable_payload_size - 1) / usable_payload_size;

#ifdef CONSOLE_ENABLE
    uprintf("SENDING cmd: 0x%X, call_id: 0x%X, length: %d\n", cmd, call_id, length);
#endif

    while (bytes_sent < length) {
        header.report_id     = 0;
        header.command_code  = cmd;
        header.call_id       = call_id;
        header.packet_number = packet_number;
        header.total_packets = total_packets;

        memcpy(packet, &header, header_size);
        memcpy(packet + header_size, data + bytes_sent, usable_payload_size);

        raw_hid_send(packet, HID_PACKET_SIZE);

        bytes_sent += usable_payload_size;
        packet_number++;
    }

#ifdef CONSOLE_ENABLE
    uprintf("%u bytes were sent\n", bytes_sent);
#endif
}

/**
 * Sends Layer data to the host computer
 * That the whole content of the keymaps array
 */
void send_layer_data_hid(uint16_t call_id) {
    uint16_t total_layers_length = N_LAYERS * MATRIX_ROWS * MATRIX_COLS * sizeof(uint16_t);

    uint16_t *data = malloc(total_layers_length);
#ifdef CONSOLE_ENABLE
    uprintf("SENDING LAYER_DATA %2u worthof bytes\n", total_layers_length);
#endif
    uint16_t idx = 0;
    for (uint8_t i = 0; i < N_LAYERS; i++) {
        for (uint8_t j = 0; j < MATRIX_ROWS; j++) {
            for (uint8_t k = 0; k < MATRIX_COLS; k++) {
                data[idx++] = pgm_read_word(&keymaps[i][j][k]);
#ifdef CONSOLE_ENABLE
                uprintf("KEYMAP: %2u 0x%X\n", idx - 1, data[idx - 1]);
#endif
            }
        }
    }

    send_hid_data(HID_CMD_GET_LAYERS, call_id, (uint8_t *)data, total_layers_length);
    free(data);
}

typedef struct {
    uint8_t n_layers;
    uint8_t rows;
    uint8_t cols;
} hid_layer_metadata_t;

/**
 * Sends Layer metadata to the host computer
 * Includes number of layers, rows and columns
 */
void send_layer_metadata_hid(uint16_t call_id) {
    hid_layer_metadata_t metadata = {0};
    metadata.n_layers             = N_LAYERS;
    metadata.rows                 = MATRIX_ROWS;
    metadata.cols                 = MATRIX_COLS;

#ifdef CONSOLE_ENABLE
    uprintf("SENDING CMD_LAYER_METADATA %u %u %u\n", metadata.n_layers, metadata.rows, metadata.cols);
#endif

    send_hid_data(HID_CMD_GET_LAYERS_METADATA, call_id, (uint8_t *)&metadata, sizeof(metadata));
}

/** Host event reception **/

typedef struct {
    uint8_t  cmd;
    uint16_t call_id;
} hid_cmd_call_t;

void raw_hid_receive(uint8_t *data, uint8_t length) {
#ifdef CONSOLE_ENABLE
    uprint("--- HID RECEIVE ---");
#endif
    if (length < 2) {
#ifdef CONSOLE_ENABLE
        uprintf("RECEIVED invalid data of length %u\n", length);
#endif
        return;
    }
    hid_cmd_call_t *cmd_call = (hid_cmd_call_t *)data;

    switch (cmd_call->cmd) {
        case HID_CMD_GET_LAYERS:
#ifdef CONSOLE_ENABLE
            uprint("RECEIVED HID_CMD_GET_LAYERS");
#endif
            send_layer_data_hid(cmd_call->call_id);
            break;
        case HID_CMD_GET_LAYERS_METADATA:
#ifdef CONSOLE_ENABLE
            uprint("RECEIVED HID_CMD_GET_LAYERS_METADATA");
#endif
            send_layer_metadata_hid(cmd_call->call_id);
            break;
        default:
#ifdef CONSOLE_ENABLE
            uprintf("RECEIVED unknown command %u\n", cmd_call->cmd);
#endif
            break;
    }
}

/* Key event sending */
typedef struct {
    uint16_t keycode;
    uint8_t  col;
    uint8_t  row;
    uint8_t  pressed;
    uint8_t  mods;
    uint8_t  layer;
} hid_event_t;

void send_event_to_hid(uint16_t keycode, keyevent_t event) {
    hid_event_t hidevent;
    hidevent.keycode = keycode;
    hidevent.col     = event.key.col;
    hidevent.row     = event.key.row;
    hidevent.pressed = event.pressed ? 1 : 0;
    hidevent.mods    = get_mods();
    hidevent.layer   = get_highest_layer(layer_state);
#ifdef CONSOLE_ENABLE
    uprintf("SENDING HID_EVENT 0x%X %2u %2u %u %X %u\n", hidevent.keycode, hidevent.col, hidevent.row, hidevent.pressed, hidevent.mods, hidevent.layer);
#endif

    send_hid_data(HID_EVENT, next_call_id(), (uint8_t *)&hidevent, sizeof(event));
}
