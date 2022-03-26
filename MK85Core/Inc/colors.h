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

#include <stdint.h>

typedef struct display_color_hsv {
    uint16_t h;  // 0 ... 6*256-1
    uint8_t s;   // 0 ... 255
    uint8_t v;   // 0 ... 255
} display_color_hsv_t;

typedef union {
    uint32_t rgb;
    struct {
        uint8_t r;  // 0 ... 255
        uint8_t g;  // 0 ... 255
        uint8_t b;  // 0 ... 255
    };
} display_color_rgb888_t;

typedef union {
    uint16_t rgb;
    struct {
        uint16_t r: 5;  // 0 ... 31
        uint16_t brightness: 1;
        uint16_t g: 5;  // 0 ... 31
        uint16_t b: 5;  // 0 ... 31
    };
    struct {
        uint8_t rgb_l;
        uint8_t rgb_h;
    };
} display_color_rgb555_t;

typedef union {
    uint32_t argb;
    struct {
        uint32_t r: 8;  // 0 ... 31
        uint32_t g: 8;  // 0 ... 31
        uint32_t b: 8;  // 0 ... 31
        uint32_t a: 8;  // 0 ... 31
    };
} display_color_argb8888_t;

void display_rgb888_rgb555(display_color_rgb555_t *rgb555, const display_color_rgb888_t *rgb888);
void display_hsv_rgb888(display_color_rgb888_t *rgb, const display_color_hsv_t *hsv);
uint16_t display_rgb555_uint16(const display_color_rgb555_t *rgb555);

