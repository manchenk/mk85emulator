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

typedef struct display_point {
    int x;
    int y;
} display_point_t;

typedef struct display_size {
    int width;
    int height;
} display_size_t;

typedef struct display_rectangle {
    int left;
    int top;
    int width;
    int height;
} display_rectangle_t;

int display_point_in_sector(const display_point_t *center, const display_point_t *ray1, const display_point_t *ray2, const display_point_t *pnt);
int display_point_in_zone(const display_point_t *list, int count, const display_point_t *pnt);
int display_point_in_pzone(const display_point_t **list, int count, const display_point_t *pnt);

// 00 01 02 04 08
//   +--------+
// 10|11 12 14|18
//   |  +--+  |
// 20|21|22|24|28
//   |  +--+  |
// 40|41 42 44|48
//   +--------+
// 80 81 82 84 88
int display_point_in_rectangle(const display_rectangle_t *r, int thickness, const display_point_t *p);
int display_rectangle_intersect(display_rectangle_t *result, const display_rectangle_t *r0, const display_rectangle_t *r1);
