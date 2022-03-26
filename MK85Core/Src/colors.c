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
#include "colors.h"

void display_rgb888_rgb555(display_color_rgb555_t *rgb555, const display_color_rgb888_t *rgb888)
{
    rgb555->r = rgb888->r >> 3;
    rgb555->g = rgb888->g >> 3;
    rgb555->b = rgb888->b >> 3;
}

uint16_t display_rgb555_uint16(const display_color_rgb555_t *rgb555)
{
    display_color_rgb555_t tmp;
    tmp.rgb_l = rgb555->rgb_h;
    tmp.rgb_h = rgb555->rgb_l;
    return tmp.rgb;
}

#define DISPLAY_MUL(a,b) ((257*(a)*(b)+32768)>>16)
#define DISPLAY_INV(a) (255-(a))

void display_hsv_rgb888(display_color_rgb888_t *rgb, const display_color_hsv_t *hsv)
{
    if (hsv->s == 0) {
        rgb->r = hsv->v;
        rgb->g = hsv->v;
        rgb->b = hsv->v;
    }
    else {
        uint8_t r = hsv->h & 0xff;
        uint8_t p = DISPLAY_MUL(hsv->v, DISPLAY_INV(hsv->s));                                 // (hsv->v * (255 - hsv->s) + 128) >> 8;
        uint8_t q = DISPLAY_MUL(hsv->v, DISPLAY_INV(DISPLAY_MUL(hsv->s, r)));                 // (hsv->v * (255 - ((hsv->s * r + 128) >> 8)) + 128) >> 8;
        uint8_t t = DISPLAY_MUL(hsv->v, DISPLAY_INV(DISPLAY_MUL(hsv->s, DISPLAY_INV(r))));    // (hsv->v * (255 - ((hsv->s * (255 - r) +128) >> 8)) + 128) >> 8;

        switch ((hsv->h >> 8) % 6) {
            case 0:
                rgb->r = hsv->v; rgb->g = t; rgb->b = p;
                break;
            case 1:
                rgb->r = q; rgb->g = hsv->v; rgb->b = p;
                break;
            case 2:
                rgb->r = p; rgb->g = hsv->v; rgb->b = t;
                break;
            case 3:
                rgb->r = p; rgb->g = q; rgb->b = hsv->v;
                break;
            case 4:
                rgb->r = t; rgb->g = p; rgb->b = hsv->v;
                break;
            default:
                rgb->r = hsv->v; rgb->g = p; rgb->b = q;
                break;
        }
    }
}

#undef DISPLAY_INV
#undef DISPLAY_MUL

