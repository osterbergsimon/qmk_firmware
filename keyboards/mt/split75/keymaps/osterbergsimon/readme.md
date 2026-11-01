# Split75 — osterbergsimon

YMDK Split75 (`mt/split75`, ATmega32A, bootloadHID/V-USB). Mainline default keymap plus one
Fn layer on the small key right of the left space bar. Successor to the 2020 `tux_keymap`
(still on this fork's `master` branch, untouched): only the media keys were kept, lighting keys added.

## Base layer

```
               ,-----------------------------------------.     ,-----------------------------------------------------.
               | Esc | F1  | F2  | F3  | F4  | F5  | F6  |     | F7  | F8  | F9  | F10 | F11 | F12 |PrScr| Ins | Del |
,-----------.  |-----+-----+-----+-----+-----+-----+-----|     |-----+-----+-----+-----+-----+-----+-----------+-----|
|  8  |  9  |  |  `  |  1  |  2  |  3  |  4  |  5  |  6  |     |  7  |  8  |  9  |  0  |  -  |  =  | Backspace | Home|
|-----+-----|  |-----------------------------------------'  ,--------------------------------------------------+-----|
|  6  |  7  |  | Tab   |  Q  |  W  |  E  |  R  |  T  |     |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |    \    | PgUp|
|-----+-----|  |---------------------------------------.    `--------------------------------------------------+-----|
|  4  |  5  |  | Caps    |  A  |  S  |  D  |  F  |  G  |     |  H  |  J  |  K  |  L  |  ;  |  '  |    Enter    | PgDn|
|-----+-----|  |-----------------------------------------.   `-------------------------------------------------+-----|
|  2  |  3  |  | Shift     |  Z  |  X  |  C  |  V  |  B  |     |  N  |  M  |  ,  |  .  |   /   |  Shift  | Up  | End |
|-----+-----|  |-----------------------------------------'   ,-------------------------------------------+-----+-----|
|  0  |  1  |  | Ctrl  |  GUI |  Alt |   Space   | Fn  |     | Space         |  Alt  |     | Ctrl  | Left| Down|Right|
`-----------'  `---------------------------------------'     `-------------------------------------------------------'
```

## Fn layer (hold Fn)

| Key | Action |
|-----|--------|
| Esc | **Bootloader** (for flashing) |
| F1 / F2 / F3 | Mute / Volume down / Volume up |
| F4 | Underglow **static dim blue** (matches the OpenRGB `night-blue` profile; saved on the keyboard) |
| F5 | Underglow on/off |
| F6 / F7 | Underglow next / previous mode |
| F8 / F9 | Underglow brightness down / up |
| F10 / F11 | Underglow hue down / up |
| F12 | Key backlight on/off |

Everything else is transparent (behaves as the base layer).

## Build and flash

```sh
cd ~/code/qmk_firmware
qmk compile -kb mt/split75 -km osterbergsimon     # build only
qmk flash   -kb mt/split75 -km osterbergsimon     # build, wait for bootloader, flash
```

Then press **Fn + Esc** to enter the bootloader. If the firmware is broken, unplug, hold
**Left Ctrl** (3rd key from the left on the bottom row) and plug back in instead.

On NixOS (dotfiles `eclipse`), `qmk` and its udev rules (`hardware.keyboard.qmk.enable`) are
installed, so no sudo is needed. Without the udev rules, flash the hex manually:
`sudo bootloadHID -r mt_split75_osterbergsimon.hex`.

## Gotchas (learned the hard way, 2026-11-01)

- **Underglow always boots in static mode.** Animations can be picked at runtime but are not
  restored at power-on: on this bit-banged V-USB chip a saved animation running from power-on
  starved USB enumeration (`usb …: device not accepting address, error -71`) and the board looked
  bricked. Flashing does not clear the saved setting (EEPROM survives).
- **If it's dead anyway:** flash the `recover` keymap (`qmk compile -kb mt/split75 -km recover`).
  It wipes EEPROM and keeps underglow off on every boot; once the board is alive, flash this
  keymap again.
- **Flashing is flaky on USB 3 ports** (`Communication error with device` partway through). Just
  rerun — the board stays in the bootloader. The case front ports worked best. Never run two
  flashers at once (e.g. an old `until lsusb … bootloadHID` loop still waiting in another
  terminal); interleaved writes leave a half-written firmware.
- `bootloadHID -r` often fails to restart the board after flashing; unplug/replug (no keys held).
