#include QMK_KEYBOARD_H

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // clang-format off
    /*
    * Layer BASE
    * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬───────────┐
    * │  `  │  1  │  2  │  3  │  4  │  5  │  6  │  7  │  8  │  9  │  0  │  ─  │  =  │ Backspace │
    * ├─────┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬────────┤
    * │ Tab    │  q  │  w  │  e  │  r  │  t  │  y  │  u  │  i  │  o  │  p  │  [  │  ]  │   \    │
    * ├────────┴┬────┴┬────┴┬────┴┬────┴┬────┴┬────┴┬────┴┬────┴┬────┴┬────┴┬────┴┬────┴────────┤
    * │ Caps    │  a  │  s  │  d  │  f  │  g  │  h  │  j  │  k  │  l  │  ;  │  '  │    Enter    │
    * ├─────────┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴──┬──┴─────────────┤
    * │ Shift      │  z  │  x  │  c  │  v  │  b  │  n  │  m  │  ,  │  .  │  /  │     Shift      │
    * ├───────┬────┴──┬──┴────┬┴─────┴─────┴─────┴─────┴─────┴──┬──┴────┬┴─────┴┬───────┬───────┤
    * │ Ctrl  │  GUI  │  Alt  │               Space             │ Left  │ Down  │  Up   │ Right │
    * └───────┴───────┴───────┴─────────────────────────────────┴───────┴───────┴───────┴───────┘
    */
    [0] = LAYOUT_60_ansi(
        KC_GRV,  KC_1,         KC_2,         KC_3,         KC_4,         KC_5,    KC_6,    KC_7,            KC_8,           KC_9,            KC_0,            KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,  KC_Q,         KC_W,         KC_E,         KC_R,         KC_T,    KC_Y,    KC_U,            KC_I,           KC_O,            KC_P,            KC_LBRC, KC_RBRC, KC_BSLS,
        KC_ESC,  LGUI_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F), KC_G,    KC_H,    RSFT_T(KC_J),    RCTL_T(KC_K),   LALT_T(KC_L),    RGUI_T(KC_SCLN), KC_QUOT, KC_ENT,
        KC_LSFT, KC_Z,         KC_X,         KC_C,         KC_V,         KC_B,    KC_N,    KC_M,            KC_COMM,        KC_DOT,          KC_SLSH,         KC_RSFT,
        KC_LCTL, KC_LGUI,      KC_LALT,                                           LT(2, KC_SPC),                                             KC_LEFT,         KC_DOWN, KC_UP,   KC_RGHT
    ),
    [2] = LAYOUT_60_ansi(
        KC_F12 , KC_F1,        KC_F2,        KC_F3,        KC_F4,        KC_F5,   KC_F6,   KC_F7,           KC_F8,          KC_F9,           KC_F10,          KC_F11,  KC_F12 , KC_DEL,
        _______, _______,      _______,      _______,      _______,      _______, _______, _______,         _______,        _______,         KC_PSCR,         _______, _______, _______,
        _______, KC_LGUI,      KC_LALT,      KC_LCTL,      KC_LSFT,      _______, _______, RSFT_T(KC_MINS), RCTL_T(KC_EQL), LALT_T(KC_LBRC), RGUI_T(KC_RBRC), KC_BSLS, _______,
        _______, _______,      _______,      _______,      _______,      _______, _______, _______,         _______,        _______,         _______,         _______,
        RGB_TOG, RGB_MOD,      _______,                                           _______,                                                   _______,         _______, _______, _______
    ),
    // clang-format on
};
