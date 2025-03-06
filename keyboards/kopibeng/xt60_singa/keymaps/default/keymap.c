/* Copyright 2021 Samuel Lu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// Bật tất cả hiệu ứng LED
#define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_DEFAULT_MODE 1  // Mặc định mode 1
#define RGBLIGHT_DISABLE_SAVE    // Không lưu vào EEPROM
#define RGBLIGHT_DISABLE_EEPROM                        // Không lưu trạng thái LED vào EEPROM
#define VIA_EEPROM_CUSTOM_RESET                        // Ngăn VIA ghi đè cấu hình LED
#define RGBLIGHT_LAYERS_OVERRIDE

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	// Default layer
	[0] = LAYOUT_all(
		QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_BSPC,
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,        
		KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,         
		KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, MO(2),  
		KC_LCTL, KC_LGUI, KC_LALT,          KC_SPC,           KC_SPC,           KC_SPC,           MO(2),   KC_RALT, MO(1),   KC_RCTL
	),

	// Fn1 Layer
	[1] = LAYOUT_all(
		KC_TRNS, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, QK_BOOT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
		KC_TRNS, KC_TRNS, KC_VOLD, KC_VOLU, KC_MPLY, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	),

	// Fn2 Layer
	[2] = LAYOUT_all(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	),
};

/*
#include "timer.h"

#define EFFECT_DELAY 3000 // Chuyển hiệu ứng mỗi 5 giây

uint16_t last_effect_change = 0;
uint8_t current_mode = 1;

void matrix_scan_user(void) {
    if (timer_elapsed(last_effect_change) > EFFECT_DELAY) {
        current_mode += 1;
		if(current_mode > 35)
			current_mode = 1;
        rgblight_mode(current_mode);
        last_effect_change = timer_read();
    }
}
*/


#include "rgblight.h"
#include "timer.h"

#define EFFECT_DELAY 15000
#define COLOR_CHANGE_COUNT 5
#define NUM_EFFECTS 35

uint16_t last_effect_change = 0;
uint8_t effect_index = 0;
uint8_t color_change_counter = 0;
uint8_t effect_list[NUM_EFFECTS];

bool single_color_effects[] = {
    false, true,  false, true,  false, true,  false, false, true,  false,
    true,  false, true,  false, false, false, true,  false, true,  false,
    true,  false, false, false, true,  false, true,  false, false, true,
    false, true,  false, false, false
};

void shuffle_effects(void) {
    for (uint8_t i = 0; i < NUM_EFFECTS; i++) {
        effect_list[i] = i + 1;
    }
    for (uint8_t i = 0; i < NUM_EFFECTS - 1; i++) {
        uint8_t j = i + (rand() % (NUM_EFFECTS - i));
        uint8_t temp = effect_list[i];
        effect_list[i] = effect_list[j];
        effect_list[j] = temp;
    }
}

void matrix_scan_user(void) {
    // Kiểm tra nếu VIA đang điều khiển LED
    if (!rgblight_is_enabled() || rgblight_get_mode() == 0) {
        return;
    }

    if (effect_index == 0) {
        shuffle_effects();
    }

    if (timer_elapsed(last_effect_change) > EFFECT_DELAY) {
        if (single_color_effects[effect_list[effect_index] - 1] && color_change_counter < COLOR_CHANGE_COUNT) {
            rgblight_sethsv_noeeprom(rand() % 256, 255, 255); // Không ghi EEPROM
            color_change_counter++;
        } else {
            color_change_counter = 0;
            rgblight_mode_noeeprom(effect_list[effect_index]); // Không ghi EEPROM
            effect_index++;

            if (effect_index >= NUM_EFFECTS) {
                effect_index = 0;
                shuffle_effects();
            }
        }
        last_effect_change = timer_read();
		rgblight_set_speed_noeeprom(128);//50%
    }
}
