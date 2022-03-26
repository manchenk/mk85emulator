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

#include "esp_err.h"
#include "display.h"
#include "mk85_mb.h"
#include "app_lcd.h"
#include "touch_kbd.h"

typedef struct {
    void *video_memory;
    display_t *oled;
    touch_kbd_t *keyboard;
    display_bitmap_t *canvas;
    app_lcd_t *lcd;
    mk85_mb_t *core;
} app_t;

esp_err_t app_create(app_t **papp);
esp_err_t app_init(app_t *app);
esp_err_t app_run(app_t *app);
esp_err_t app_done(app_t *app);
