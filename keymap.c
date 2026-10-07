/*
 * Sofle keymap - 2 layers
 *
 *   Layer 0 (BASE): normal QWERTY typing
 *   Layer 1 (NAV):  arrows, navigation, editing shortcuts, encoder = volume
 *                   (active while you hold the NAV thumb key)
 *
 * Shorthand used below (both are built into QMK):
 *   _______  = transparent: falls through to the key on the layer below
 *   XXXXXXX  = does nothing
 */

#include QMK_KEYBOARD_H

// Layer names, so the code says _NAV instead of a bare number.
enum layers {
    _BASE,  // 0
    _NAV,   // 1
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
 * BASE - layer 0
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | Esc  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------| Mute  |    | Next  |------+------+------+------+------+------|
 * | Ctrl |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |  -   |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | GUT  | Alt  | Ctrl | NAV  | /Space  /       \Enter \  |(none)|  [   |  ]   | RCtrl|
 *            `----------------------------------'           '------''---------------------------'
 * Mute / Next = what happens when you click the left / right encoder.
 */
[_BASE] = LAYOUT(
  KC_ESC,  KC_1, KC_2, KC_3, KC_4, KC_5,                      KC_6, KC_7, KC_8,    KC_9,   KC_0,    KC_BSPC,
  KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_T,                      KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_GRV,
  KC_LSFT, KC_A, KC_S, KC_D, KC_F, KC_G,                      KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
  KC_LCTL, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_MUTE,    KC_MNXT, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_MINS,
  //                                     ^ left        ^ right encoder clicks
       KC_LGUI, KC_LALT, KC_LCTL, MO(_NAV), KC_SPC,   KC_ENT, XXXXXXX, KC_LBRC, KC_RBRC, KC_RCTL
  //                              ^ hold for NAV layer          ^ unused (was the old "upper" layer key)
),

/*
 * NAV - layer 1 (hold the NAV thumb key)
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |  =   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | Ins  | PrtSc| Menu |      |      |                    | PgUp |C-Left|      |C-Rght|C-Bspc| Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | Alt  | Ctrl |Shift |      | Caps |-------.    ,-------| Left | Down | Up   | Rght | Del  | Bspc |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      | Undo | Cut  | Copy |Paste |      |-------|    |-------|      | Home |      | End  |  \ | |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      |      | /       /       \      \  |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 * Blank = transparent (same as BASE). C- = Ctrl+key (C-Left/C-Rght jump by word, C-Bspc deletes a word).
 * The left-hand Alt/Ctrl/Shift let you hold modifiers while using the arrows on the right.
 */
[_NAV] = LAYOUT(
  _______, _______,    _______,    _______,    _______,    _______,                       _______, _______,       _______, _______,       _______,       KC_EQL,
  _______, KC_INS,     KC_PSCR,    KC_APP,     XXXXXXX,    XXXXXXX,                       KC_PGUP, C(KC_LEFT),    XXXXXXX, C(KC_RGHT),    C(KC_BSPC),    KC_BSPC,
  _______, KC_LALT,    KC_LCTL,    KC_LSFT,    XXXXXXX,    KC_CAPS,                       KC_LEFT, KC_DOWN,       KC_UP,   KC_RGHT,       KC_DEL,        KC_BSPC,
  _______, C(KC_Z),    C(KC_X),    C(KC_C),    C(KC_V),    XXXXXXX, _______,     _______, XXXXXXX, KC_HOME,       XXXXXXX, KC_END,        KC_BSLS,       _______,
  //                                                                ^ encoder clicks stay Mute / Next
                _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
),
};

/*
 * Encoder rotation, per layer.
 * Each entry is ENCODER_CCW_CW(counter-clockwise, clockwise), left encoder first, then right.
 * To flip a direction, swap the two keycodes.
 */
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = { ENCODER_CCW_CW(MS_WHLU, MS_WHLD), ENCODER_CCW_CW(MS_WHLU, MS_WHLD) },  // mouse scroll up / down
    [_NAV]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },  // volume down / up
};
#endif
