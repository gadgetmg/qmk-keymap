#include QMK_KEYBOARD_H
#include "features/achordion.h"

enum layers {
  BASE,
  SYM,
  NAV,
  CFG,
};

#define HOME_A LGUI_T(KC_A)
#define HOME_S LALT_T(KC_S)
#define HOME_D LCTL_T(KC_D)
#define HOME_F LSFT_T(KC_F)
#define HOME_J RSFT_T(KC_J)
#define HOME_K RCTL_T(KC_K)
#define HOME_L LALT_T(KC_L)
#define HOME_SC RGUI_T(KC_SCLN)
#define SPACEFN LT(SYM, KC_SPC)

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }
  return true;
}

void matrix_scan_user(void) {
  achordion_task();
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    // Increase TAPPING_TERM for home row weaker fingers
    case HOME_S:
    case HOME_L:
      return TAPPING_TERM + 50;
    case HOME_A:
    case HOME_SC:
      return TAPPING_TERM + 100;
    default:
      return TAPPING_TERM;
  }
}

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                     uint16_t other_keycode, keyrecord_t* other_record) {
  // Also allow same-hand holds when the other key is in the rows outside the
  // alphas.
  uint8_t row = other_record->event.key.row;
  if (!(1 <= row && row <= 3)) { return true; }

  switch (tap_hold_keycode) {
    // Exclude mod-taps on space from opposite hand restriction
    case SPACEFN:
        return true;
        break;
  }

  return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  switch (tap_hold_keycode) {
    default:
      return 1000;  // Use a timeout of 1000 ms.
  }
}

bool achordion_eager_mod(uint8_t mod) {
  switch (mod) {
    case MOD_LSFT:
    case MOD_RSFT:
    case MOD_LCTL:
    case MOD_RCTL:
      return true;  // Eagerly apply Shift and Ctrl mods.

    default:
      return false;
  }
}

uint16_t achordion_streak_chord_timeout(uint16_t tap_hold_keycode, uint16_t next_keycode) {
  if (IS_QK_LAYER_TAP(tap_hold_keycode)) {
    return 0;  // Disable streak detection on layer-tap keys.
  }

  // Otherwise, tap_hold_keycode is a mod-tap key.
  uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(tap_hold_keycode));
  if ((mod & MOD_LSFT) != 0) {
    return 100;  // A shorter streak timeout for Shift mod-tap keys.
  } else {
    return 250;  // A longer timeout otherwise.
  }
}

bool achordion_streak_continue(uint16_t keycode) {
  // If mods other than shift or AltGr are held, don't continue the streak.
  if (get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) return false;
  // This function doesn't get called for holds, so convert to tap keycodes.
  if (IS_QK_MOD_TAP(keycode)) {
    keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
  }
  if (IS_QK_LAYER_TAP(keycode)) {
    keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
  }
  // Regular letters and punctuation continue the streak.
  if (keycode >= KC_A && keycode <= KC_Z) return true;
  switch (keycode) {
    case KC_DOT:
    case KC_COMMA:
    case KC_QUOTE:
    case KC_SEMICOLON:
    case KC_SPACE:
      return true;
  }
  return false;  // All other keys end the streak.
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* QWERTY base
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │ ` │ 1 │ 2 │ 3 │ 4 │ 5 │Num│ / │ * │ 6 │ 7 │ 8 │ 9 │ 0 │ - │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │Esc│ Q │ W │ E │ R │ T │ 7 │ 8 │ 9 │ Y │ U │ I │ O │ P │Bsp│
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │Tab│ A │ S │ D │ F │ G │ 4 │ 5 │ 6 │ H │ J │ K │ L │ ; │ ' │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │Sft│ Z │ X │ C │ V │ B │ 1 │ 2 │ 3 │ N │ M │ , │ . │ / │Ent│
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │Ctl│Gui│Alt│Spc│   │   │Del│ 0 │ . │   │Spc│Lft│Dwn│Up │Rgt│
     * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
     * Hold modifiers
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │   │   │   │   │   │   │   │   │   │   │   │   │   │   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │   │   │   │   │   │   │   │   │   │   │   │   │   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │Gui│Alt│Ctl│Sft│   │   │   │   │   │Sft│Ctl│Alt│Gui│   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │   │   │   │   │   │   │   │   │   │   │   │   │   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │   │   │   │Sym│Nav│   │   │   │Nav│Sym│   │   │   │   │
     * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
     */
    [BASE] = LAYOUT_ortho_5x15(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_NUM,  KC_PSLS, KC_PAST, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
        KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_KP_7, KC_KP_8, KC_KP_9, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_TAB,  HOME_A,  HOME_S,  HOME_D,  HOME_F,  KC_G,    KC_KP_4, KC_KP_5, KC_KP_6, KC_H,    HOME_J,  HOME_K,  HOME_L,  HOME_SC, KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_KP_1, KC_KP_2, KC_KP_3, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
        KC_LCTL, KC_LGUI, KC_LALT, XXXXXXX, SPACEFN, TT(NAV), KC_DEL,  KC_KP_0, KC_PDOT, TT(NAV), SPACEFN, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
    ),

    /* Symbol layer
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │F12│F1 │F2 │F3 │F4 │F5 │   │   │   │F6 │F7 │F8 │F9 │F11│F12│
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │   │   │   │   │   │   │   │   │   │   │   │   │Psc│Del│
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │Gui│Alt│Ctl│Sft│   │   │   │   │   │ - │ = │ [ │ ] │ \ │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │ ^ │   │   │   │   │   │   │   │   │   │   │   │   │   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │ ^ │ ^ │ ^ │ ^ │Cfg│   │   │   │   │   │Cfg│   │   │   │   │
     * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
     */
    [SYM] = LAYOUT_ortho_5x15(
        KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   XXXXXXX, XXXXXXX, XXXXXXX, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PSCR, KC_DEL,
        XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_NO,
        _______, _______, _______, _______, MO(CFG), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MO(CFG), XXXXXXX, XXXXXXX, XXXXXXX, KC_NO
    ),

    /* Navigation layer
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │   │   │Num│ / │ * │Bsp│   │   │   │   │   │   │   │   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │ ^ │   │ 7 │ 8 │ 9 │ - │   │   │   │Hom│PgD│PgU│End│   │ ^ │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │ ^ │   │ 4 │ 5 │ 6 │ + │   │   │   │Lft│Dwn│Up │Rgt│   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │ ^ │   │ 1 │ 2 │ 3 │ = │   │   │   │   │   │   │   │   │ ^ │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │ ^ │ ^ │ ^ │ 0 │ . │Ent│   │   │   │Bas│Spc│ ^ │ ^ │ ^ │ ^ │
     * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
     */
    [NAV] = LAYOUT_ortho_5x15(
        XXXXXXX, XXXXXXX, KC_NUM,  KC_PSLS, KC_PAST, KC_BSPC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_NO,
        _______, XXXXXXX, KC_KP_7, KC_KP_8, KC_KP_9, KC_PMNS, XXXXXXX, XXXXXXX, XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX, _______,
        _______, XXXXXXX, KC_KP_4, KC_KP_5, KC_KP_6, KC_PPLS, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, KC_NO,
        _______, XXXXXXX, KC_KP_1, KC_KP_2, KC_KP_3, KC_PEQL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
        _______, _______, _______, KC_KP_0, KC_PDOT, KC_PENT, XXXXXXX, XXXXXXX, XXXXXXX, TG(NAV), KC_SPC,  _______, _______, _______, _______
    ),

    /* Config layer
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │   │   │   │   │   │   │   │   │   │   │   │   │   │   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │   │   │   │   │   │   │   │   │Hu-│Sa-│Sa+│Hu+│   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │   │   │   │   │   │   │   │   │Mo<│Br-│Br+│Mo>│   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │   │   │   │   │RGB│   │   │   │   │   │   │   │   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │   │   │   │ ^ │   │   │   │   │   │ ^ │   │   │   │   │
     * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
     */
    [CFG] = LAYOUT_ortho_5x15(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_NO,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUD, RGB_SAD, RGB_SAI, RGB_HUI, XXXXXXX, KC_NO,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_RMOD,RGB_VAD, RGB_VAI, RGB_MOD, XXXXXXX, KC_NO,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_SPD, RGB_SPI, XXXXXXX, XXXXXXX, KC_NO,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, KC_NO
    )
};
