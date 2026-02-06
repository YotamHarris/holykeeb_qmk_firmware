#include QMK_KEYBOARD_H
#include <stdio.h>
#include "users/holykeebs/holykeebs.h"

#define QK_C_EEPROM QK_CLEAR_EEPROM

enum layer_number {
  _QWERTY = 0,
  _LOWER,
  _RAISE,
  _ADJUST,
};

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

 [_QWERTY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV,
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS,
  KC_LCTL,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_LBRC,  KC_RBRC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,
                      LOWER, KC_LGUI,  KC_LALT, KC_SPC,   KC_ENT,   KC_BSPC,  KC_RGUI, RAISE
),

[_LOWER] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                        _______, _______, _______,_______, _______, _______,
  _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                        KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
  _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                           KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
  _______, HK_D_MODE, KC_BTN4, KC_BTN5, KC_BTN1, KC_BTN2, HK_S_MODE,  _______, KC_PIPE, KC_GRAVE, KC_PLUS, KC_LCBR, KC_RCBR, _______,
                             _______, _______, _______, _______,      _______,  _______, _______, _______
),

[_RAISE] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                     _______, _______, _______, _______, _______, _______,
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
  _______, _______, _______, _______, _______, _______,                     XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
  _______, _______, _______, _______, _______, _______,   _______, _______, KC_PLUS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, _______,
                             _______, _______, _______,  _______, _______,  _______, _______, _______
),

  [_ADJUST] = LAYOUT(
    XXXXXXX,      XXXXXXX,    XXXXXXX,    XXXXXXX,      XXXXXXX, XXXXXXX,                        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    QK_BOOT,      HK_DUMP,    HK_SAVE,    HK_RESET,     XXXXXXX, HK_C_SCROLL,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
    QK_C_EEPROM,  HK_P_SET_D, HK_P_SET_S, HK_P_SET_BUF, XXXXXXX, HK_S_MODE_T,                    KC_UP,   KC_DOWN, XXXXXXX, XXXXXXX, XXXXXXX, QK_C_EEPROM,
    KC_LSFT,      XXXXXXX,    XXXXXXX,    XXXXXXX,      XXXXXXX, HK_D_MODE_T, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                   _______, _______, _______, _______,  _______, _______, _______, _______
  )
};

layer_state_t layer_state_set_user(layer_state_t state) {
  state = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
  return state;
}

#if defined(OLED_ENABLE) && !defined(HK_OLED_ENABLE)

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master())
    return OLED_ROTATION_270;
  return rotation;
}

// Hamsa Logo - 32x48 raw bitmap for vertical OLED (32w x 128h with ROTATION_270)
static void render_logo(void) {
    static const char PROGMEM hamsa[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0,
        0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xF8, 0xE0, 0x00, 0x00, 0x00, 0x00, 0xFF,
        0xFF, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x38, 0x70, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF,
        0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0x70, 0x38, 0x00,
        0x60, 0x60, 0x60, 0x71, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x30, 0x7F,
        0x7F, 0x30, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x71, 0x70, 0x70, 0x70,
        0x00, 0x00, 0x30, 0x38, 0x1D, 0x1F, 0x1E, 0x38, 0x70, 0xE0, 0xC1, 0xC3, 0x87, 0x8E, 0x8C, 0x8C,
        0x8C, 0x8C, 0x8E, 0x87, 0xC3, 0xC1, 0xE0, 0x70, 0x38, 0x1E, 0x0F, 0x1D, 0x38, 0x30, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x07, 0x07, 0x01, 0x00, 0x01, 0x01, 0x01, 0x1F,
        0x1F, 0x01, 0x01, 0x01, 0x00, 0x01, 0x07, 0x07, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    oled_write_raw_P(hamsa, sizeof(hamsa));
}

bool oled_task_user(void) {
  if (is_keyboard_master()) {
    oled_write_ln_P(PSTR("Layer"), false);

    switch (get_highest_layer(layer_state)) {
    case _QWERTY:
        oled_write_ln_P(PSTR("Base"), false);
        break;
    case _RAISE:
        oled_write_ln_P(PSTR("Raise"), false);
        break;
    case _LOWER:
        oled_write_ln_P(PSTR("Lower"), false);
        break;
    case _ADJUST:
        oled_write_ln_P(PSTR("Adjst"), false);
        break;
    default:
        oled_write_ln_P(PSTR("?????"), false);
    }
  } else {
      render_logo();
      // Layer text below logo
      oled_set_cursor(0, 7);
      switch (get_highest_layer(layer_state)) {
      case _QWERTY:
          oled_write_ln_P(PSTR("Base"), false);
          break;
      case _RAISE:
          oled_write_ln_P(PSTR("Raise"), false);
          break;
      case _LOWER:
          oled_write_ln_P(PSTR("Lower"), false);
          break;
      case _ADJUST:
          oled_write_ln_P(PSTR("Adjst"), false);
          break;
      default:
          oled_write_ln_P(PSTR("?????"), false);
          break;
      }
  }
    return false;
}

#endif // OLED_ENABLE
