# The default keymap for plaid

## Mac / Windows host modes

Both base layers use the same QCLPBK (Annie) layout, home-row modifiers,
combos, thumb keys, and French/code/number layers:

* **Layer 0 (ANNIE):** macOS, with the **French (AZERTY), ISO** input layout.
* **Layer 1 (ANNIE_PC):** Windows, with the classic **French (France)** AZERTY
  input layout, not French (Standard)/the newer AFNOR layout. This replaces
  the old QWERTY layer.

Hold both Space/Numbers and Enter/Code to open Adjust. The leftmost key on
the home row selects **Mac**; the leftmost key on the row below selects **PC**.
The choice is saved across unplugging/rebooting. The keyboard does not change
the computer's input layout: select the matching French layout on each host.

PC mode translates symbols on all layers, including `-`, `_`, `=`, `+`, `*`,
`!`, `@`, `#`, brackets, braces, backslash, pipe, and euro. It preserves the
custom shifted punctuation (`,` → `*`, `.` → `!`, `/` → `?`) and shifted
French characters (`é` → `É`, `œ` → `Œ`, `«` → `“`, `…` → `·`, etc.).
Letters, physical Ctrl/Alt/GUI positions, and combo outputs stay unchanged;
GUI means Command on Mac and Windows on PC, not an automatic Ctrl swap.

Characters missing from classic Windows AZERTY are entered with leading-zero
Alt+numpad codes (Windows-1252). Num Lock is temporarily enabled if necessary,
then restored. No WinCompose, Unicode registry setting, or other software is
needed. Applications must support Windows Alt codes; some terminals, remote
desktop clients, or non-Western Windows code pages may handle them differently.
These characters are emitted once per press, just like the Mac text macros.

Run the host-side translation tests with a local QMK checkout:

```sh
python3 layouts/ortho_4x12/gaelph/tests/run.py /path/to/qmk_firmware
```

## Original Plaid notes

Original copyright 2019 Takuya Urakawa (dm9records.com)
LED Support added  by Richard Sutherland (rich@brickbots.com)

This layout is based on the Planck layout, and includes an adjust layer (6)
accessible by holding the lower and raise modifiers (MO3 and MO4) together.
The adjustment layer is used to set the behavior of the two LEDs:

**Modifier Mode:**   
Activates when any modifier (shift, alt, os, MO) key is held
down.  LED turns off when key is release

**Blinkinlights Mode:**  
Random chance of state change on each keystroke.

**Keypress Mode:**  
On for any keypress as long as the key is pressed

**Carriage Mode:**  
Turns on when enter is pressed, turns off when any next key is pressed

To set the behavior of an LED, and save it to eeprom, hold the
raise/lower keys together to access the adjust layer, then use
the keys indicated below to set the behaviors

* q = Toggle Red LED state, deactivates any other modes
* w = Toggle Green LED state, deactivates any other modes
* e = Set RED LED to modifier mode
* r = Set GREEN LED to modifier mode
* t = Set RED LED to Blinkinlights mode
* y = set GREEN LED to Blinkinlights mode
* u = set RED LED to Keypress mode
* i = set GREEN LED to Keypress mode
* o = set RED LED to Carriage mode
* p = set GREEN LED to Carriage mode
