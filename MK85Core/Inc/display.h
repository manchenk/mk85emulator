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

#include <stdbool.h>

#include "bitmap.h"
#include "esp_err.h"

struct display;

typedef enum {
    DISPLAY_LANDSCAPE = 0,
    DISPLAY_PORTRAIT = 1
} display_orientation_t;

typedef struct {
    display_orientation_t orientation;
    bool flip_vertically;
    bool flip_horizontally;
} display_initialization_t;

typedef esp_err_t (*display_get_config_t)(const struct display *display, int type);
typedef esp_err_t (*display_init_t)(struct display *display, const display_initialization_t *init);
typedef esp_err_t (*display_done_t)(struct display *display);
typedef esp_err_t (*display_refresh_t)(const display_bitmap_t *bitmap);

typedef struct {
    int bitmap_extra_size;
    int bpp;
    device_color_format_t default_format;
    void *palette;
    int palette_count;
    int type;
} display_hardware_config_t;

typedef struct display {
    void *device;
    display_hardware_config_t *hardware;
    display_rectangle_t bounds;
//    int bitmap_extra_size;
//    int bpp;
//    device_color_format_t default_format;

    display_orientation_t orientation;

//    int formats;
    display_color_t background;
    display_color_t foreground;

    display_get_config_t get_config;
    display_init_t init;
    display_done_t done;
    display_refresh_t refresh;
} display_t;

int display_get_color(const display_point_t *p, const display_refresh_info_t *refresh_info, void *color);
int display_get_color_with_background(const display_point_t *p, const display_refresh_info_t *info, void *color);

esp_err_t display_get_config(display_t *display, int type);
esp_err_t display_init(display_t *display, const display_initialization_t *init);
esp_err_t display_done(display_t *display);
esp_err_t display_refresh(const display_bitmap_t *bitmap);

