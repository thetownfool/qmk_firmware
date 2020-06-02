#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif


#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
    _BEAKL = 0,
    _SHIFTED,
    _SYMBOLS,
    _MODS,
    _LOWER,
    _RAISE,
    _ADJUST
};

enum custom_keycodes {
  LT_LL = SAFE_RANGE,
  LT_LR,
  LT_RL,
  LT_RR,
  RGBRST
};

enum macro_keycodes {
  KC_SAMPLEMACRO,
};

#define CALTDEL LCTL(LALT(KC_DEL))
#define SF_SLSH S(KC_SLSH)
#define SF_COMM S(KC_COMM)
#define SF_DOT S(KC_DOT)
#define LT_TAP_LL KC_ENT
#define LT_TAP_LR KC_ENT
#define LT_TAP_RL KC_ENT
#define LT_TAP_RR KC_ENT
#define LT_LAY_LL _MODS
#define LT_LAY_LR _SYMBOLS
#define LT_LAY_RL _SYMBOLS
#define LT_LAY_RR _SHIFTED


bool is_ll_active = false;
uint16_t ll_timer = 0;
bool is_lr_active = false;
uint16_t lr_timer = 0;
bool is_rl_active = false;
uint16_t rl_timer = 0;
bool is_rr_active = false;
uint16_t rr_timer = 0;

char layertext[16] = "Layout: BEAKL";

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* BEAKL
   * ,-----------------------------------------------.               ,-----------------------------------------------.
   * |       |       |       |       |       |       |               |       |       |       |       |       |       |
   * |-------+-------+-------+-------+-------+-------|               |-------+-------+-------+-------+-------+-------|
   * |       |   Q   |   H   |   O   |   U   |   X   |               |   G   |   C   |   R   |   F   |   Z   |       |
   * |-------+-------+-------+-------+-------+-------|               |-------+-------+-------+-------+-------+-------|
   * |       |   Y   |   I   |   E   |   A   |   .   |               |   D   |   S   |   T   |   N   |   B   |       |
   * |-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------|
   * |       |   J   |   /   |   ,   |   K   |   '   |       |       |   W   |   M   |   L   |   P   |   V   |       |
   * |-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------|
   * |       |       |       |       |   LL  |  LR   | Space | BckSpc|  RL   |   RR  |       |       |       |       |
   * `---------------------------------------------------------------------------------------------------------------'
   */
  [_BEAKL] = LAYOUT( \
      _______,_______,_______,_______,_______,_______,                _______,_______,_______,_______,_______,_______, \
      _______,   KC_Q,   KC_H,   KC_O,   KC_U,   KC_X,                   KC_G,   KC_C,   KC_R,   KC_F,   KC_Z,_______, \
      _______,   KC_Y,   KC_I,   KC_E,   KC_A, KC_DOT,                   KC_D,   KC_S,   KC_T,   KC_N,   KC_B,_______, \
      _______,   KC_J,KC_SLSH,KC_COMM,   KC_K,KC_QUOT,_______,_______,   KC_W,   KC_M,   KC_L,   KC_P,   KC_V,_______, \
      _______,_______,_______,_______,  LT_LL,  LT_LR, KC_SPC,KC_BSPC,  LT_RL,  LT_RR,_______,_______,_______,_______ \
      ),

  /* Shifted
   * ,-----------------------------------------------.               ,-----------------------------------------------.
   * |       |       |       |       |       |       |               |       |       |       |       |       |       |
   * |-------+-------+-------+-------+-------+-------|               |-------+-------+-------+-------+-------+-------|
   * |       | SFT_Q | SFT_H | SFT_O | SFT_U | SFT_X |               | SFT_G | SFT_C | SFT_R | SFT_F | SFT_Z |       |
   * |-------+-------+-------+-------+-------+-------|               |-------+-------+-------+-------+-------+-------|
   * |       | SFT_Y | SFT_I | SFT_E | SFT_A |   @   |               | SFT_D | SFT_S | SFT_T | SFT_N | SFT_B |       |
   * |-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------|
   * |       | SFT_J |   ?   |   !   | SFT_K |   `   |       |       | SFT_W | SFT_M | SFT_L | SFT_P | SFT_V |       |
   * |-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------|
   * |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
   * `---------------------------------------------------------------------------------------------------------------'
   */
  [_SHIFTED] = LAYOUT( \
      _______,_______,_______,_______,_______,_______,                _______,_______,_______,_______,_______,_______, \
      _______,S(KC_Q),S(KC_H),S(KC_O),S(KC_U),S(KC_X),                S(KC_G),S(KC_C),S(KC_R),S(KC_F),S(KC_Z),_______, \
      _______,S(KC_Y),S(KC_I),S(KC_E),S(KC_A),S(KC_2),                S(KC_D),S(KC_S),S(KC_T),S(KC_N),S(KC_B),_______, \
      _______,S(KC_J),SF_SLSH,S(KC_1),S(KC_K), KC_GRV,_______,_______,S(KC_W),S(KC_M),S(KC_L),S(KC_P),S(KC_V),_______, \
      _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______ \
      ),

  /* symbols
   * ,-----------------------------------------------.               ,-----------------------------------------------.
   * |       |       |       |       |       |       |               |       |       |       |       |       |       |
   * |-------+-------+-------+-------+-------+-------|               |-------+-------+-------+-------+-------+-------|
   * |       |       |   <   |   $   |   >   |       |               |       |   [   |   _   |   ]   |       |       |
   * |-------+-------+-------+-------+-------+-------|               |-------+-------+-------+-------+-------+-------|
   * |       |   \   |   (   |   "   |   )   |   #   |               |   %   |   {   |   =   |   }   |   |   |       |
   * |-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------|
   * |       |       |   :   |   *   |   +   |       |       |       |       |   &   |   ^   |   ~   |       |       |
   * |-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------|
   * |       |       |       |       | Ctrl  | Lower | Space |BackSpc| Raise |       |       |       |       |       |
   * `---------------------------------------------------------------------------------------------------------------'
   */
  [_SYMBOLS] = LAYOUT( \
      _______,_______,_______,_______,_______,_______,                _______,_______,_______,_______,_______,_______, \
      _______,_______,SF_COMM,S(KC_4), SF_DOT,_______,                _______,KC_LBRC,KC_MINS,KC_RBRC,_______,_______, \
      _______,KC_BSLS,KC_LPRN,S(KC_E),KC_RPRN,S(KC_2),                S(KC_D),S(KC_S),S(KC_T),S(KC_N),S(KC_B),_______, \
      _______,_______,SF_SLSH,S(KC_1),S(KC_K),_______,_______,_______,_______,S(KC_M),S(KC_L),S(KC_P),_______,_______, \
      _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______ \
      ),

  /* mods
   * ,-----------------------------------------------.               ,-----------------------------------------------.
   * |       |       |       |       |       |       |               |       |       |       |       |       |       |
   * |-------+-------+-------+-------+-------+-------|               |-------+-------+-------+-------+-------+-------|
   * |       | SFT_Q | SFT_H | SFT_O | SFT_U | SFT_X |               |  F1   |  F2   |  F3   |  F4   |       |       |
   * |-------+-------+-------+-------+-------+-------|               |-------+-------+-------+-------+-------+-------|
   * |       | SFT_Y | SFT_I | SFT_E | SFT_A |   @   |               |  F5   |  F6   |  F7   |  F8   |       |       |
   * |-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------|
   * |       | SFT_J |   ?   |   !   | SFT_K |   `   |       |       |  F9   |  F10  |  F11  |  F12  |       |      |
   * |-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------|
   * |       |       |       |       | Ctrl  | Lower | Space |BackSpc| Raise |       |       |       |       |       |
   * `---------------------------------------------------------------------------------------------------------------'
   */
  [_MODS] = LAYOUT( \
      _______,_______,_______,_______,_______,_______,                _______,_______,_______,_______,_______,_______, \
      _______,S(KC_Q),S(KC_H),S(KC_O),S(KC_U),S(KC_X),                S(KC_G),S(KC_C),S(KC_R),S(KC_F),S(KC_Z),_______, \
      _______,S(KC_Y),S(KC_I),S(KC_E),S(KC_A),S(KC_2),                S(KC_D),S(KC_S),S(KC_T),S(KC_N),S(KC_B),_______, \
      _______,S(KC_J),SF_SLSH,S(KC_1),S(KC_K), KC_GRV,_______,_______,S(KC_W),S(KC_M),S(KC_L),S(KC_P),S(KC_V),_______, \
      _______,_______,_______,_______,_______, _______, _______, _______, _______, _______,_______,_______,_______,_______ \
      ),

  /* Lower
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |   ~  |   !  |   @  |   #  |   $  |   %  |             |   ^  |   &  |   *  |   (  |   )  |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |   ~  |   !  |   @  |   #  |   $  |   %  |             |   ^  |   &  |   *  |   (  |   )  |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |  F1  |  F2  |  F3  |  F4  |  F5  |             |  F6  |   _  |   +  |   {  |   }  |  |   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | CAPS |  F7  |  F8  |  F9  |  F10 |  F11 |  (   |   )  |  F12 |      |      | Home | End  |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      | Next | Vol- | Vol+ | Play |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_LOWER] = LAYOUT( \
      KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______, \
      KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______, \
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, \
      KC_CAPS, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_LPRN, KC_RPRN, KC_F12,  _______, _______, KC_HOME, KC_END,  _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY \
      ),

  /* Raise
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |   `  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | Bksp |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |   `  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | Del  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |  F1  |  F2  |  F3  |  F4  |  F5  |             |  F6  |   -  |   =  |   [  |   ]  |  \   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | CAPS |  F7  |  F8  |  F9  |  F10 |  F11 |      |      |  F12 |      |      |PageDn|PageUp|      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      | Next | Vol- | Vol+ | Play |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_RAISE] = LAYOUT( \
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, \
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL, \
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, \
      KC_CAPS, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  _______, _______, KC_F12,  _______, _______, KC_PGDN, KC_PGUP, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY \
      ),

  /* Adjust (Lower + Raise)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |             |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      | Reset|RGBRST|      |      |      |             |      |      |      |      |      |  Del |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |Aud on|Audoff| Mac  |             | Win  |Qwerty|Colemk|Dvorak|      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |RGB ON| HUE+ | SAT+ | VAL+ |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      | MODE | HUE- | SAT- | VAL- |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_ADJUST] =  LAYOUT( \
      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, \
      _______, RESET,   RGBRST,  _______, _______, _______,                   _______, _______, _______, _______, _______, KC_DEL, \
      _______, _______, _______, AU_ON,   AU_OFF,  AG_NORM,                   AG_SWAP, _______, _______, _______,  _______, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD \
      )
};


// define variables for reactive RGB
bool TOG_STATUS = false;
bool persist_hsv = true;

#ifdef RGBLIGHT_ENABLE

rgblight_config_t temp_config;

void keyboard_post_init_user(void) {temp_config.raw = rgblight_config.raw;}

#endif
void update_multi_layer_mappings(void){
    update_tri_layer(LT_LAY_LL, LT_LAY_LR, _LOWER);
    update_tri_layer(LT_LAY_RL, LT_LAY_RR, _RAISE);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LT_LL:
      if (record->event.pressed) {
        if(!is_ll_active){
            is_ll_active  = true;
        }
        ll_timer = timer_read();
      } else {
        if (timer_elapsed(ll_timer) > 100) {
            layer_off(LT_LAY_LL);
            update_multi_layer_mappings();
        } else {
            tap_code(LT_TAP_LL);
        }
        is_ll_active = false;
      }
      return false;
      break;

    case LT_LR:
      if (record->event.pressed) {
        if(!is_lr_active){
            is_lr_active  = true;
        }
        lr_timer = timer_read();
      } else {
        if (timer_elapsed(lr_timer) > 100) {
            layer_off(LT_LAY_LR);
            update_multi_layer_mappings();
        } else {
            tap_code(LT_TAP_LR);
        }
        is_lr_active = false;
      }
      return false;
      break;

    case LT_RL:
      if (record->event.pressed) {
        if(!is_rl_active){
            is_rl_active  = true;
        }
        rl_timer = timer_read();
      } else {
        if (timer_elapsed(rl_timer) > 100) {
            layer_off(LT_LAY_RL);
            update_multi_layer_mappings();
        } else {
            tap_code(LT_TAP_RL);
        }
        is_rl_active = false;
      }
      return false;
      break;

    case LT_RR:
      if (record->event.pressed) {
        if(!is_rr_active){
            is_rr_active  = true;
        }
        rr_timer = timer_read();
      } else {
        if (timer_elapsed(rr_timer) > 100) {
            layer_off(LT_LAY_RR);
            update_multi_layer_mappings();
        } else {
            tap_code(LT_TAP_RR);
        }
        is_rr_active = false;
      }
      return false;
      break;

      //led operations - RGB mode change now updates the RGB_current_mode to allow the right RGB mode to be set after reactive keys are released
    case RGB_MOD:


    #ifdef LED_BACK_ENABLE
        if (record->event.pressed) {
            rgblight_mode(temp_config.mode);
            rgblight_step();
            temp_config.mode = rgblight_config.mode;
        }
        return false;
      #endif
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
            temp_config.mode = rgblight_config.mode;
        }
      #endif
      break;
  }
  return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {

    //char buf[10];

    switch (get_highest_layer(state)) {
    case _BEAKL:
        strcpy(layertext, "Layer: BEAKL");
        #ifdef CONSOLE_ENABLE
            uprintf("enabling: _BEAKL\n");
            uprintf("lt: %u\n", &layertext);
        #endif
        break;
    case _SHIFTED:
        strcpy(layertext, "Layer: Shift");
        #ifdef CONSOLE_ENABLE
            uprintf("enabling: _SHIFTED\n");
        #endif
        break;
    case _SYMBOLS:
        strcpy(layertext, "Layer: Symbols");
        #ifdef CONSOLE_ENABLE
            uprintf("enabling: _SYMBOLS\n");
        #endif
        break;
    case _MODS:
        strcpy(layertext, "Layer: Mods");
        #ifdef CONSOLE_ENABLE
            uprintf("enabling: _MODS\n");
        #endif
        break;
    case _LOWER:
        strcpy(layertext, "Layer: Lower");
        #ifdef CONSOLE_ENABLE
           uprintf("enabling: _LOWER\n");
        #endif
        break;
    case _RAISE:
        strcpy(layertext, "Layer: Raise");
        #ifdef CONSOLE_ENABLE
            uprintf("enabling: _RAISE\n");
        #endif
        break;
    case _ADJUST:
        strcpy(layertext, "Layer: Adjust");
        #ifdef CONSOLE_ENABLE
            uprintf("enabling: _ADJUST\n");
        #endif
        break;
    default:
        strcpy(layertext, "Layer: UNKNOWN");
    }
  return state;
}

void matrix_init_user(void) {
    #ifdef AUDIO_ENABLE
        startup_user();
    #endif
    #ifdef RGBLIGHT_ENABLE
        temp_config.mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}

void matrix_scan_user(void) {
    iota_gfx_task();  // this is what updates the display continuously

    if (is_ll_active && timer_elapsed(ll_timer) > 100 && !IS_LAYER_ON(LT_LAY_LL)) {
        #ifdef CONSOLE_ENABLE
            uprintf("enabling: %u\n", LT_LAY_LL);
        #endif
        layer_on(LT_LAY_LL);
        update_multi_layer_mappings();
    }
    if (is_lr_active && timer_elapsed(lr_timer) > 100 && !IS_LAYER_ON(LT_LAY_LR)) {
        #ifdef CONSOLE_ENABLE
            uprintf("enabling: %u\n", LT_LAY_LR);
        #endif
        layer_on(LT_LAY_LR);
        update_multi_layer_mappings();
    }
    if (is_rl_active && timer_elapsed(rl_timer) > 100 && !IS_LAYER_ON(LT_LAY_RL)) {
        #ifdef CONSOLE_ENABLE
            uprintf("enabling: %u\n", LT_LAY_RL);
        #endif
        layer_on(LT_LAY_RL);
        update_multi_layer_mappings();
    }
    if (is_rr_active && timer_elapsed(rr_timer) > 100 && !IS_LAYER_ON(LT_LAY_RR)) {
        #ifdef CONSOLE_ENABLE
            uprintf("enabling: %u\n", LT_LAY_RR);
        #endif
        layer_on(LT_LAY_RR);
        update_multi_layer_mappings();
    }
}

void matrix_update(struct CharacterMatrix *dest,
                          const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

//assign the right code to your layers for OLED display
#define L_BASE 0
#define L_SHIFTED (1<<_SHIFTED)
#define L_SYMBOLS (1<<_SYMBOLS)
#define L_MODS (1<<_MODS)
#define L_LOWER (1<<_LOWER)
#define L_RAISE (1<<_RAISE)
#define L_ADJUST (1<<_ADJUST)
#define L_ADJUST_TRI (L_ADJUST|L_RAISE|L_LOWER)

static void render_logo(struct CharacterMatrix *matrix) {

  static const char helix_logo[] PROGMEM ={
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,
    0};
  matrix_write_P(matrix, helix_logo);
}
#ifdef LED_BACK_ENABLE
    static void render_rgbled_status(bool full, struct CharacterMatrix *matrix) {
    char buf[30];
    if (RGBLIGHT_MODES > 1 && rgblight_config.enable) {
        if (full) {
            snprintf(buf, sizeof(buf), " LED %2d: %d,%d,%d ",
                    rgblight_config.mode,
                    rgblight_config.hue/RGBLIGHT_HUE_STEP,
                    rgblight_config.sat/RGBLIGHT_SAT_STEP,
                    rgblight_config.val/RGBLIGHT_VAL_STEP);
        } else {
            snprintf(buf, sizeof(buf), "[%2d] ",rgblight_config.mode);
        }
        matrix_write(matrix, buf);
    }
    }
#endif

// static void render_layer_status(struct CharacterMatrix *matrix) {
//   // Define layers here, Have not worked out how to have text displayed for each layer. Copy down the number you see and add a case for it below
//   char buf[10];

//   matrix_write_P(matrix, PSTR("Layer: "));
//     // #ifdef CONSOLE_ENABLE
//     //     uprintf("layer_state: %u\n", layer_state);
//     //     uprintf("get_highest_layer(layer_state): %u\n", get_highest_layer(layer_state));
//     // #endif

//     switch (get_highest_layer(layer_state)) {
//         case L_BASE:
//             matrix_write_P(matrix, PSTR("Default"));
//             break;
//         case L_SHIFTED:
//            matrix_write_P(matrix, PSTR("Shift"));
//            break;
//         case L_SYMBOLS:
//            matrix_write_P(matrix, PSTR("Symbols"));
//            break;
//         case L_MODS:
//            matrix_write_P(matrix, PSTR("Mods"));
//            break;
//         case L_RAISE:
//            matrix_write_P(matrix, PSTR("Raise"));
//            break;
//         case L_LOWER:
//            matrix_write_P(matrix, PSTR("Lower"));
//            break;
//         case L_ADJUST:
//         case L_ADJUST_TRI:
//            matrix_write_P(matrix, PSTR("Adjust"));
//            break;
//         default:
//            matrix_write_P(matrix, PSTR("Undef-"));
//            snprintf(buf,sizeof(buf), "%ld", layer_state);
//            matrix_write(matrix, buf);

//     }
// }

void render_status(struct CharacterMatrix *matrix) {

  matrix_write_P(matrix, PSTR(" "));
  matrix_write(matrix, layertext);
  matrix_write_P(matrix, PSTR("\n"));

  // Host Keyboard LED Status
  matrix_write_P(matrix, (host_keyboard_leds() & (1<<USB_LED_NUM_LOCK)) ?
                 PSTR("NUMLOCK") : PSTR("       "));
  matrix_write_P(matrix, (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK)) ?
                 PSTR("CAPS") : PSTR("    "));
  matrix_write_P(matrix, (host_keyboard_leds() & (1<<USB_LED_SCROLL_LOCK)) ?
                 PSTR("SCLK") : PSTR("    "));
  matrix_write_P(matrix, PSTR("\n"));
  #ifdef LED_BACK_ENABLE
    render_rgbled_status(true, matrix);
  #endif
}


void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;

#if DEBUG_TO_SCREEN
  if (debug_enable) {
    return;
  }
#endif

  matrix_clear(&matrix);
  if(is_master){
    render_status(&matrix);
  }else{
    render_logo(&matrix);
    #ifdef LED_BACK_ENABLE
      render_rgbled_status(false, &matrix);
    #endif
    matrix_write_P(&matrix, layertext);
  }
  matrix_update(&display, &matrix);
}


