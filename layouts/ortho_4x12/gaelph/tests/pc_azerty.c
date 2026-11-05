#include <assert.h>
#include <stdio.h>
#include "keyboard.h"
#include "../defs.h"
#include "../custom_keycodes.h"
#include "../features/pc_azerty.h"

layer_state_t default_layer_state;
static uint8_t mods, weak_mods, oneshot_mods;
static bool num_lock = true;
static struct { char type; uint16_t code; uint8_t mods; } events[32];
static unsigned count;

uint8_t get_mods(void) { return mods; }
uint8_t get_weak_mods(void) { return weak_mods; }
uint8_t get_oneshot_mods(void) { return oneshot_mods; }
void clear_mods(void) { mods = 0; }
void clear_weak_mods(void) { weak_mods = 0; }
void clear_oneshot_mods(void) { oneshot_mods = 0; }
void set_mods(uint8_t value) { mods = value; }
void set_weak_mods(uint8_t value) { weak_mods = value; }
void set_oneshot_mods(uint8_t value) { oneshot_mods = value; }
void send_keyboard_report(void) {}
led_t host_keyboard_led_state(void) { return (led_t){num_lock}; }
void wait_ms(uint16_t delay) { (void)delay; }

static void log_event(char type, uint16_t code) {
    assert(count < sizeof(events) / sizeof(events[0]));
    events[count].type = type;
    events[count].code = code;
    events[count++].mods = mods | weak_mods | oneshot_mods;
}
void register_code16(uint16_t code) { log_event('D', code); }
void unregister_code16(uint16_t code) { log_event('U', code); }
void register_code(uint8_t code) {
    assert(code == KC_LALT);
    mods |= MOD_BIT_LALT;
    log_event('D', code);
}
void unregister_code(uint8_t code) {
    assert(code == KC_LALT);
    log_event('U', code);
    mods &= ~MOD_BIT_LALT;
}
void tap_code16_delay(uint16_t code, uint16_t delay) {
    (void)delay;
    log_event('T', code);
    if (code == KC_NUM_LOCK) { num_lock = !num_lock; }
}

static bool key(uint16_t code, bool pressed) {
    keyrecord_t record = {.event = {.key = {1, 1}, .pressed = pressed}};
    return process_pc_azerty(code, &record);
}

static void native(uint16_t input, uint16_t output) {
    count = 0;
    assert(!key(input, true));
    assert(count == 1 && events[0].type == 'D' && events[0].code == output);
    assert(!key(input, false));
    assert(count == 2 && events[1].type == 'U' && events[1].code == output);
}

static void alt(uint16_t input, uint8_t expected) {
    count = 0;
    uint8_t saved_mods = mods, saved_weak = weak_mods, saved_oneshot = oneshot_mods;
    bool saved_num_lock = num_lock;
    assert(!key(input, true));
    assert(mods == saved_mods && weak_mods == saved_weak && oneshot_mods == saved_oneshot);
    assert(num_lock == saved_num_lock);
    unsigned offset = saved_num_lock ? 0 : 1;
    assert(count == (saved_num_lock ? 6 : 8));
    assert(events[offset].type == 'D' && events[offset].code == KC_LALT);
    unsigned value = 0;
    for (unsigned i = offset + 1; i < offset + 5; ++i) {
        assert(events[i].type == 'T');
        assert(events[i].mods == MOD_BIT_LALT);
        uint16_t code = events[i].code;
        assert(code == KC_KP_0 || (code >= KC_KP_1 && code <= KC_KP_9));
        unsigned digit = code == KC_KP_0 ? 0 : code - KC_KP_1 + 1;
        if (i == offset + 1) { assert(digit == 0); }
        value = value * 10 + digit;
    }
    assert(value == expected);
    unsigned before_release = count;
    mods = 0; // Releasing Shift before the character must not change key-up.
    assert(!key(input, false));
    assert(count == before_release);
    mods = saved_mods;
}

static void shifted(uint16_t input, uint16_t output) {
    count = 0;
    mods = MOD_BIT_RSHIFT;
    assert(!key(input, true));
    assert(count == 1 && events[0].type == 'T' && events[0].code == output);
    assert(events[0].mods == 0 && mods == MOD_BIT_RSHIFT);
    mods = 0;
    assert(!key(input, false));
    assert(count == 1);
}

int main(void) {
    default_layer_state = 1 << _ANNIE;
    assert(key(FR_MINS, true) && key(FR_MINS, false));
    assert(key(FR_OE, true) && key(FR_OE, false));
    assert(count == 0); // Mac mode is untouched.

    default_layer_state = 1 << _ANNIE_PC;
    assert(key(FR_Q, true) && key(FR_Q, false));
    assert(key(LCTL_A, true) && key(LCTL_A, false));
    assert(key(NM_SPC, true) && key(NM_SPC, false));
    assert(key(MO(_FRENCH), true) && key(MO(_FRENCH), false));
    assert(key(FR_1, true) && key(FR_1, false));

    const uint16_t native_pairs[][2] = {
        {FR_MINS, KC_6}, {FR_UNDS, KC_8}, {FR_EQL, KC_EQL}, {FR_PLUS, S(KC_EQL)},
        {FR_EXLM, KC_SLSH}, {FR_ASTR, KC_NUHS}, {FR_AT, RALT(KC_0)},
        {FR_HASH, RALT(KC_3)}, {FR_LCBR, RALT(KC_4)}, {FR_LBRC, RALT(KC_5)},
        {FR_RBRC, RALT(KC_MINS)}, {FR_RCBR, RALT(KC_EQL)}, {FR_PIPE, RALT(KC_6)},
        {FR_BSLS, RALT(KC_8)}, {FR_EURO, RALT(KC_E)}, {FR_GRV, RALT(KC_7)},
        {FR_COMM, FR_COMM}, {FR_DOT, FR_DOT}, {FR_SLSH, FR_SLSH},
        {FR_LEAC, FR_LEAC}, {FR_LEGR, FR_LEGR}, {FR_LCCE, FR_LCCE},
        {FR_LAGR, FR_LAGR}, {FR_LUGR, FR_LUGR},
    };
    for (unsigned i = 0; i < sizeof(native_pairs) / sizeof(native_pairs[0]); ++i) {
        native(native_pairs[i][0], native_pairs[i][1]);
    }
    shifted(FR_COMM, KC_NUHS);
    shifted(FR_DOT, KC_SLSH);
    shifted(FR_SLSH, S(KC_M));
    shifted(FR_MINS, KC_8);
    shifted(FR_UNDS, KC_8);
    shifted(FR_ASTR, KC_NUHS);
    shifted(FR_AT, RALT(KC_3));
    shifted(FR_HASH, RALT(KC_3));
    shifted(FR_LCBR, RALT(KC_5));
    shifted(FR_LBRC, RALT(KC_5));
    shifted(FR_RCBR, RALT(KC_MINS));
    shifted(FR_RBRC, RALT(KC_MINS));
    shifted(FR_PIPE, RALT(KC_6));
    shifted(FR_BSLS, RALT(KC_8));
    mods = MOD_BIT_RSHIFT;
    native(FR_GRV, S(KC_RBRC));
    native(FR_EXLM, S(KC_8));
    alt(FR_EURO, 165);
    mods = MOD_BIT_LCTRL | MOD_BIT_LALT | MOD_BIT_LSHIFT;
    native(FR_COMM, KC_M);
    native(FR_DOT, KC_DOT);
    native(FR_SLSH, KC_SLSH);

    const uint16_t alt_pairs[][3] = {
        {__CIRC, 94, 94}, {__GRV, 96, 96}, {__TILD, 126, 126},
        {FR_OE, 156, 140}, {FR_AE, 230, 198}, {FR_ECIR, 234, 202},
        {FR_LSQU, 145, 146}, {FR_LRQU, 146, 146}, {FR_LDAQ, 171, 147},
        {FR_RDAQ, 187, 148}, {FR_ELLP, 133, 183}, {FR_NDSH, 150, 151},
        {FR_MDSH, 151, 151}, {RALT(KC_SPACE), 160, 160},
    };
    for (unsigned i = 0; i < sizeof(alt_pairs) / sizeof(alt_pairs[0]); ++i) {
        mods = 0;
        alt(alt_pairs[i][0], alt_pairs[i][1]);
        mods = MOD_BIT_RSHIFT;
        alt(alt_pairs[i][0], alt_pairs[i][2]);
    }
    mods = MOD_BIT_LSHIFT;
    alt(FR_LEAC, 201);
    alt(FR_LEGR, 200);
    alt(FR_LCCE, 199);
    alt(FR_LAGR, 192);
    alt(FR_LUGR, 217);

    num_lock = false;
    mods = MOD_BIT_RSHIFT | MOD_BIT_LCTRL;
    weak_mods = MOD_BIT_RALT;
    oneshot_mods = MOD_BIT_RGUI;
    alt(FR_OE, 140); // Modifier and Num Lock restoration.
    mods = weak_mods = oneshot_mods = 0;

    count = 0;
    assert(!key(FR_LBRC, true));
    keyrecord_t combo = {.event = {.key = {1, 1}, .pressed = false, .synthetic = true}};
    assert(process_pc_azerty(LGUI(KC_U), &combo)); // Not a physical release.
    default_layer_state = 1 << _ANNIE;
    assert(!key(FR_LBRC, false));
    assert(count == 2 && events[1].code == RALT(KC_5));

    puts("PC AZERTY tests passed");
}
