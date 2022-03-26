/*
 * This file is part of the MK85Emulator distribution (https://github.com/manchenk/mk85emulator.git).
 * Copyright (c) 2022 Dmitry Romanchenko.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "display.h"
#include "pdp11_types.h"
#include "mk85_kbd.h"

typedef struct {
    display_point_t touch;
    bool_t pressed;
} touch_kbd_info_t;

typedef struct {
    display_point_t *point;
    bool_t state;
} touch_kbd_point_t;

typedef void (*touch_kbd_get_state_t)(touch_kbd_info_t *info);
typedef void (*touch_kbd_draw_point_t)(touch_kbd_point_t *info);
typedef void (*touch_kbd_update_screen_t)();

typedef struct {
    display_size_t disp_size;
    int kx, bx;
    int ky, by;
    bool_t done;
    touch_kbd_info_t info;
    int press_counter;
    int release_counter;
    mk85_key_t prv_key;
    mk85_key_t cur_key;
    touch_kbd_update_screen_t keyboard_screen;
    touch_kbd_draw_point_t draw_point;
    touch_kbd_get_state_t get_state;
} touch_kbd_t;

esp_err_t touch_kbd_create(touch_kbd_t **pkbd);
esp_err_t touch_kbd_init(touch_kbd_t *kbd);
esp_err_t touch_kbd_get_key(touch_kbd_t *kbd, mk85_key_t *key);
esp_err_t touch_kbd_done(touch_kbd_t *kbd);

