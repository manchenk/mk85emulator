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

#include "geometry.h"
#include "esp_err.h"

typedef enum {
    DEVICE_COLOR_UNKNOWN = 0,
    DEVICE_COLOR_ARGB8888,
    DEVICE_COLOR_RGB555,
    DEVICE_COLOR_BW1LE,
    DEVICE_COLOR_BW1BE,
    DEVICE_COLOR_MAX
} device_color_format_t;

typedef enum {
    DISPLAY_COLOR_UNKNOWN = 0,
    DISPLAY_COLOR_ARGB8888,
    DISPLAY_COLOR_RGB555,
    DISPLAY_COLOR_PRGB555,
    DISPLAY_COLOR_BW1,
    DISPLAY_COLOR_8I4,
    DISPLAY_COLOR_MAX
} display_color_format_t;

struct display;

typedef struct {
    void *buffer;
    int size;
    void *extra;
    int extra_size;
    void *data;
    int data_size;
    const struct display *display;
    display_rectangle_t bounds;
    device_color_format_t format;
    int rows;
    int columns;
    int bpp;
} display_bitmap_t;

struct display_refresh_info;
typedef int (*display_color_callback_t)(const display_point_t *, const struct display_refresh_info *, void *color);

typedef struct {
    display_color_callback_t get;
    display_color_format_t format;
    void *args;
} display_color_t;

typedef struct display_refresh_info {
    const display_rectangle_t rectangle;
    const display_bitmap_t *bitmap;
    const display_color_t color;
} display_refresh_info_t;

esp_err_t display_bitmap_create(display_bitmap_t **pbitmap);
esp_err_t display_bitmap_init(display_bitmap_t *bitmap, const struct display *display);
esp_err_t display_bitmap_done(display_bitmap_t *bitmap);
esp_err_t display_bitmap_refresh(display_refresh_info_t *refresh_info);

