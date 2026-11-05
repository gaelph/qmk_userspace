// Minimal QMK host mock. Keycodes and modifier encodings come from QMK itself.
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "quantum_keycodes.h"
#include "modifiers.h"

#define MATRIX_ROWS 4
#define MATRIX_COLS 12
typedef uint32_t layer_state_t;
extern layer_state_t default_layer_state;
typedef struct {
    struct {
        struct { uint8_t row, col; } key;
        bool pressed;
        bool synthetic;
    } event;
} keyrecord_t;
#define IS_KEYEVENT(event) (!(event).synthetic)
typedef struct { bool num_lock; } led_t;

uint8_t get_mods(void);
uint8_t get_weak_mods(void);
uint8_t get_oneshot_mods(void);
void clear_mods(void);
void clear_weak_mods(void);
void clear_oneshot_mods(void);
void set_mods(uint8_t value);
void set_weak_mods(uint8_t value);
void set_oneshot_mods(uint8_t value);
void send_keyboard_report(void);
led_t host_keyboard_led_state(void);
void register_code(uint8_t code);
void unregister_code(uint8_t code);
void register_code16(uint16_t code);
void unregister_code16(uint16_t code);
void tap_code16_delay(uint16_t code, uint16_t delay);
void wait_ms(uint16_t delay);
