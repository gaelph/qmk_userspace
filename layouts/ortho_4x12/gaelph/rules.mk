# Allows to type keys with simultaneous presses
COMBO_ENABLE = yes
# For en/em dash
KEY_OVERRIDE_ENABLE = yes
# For HID/keystats
RAW_ENABLE = yes
# When some debugging is required
CONSOLE_ENABLE = no
MOUSEKEY_ENABLE = no
# Media keys such as vol+, vol- brightness and so on
EXTRAKEY_ENABLE = yes
DYNAMIC_TAPPING_TERM_ENABLE = yes
# TAP_DANCE_ENABLE = yes

# keystats
SRC += features/hid.c
SRC += features/plaid_led.c
# en/em dash
SRC += key_overrides.c
