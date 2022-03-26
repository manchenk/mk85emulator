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
#include "geometry.h"
#include "esp_log.h"

static const char __attribute__((unused)) *TAG = "geometry";

static int display_vector_direction(const display_point_t *cur, const display_point_t *prv, const display_point_t *pnt)
{
    int d0x = cur->x - prv->x;
    int d0y = cur->y - prv->y;
    int d1x = pnt->x - prv->x;
    int d1y = pnt->y - prv->y;
    int d2y = pnt->y - cur->y;
    return (((d1y >= 0) && (d2y < 0)) || ((d2y >= 0) && (d1y < 0))) && (d1x * d0y * d0y < d1y * d0x * d0y);
}

static int display_half_plane(const display_point_t *p0, const display_point_t *p1, const display_point_t *p)
{
    int d0x = p0->x - p1->x;
    int d0y = p0->y - p1->y;
    int d1x =  p->x - p1->x;
    int d1y =  p->y - p1->y;
    return d1x * d0y < d1y * d0x;
}

int display_point_in_sector(const display_point_t *center, const display_point_t *ray1, const display_point_t *ray2, const display_point_t *pnt)
{
    int res = 0;
    if (display_half_plane(center, ray1, pnt))
        res += 1;
    if (display_half_plane(center, ray2, pnt))
        res += 2;
    return res;
}

int display_point_in_zone(const display_point_t *list, int count, const display_point_t *pnt)
{
    const display_point_t *prv = list + count - 1;
    int res = 0;
    int i;
    for (i = 0; i < count; ++i) {
        const display_point_t *cur = list + i;
        if (display_vector_direction(cur, prv, pnt))
            ++res;
        prv = cur;
    }
    return (res % 2) != 0;
}

int display_point_in_pzone(const display_point_t **list, int count, const display_point_t *pnt)
{
    const display_point_t *prv = *(list + count - 1);
    int res = 0;
    int i;
    for (i = 0; i < count; ++i) {
        const display_point_t *cur = *(list + i);
        if (display_vector_direction(cur, prv, pnt))
            ++res;
        prv = cur;
    }
    return (res % 2) != 0;
}

int display_point_in_rectangle(const display_rectangle_t *r, int thickness, const display_point_t *p)
{
    int res = 0;
    if (p->x >= r->left) res += 0x01;
    if (p->y >= r->top) res += 0x10;
    if (p->x >= r->left + r->width) res += 0x08;
    if (p->y >= r->top + r->height) res += 0x80;
    if (thickness) {
        if (p->x >= r->left + thickness) res += 0x02;
        if (p->x >= r->left + r->width - thickness) res += 0x04;
        if (p->y >= r->top + thickness) res += 0x20;
        if (p->y >= r->top + r->height - thickness) res += 0x40;
    }
    res ^= (res & 0xee) >> 1;
    return res;
}

int display_rectangle_intersect(display_rectangle_t *result, const display_rectangle_t *r0, const display_rectangle_t *r1) {
    int rt0 = r0->left + r0->width;
    int bt0 = r0->top + r0->height;
    int rt1 = r1->left + r1->width;
    int bt1 = r1->top + r1->height;
    int l = r0->left > r1->left ? r0->left : r1->left;
    int r = rt0 < rt1 ? rt0 : rt1;
    int t = r0->top > r1->top ? r0->top : r1->top;
    int b = bt0 < bt1 ? bt0 : bt1;
    int res;
    if (r > l && b > t) {
        result->left = l;
        result->top = t;
        result->width = r - l;
        result->height = b - t;
        res = 1;
    }
    else {
        result->left = 0;
        result->top = 0;
        result->width = 0;
        result->height = 0;
        res = 0;
    }

//    ESP_LOGI(TAG, "r0 l: %3d, t: %3d, w: %3d, h: %3d", r0->left, r0->top, r0->width, r0->height);
//    ESP_LOGI(TAG, "r1 l: %3d, t: %3d, w: %3d, h: %3d", r1->left, r1->top, r1->width, r1->height);
//    ESP_LOGI(TAG, "rr l: %3d, t: %3d, w: %3d, h: %3d", result->left, result->top, result->width, result->height);

    return res;
}

