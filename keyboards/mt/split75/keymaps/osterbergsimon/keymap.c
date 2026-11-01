// Copyright 2022 Artjoms Rizihs (@artjomsR)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Default keymap + an Fn layer on the small left space key (as in the 2020 tux_keymap):
// media on F1-F3, night-blue on F4, underglow/backlight on F5-F12, bootloader on Esc.

#include QMK_KEYBOARD_H

enum layer_names {
  BASE,
  FN
};

enum custom_keycodes {
  UG_NBLU = SAFE_RANGE  // static dim blue, matches the OpenRGB "night-blue" profile
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Base          ,-----------------------------------------.     ,-----------------------------------------------------.
  *                | ESC | F1  | F2  | F3  | F4  | F5  | F6  |     | F7  | F8  | F9  | F10 | F11 | F12 |Print| Ins | Del |
  * ,-----------.  |-----+-----+-----+-----+-----+-----+-----|     |-----+-----+-----+-----+-----+-----+-----------+-----|
  * |  8  |  9  |  |  ~  |  1  |  2  |  3  |  4  |  5  |  6  |     |  7  |  8  |  9  |  0  |  -  |  =  | Backspac  | Home|
  * |-----+-----|  |-----------------------------------------'  ,--------------------------------------------------+-----|
  * |  6  |  7  |  | Tab   |  Q  |  W  |  E  |  R  |  T  |     |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |    \    | PgUp|
  * |-----+-----|  |---------------------------------------.    `--------------------------------------------------+-----|
  * |  4  |  5  |  | Caps    |  A  |  S  |  D  |  F  |  G  |     |  H  |  J  |  K  |  L  |  ;  |  '  |    Enter    | PgDn|
  * |-----+-----|  |-----------------------------------------.   `-------------------------------------------------+-----|
  * |  2  |  3  |  | Shift     |  Z  |  X  |  C  |  V  |  B  |     |  N  |  M  |  ,  |  .  |   /   |  Shift  | Up  | End |
  * |-----+-----|  |-----------------------------------------'   ,-------------------------------------------+-----+-----|
  * |  0  |  1  |  | Ctrl  |  GUI |  Alt |   Space   | Fn  |     | Space         |  Alt  |     | Ctrl  | Left| Down|Right|
  * `-----------'  `---------------------------------------'     `-------------------------------------------------------'
  */
  [BASE] = LAYOUT(
//--------------------------------Left Hand-----------------------------------| |--------------------------------Right Hand------------------------------------------------
                KC_ESC,   KC_F1,   KC_F2,   KC_F3,  KC_F4,  KC_F5,  KC_F6,                KC_F7,  KC_F8,   KC_F9,  KC_F10,  KC_F11,   KC_F12,   KC_PSCR,  KC_INS,  KC_DEL,
KC_8,  KC_9,    KC_GRAVE, KC_1,    KC_2,    KC_3,   KC_4,   KC_5,   KC_6,                 KC_7,   KC_8,    KC_9,   KC_0,    KC_MINUS, KC_EQUAL, KC_BSPC,           KC_HOME,
KC_6,  KC_7,    KC_TAB,   KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,                 KC_Y,   KC_U,   KC_I,    KC_O,   KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS,           KC_PGUP,
KC_4,  KC_5,    KC_CAPS,  KC_A,    KC_S,    KC_D,   KC_F,   KC_G,                 KC_H,   KC_J,   KC_K,    KC_L,   KC_SCLN, KC_QUOT,  KC_ENTER,                    KC_PGDN,
KC_2,  KC_3,    KC_LSFT,  KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,                 KC_N,   KC_M,   KC_COMM, KC_DOT, KC_SLSH,                     KC_RSFT,  KC_UP,   KC_END,
KC_0,  KC_1,    KC_LCTL,  KC_LGUI, KC_LALT, KC_SPC, MO(FN),                       KC_SPC,         KC_RALT, _______,KC_RCTL,                     KC_LEFT,  KC_DOWN, KC_RIGHT
  ),

  /* Fn: Esc = bootloader, F1-F3 = mute/vol-/vol+, F4 = static dim blue (night-blue),
   *     F5 underglow on/off, F6/F7 next/prev mode, F8/F9 brightness -/+, F10/F11 hue -/+, F12 backlight on/off
   */
  [FN] = LAYOUT(
                QK_BOOT,  KC_MUTE, KC_VOLD, KC_VOLU, UG_NBLU, UG_TOGG, UG_NEXT,              UG_PREV, UG_VALD, UG_VALU, UG_HUED, UG_HUEU, BL_TOGG,  _______,  _______, _______,
_______, _______, _______, _______, _______, _______, _______, _______, _______,              _______, _______, _______, _______, _______, _______, _______,           _______,
_______, _______, _______, _______, _______, _______, _______, _______,              _______, _______, _______, _______, _______, _______, _______, _______,           _______,
_______, _______, _______, _______, _______, _______, _______, _______,              _______, _______, _______, _______, _______, _______, _______,                    _______,
_______, _______, _______, _______, _______, _______, _______, _______,              _______, _______, _______, _______, _______,                   _______, _______,  _______,
_______, _______, _______, _______, _______, _______, _______,                       _______,          _______, _______, _______,                   _______, _______,  _______
  )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case UG_NBLU:
      if (record->event.pressed) {
        // Saved to EEPROM so it survives replugging. V=48 (the Aura's 0x000030) was barely
        // visible on these LEDs; 136 = 8 brightness steps of 17
        rgblight_enable();
        rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
        rgblight_sethsv(170, 255, 136);  // hue 170 = blue
      }
      return false;
  }
  return true;
}

// Always boot into a static underglow mode. V-USB is bit-banged on this ATmega32A; a saved
// animation (EEPROM survives flashing) running from power-on starved USB enumeration
// (error -71) and bricked the board until the EEPROM was reset. Animations can still be
// picked at runtime; they just aren't restored at power-on.
void keyboard_post_init_user(void) {
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}
