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
#include <string.h>
#include <stdbool.h>

#include "colors.h"
#include "display.h"
#include "esp_log.h"

static const char __attribute__((unused)) *TAG = "display";

int display_get_color(const display_point_t *p, const display_refresh_info_t *refresh_info, void *color)
{
    return refresh_info->color.get(p, refresh_info, color);
}

int display_get_color_with_background(const display_point_t *p, const display_refresh_info_t *refresh_info, void *color)
{
    const display_t *display = refresh_info->bitmap->display;
    int result = refresh_info->color.get(p, refresh_info, color);
    if (!result) {
        if (display->background.get) {
            display_refresh_info_t background_info = {
                rectangle: refresh_info->rectangle,
                bitmap: refresh_info->bitmap,
                color: display->background
            };
            result = display->background.get(p, &background_info, color);
        }
    }
    return result;
}

static void display_format_bw1x(const display_refresh_info_t *info, bool is_le)
{
    const display_rectangle_t *r = &info->rectangle;
    const display_bitmap_t *bitmap = info->bitmap;
    display_point_t p;
    int left = r->left;
    int right = left + r->width;
    int top = r->top;
    int bottom = top + r->height;

    //ESP_LOGI(TAG, "bw1x refresh rectangle: l: %3d, t: %3d, w: %3d, h: %3d", r->left, r->top, r->width, r->height);
    //ESP_LOGI(TAG, "bitmap bounds: l: %3d, t: %3d, w: %3d, h: %3d", b->left, b->top, b->width, b->height);
    //ESP_LOGI(TAG, "color get: %p", info->color.get);
    //ESP_LOGI(TAG, "color args: %p", info->color.args);

    for (p.y = top; p.y < bottom; ++p.y) {
        uint8_t *dst = (uint8_t *)bitmap->data + p.y * bitmap->columns + (r->left >> 3);
        uint8_t out = ~(*dst);
        uint8_t mask = is_le ? 0x01 << (left & 7) : 0x80 >> (left & 7);
        for (p.x = left; p.x < right; ++p.x) {
            uint8_t in = 0;
            if (display_get_color_with_background(&p, info, &in)) {
                if (in & 1)
                    out |= mask;
                else
                    out &= ~mask;
            }
            if (is_le)
                mask <<= 1;
            else
                mask >>= 1;
            if (mask == 0) {
                mask = is_le ? 0x01 : 0x80;
                *dst++ = ~out; // black on white
                out = ~(*dst);
            }
        }
        *dst = ~out; // black on white
    }
}

static void display_format_bw1y(const display_refresh_info_t *info, bool is_le)
{
    const display_rectangle_t *r = &info->rectangle;
    const display_bitmap_t *bitmap = info->bitmap;
    display_point_t p;
    int left = r->left;
    int right = left + r->width;
    int top = r->top;
    int bottom = top + r->height;

    //ESP_LOGI(TAG, "bw1y refresh rectangle: l: %3d, t: %3d, w: %3d, h: %3d", r->left, r->top, r->width, r->height);
    //ESP_LOGI(TAG, "bitmap bounds: l: %3d, t: %3d, w: %3d, h: %3d", b->left, b->top, b->width, b->height);
    //ESP_LOGI(TAG, "color get: %p", info->color.get);
    //ESP_LOGI(TAG, "color args: %p", info->color.args);

    for (p.y = top; p.y < bottom; ++p.y) {
        uint8_t *dst = (uint8_t *)bitmap->data + (p.y >> 3) * bitmap->columns + r->left;
        uint8_t mask = is_le ? 0x01 << (p.y & 7) : 0x80 >> (p.y & 7);
        for (p.x = left; p.x < right; ++p.x) {
            uint8_t out = ~(*dst);
            uint8_t in = 0;
            if (display_get_color_with_background(&p, info, &in)) {
                if (in & 1)
                    out |= mask;
                else
                    out &= ~mask;
            }
            *dst++ = ~out; // black on white
        }
    }
}

static void display_format_8i4_to_rgb555(display_refresh_info_t *refresh_info)
{
    const display_rectangle_t *r = &refresh_info->rectangle;
    const display_bitmap_t *bitmap = refresh_info->bitmap;
    const display_t *display = bitmap->display;
    const display_rectangle_t *b = &bitmap->bounds;
    display_color_rgb555_t *dst = (display_color_rgb555_t *)bitmap->data;
    display_point_t p;

    size_t bit;
    int ofs = r->top * b->width + r->left;
    int delta = b->width - r->width;
    dst += ofs;
    for (p.y = r->top; p.y < r->top + r->height; ++p.y) {
        uint32_t in_color = 0;
        int ok = 0;
        for (p.x = r->left, bit = 0; p.x < r->left + r->width; ++p.x, ++bit) {
            if ((bit & 7) == 0) {
                in_color = 0;
                ok = display_get_color_with_background(&p, refresh_info, &in_color);
            }
            if (ok) {
                const display_hardware_config_t *hw = display->hardware;
                display_color_rgb555_t *src = (display_color_rgb555_t *)hw->palette;
                int idx = (in_color & 0x0f); // % display->palette_count;
                if (idx < hw->palette_count)
                    *dst = src[idx];
                in_color >>= 4;
            }
            ++dst;
        }
        dst += delta;
    }
}

static void display_format_rgb555(display_refresh_info_t *refresh_info)
{
    const display_rectangle_t *r = &refresh_info->rectangle;
    const display_bitmap_t *bitmap = refresh_info->bitmap;
    const display_rectangle_t *b = &bitmap->bounds;
    display_color_rgb555_t *dst = (display_color_rgb555_t *)bitmap->data;
    display_point_t p;
    int ofs = r->top * b->width + r->left;
    int delta = b->width - r->width;
    dst += ofs;

    //ESP_LOGD(TAG, "ofs: %d, delta %d", ofs, delta);
    //ESP_LOGD(TAG, "refresh rectangle: l: %3d, t: %3d, w: %3d, h: %3d", r->left, r->top, r->width, r->height);
    //ESP_LOGD(TAG, "bitmap bounds: l: %3d, t: %3d, w: %3d, h: %3d", b->left, b->top, b->width, b->height);
    //ESP_LOGD(TAG, "color get: %p", refresh_info->color.get);
    //ESP_LOGD(TAG, "color args: %p", refresh_info->color.args);

    for (p.y = r->top; p.y < r->top + r->height; ++p.y) {
        for (p.x = r->left; p.x < r->left + r->width; ++p.x) {
            uint32_t in_color = 0;
            if (display_get_color_with_background(&p, refresh_info, &in_color)) {
                dst->rgb = in_color;
            }
            ++dst;
        }
        dst += delta;
    }
}

static void display_format_argb8888(display_refresh_info_t *refresh_info)
{
    const display_rectangle_t *r = &refresh_info->rectangle;
    const display_bitmap_t *bitmap = refresh_info->bitmap;
    const display_rectangle_t *b = &bitmap->bounds;
    display_color_argb8888_t *dst = (display_color_argb8888_t *)bitmap->data;
    display_point_t p;
    int ofs = r->top * b->width + r->left;
    int delta = b->width - r->width;
    dst += ofs;

    //ESP_LOGD(TAG, "ofs: %d, delta %d", ofs, delta);
    //ESP_LOGD(TAG, "refresh rectangle: l: %3d, t: %3d, w: %3d, h: %3d", r->left, r->top, r->width, r->height);
    //ESP_LOGD(TAG, "bitmap bounds: l: %3d, t: %3d, w: %3d, h: %3d", b->left, b->top, b->width, b->height);
    //ESP_LOGD(TAG, "color get: %p", refresh_info->color.get);
    //ESP_LOGD(TAG, "color args: %p", refresh_info->color.args);

    for (p.y = r->top; p.y < r->top + r->height; ++p.y) {
        for (p.x = r->left; p.x < r->left + r->width; ++p.x) {
            uint32_t in_color = 0;
            if (display_get_color_with_background(&p, refresh_info, &in_color)) {
                dst->argb = in_color;
            }
            ++dst;
        }
        dst += delta;
    }
}

//static void display_format_prgb555(display_refresh_info_t *refresh_info)
//{
//    const display_rectangle_t *r = &refresh_info->rectangle;
//    const display_bitmap_t *bitmap = refresh_info->bitmap;
//    const display_rectangle_t *b = &bitmap->bounds;
//    display_color_rgb555_t *dst = (display_color_rgb555_t *)bitmap->data;
//    display_point_t p;
//    int ofs = r->top * b->width + r->left;
//    int delta = b->width - r->width;
//    dst += ofs;
//    for (p.y = r->top; p.y < r->top + r->height; ++p.y) {
//        uint16_t *src = NULL;
//        int ok = display_get_color_with_background(&p, refresh_info, &src);
//        for (p.x = r->left; p.x < r->left + r->width; ++p.x) {
//            if (ok) {
//                dst->rgb = *src;
//                ++src;
//            }
//            ++dst;
//        }
//        dst += delta;
//    }
//}

esp_err_t display_bitmap_refresh(display_refresh_info_t *refresh_info)
{
    esp_err_t ret = ESP_OK;
    ESP_ERROR_CHECK(refresh_info ? ESP_OK : ESP_ERR_INVALID_ARG);
    const display_bitmap_t *bitmap = refresh_info->bitmap;
    ESP_ERROR_CHECK(bitmap ? ESP_OK : ESP_ERR_INVALID_STATE);
    const display_t *display = bitmap->display;
    ESP_ERROR_CHECK(display ? ESP_OK : ESP_ERR_INVALID_STATE);
    switch(refresh_info->color.format) {
        case DISPLAY_COLOR_ARGB8888: {
            switch (bitmap->format) {
                case DEVICE_COLOR_ARGB8888:
                    display_format_argb8888(refresh_info);
                    break;
                default:
                    break;
            }
            break;
        }
        case DISPLAY_COLOR_RGB555: {
            switch (bitmap->format) {
                case DEVICE_COLOR_RGB555:
                    display_format_rgb555(refresh_info);
                    break;
                default:
                    break;
            }
            break;
        }
        case DISPLAY_COLOR_8I4: {
            switch (bitmap->format) {
                case DEVICE_COLOR_RGB555:
                    display_format_8i4_to_rgb555(refresh_info);
                    break;
                default:
                    break;
            }
            break;
        }
        case DISPLAY_COLOR_BW1: {
            switch (bitmap->format) {
                case DEVICE_COLOR_BW1LE:
                    switch(display->orientation) {
                        case DISPLAY_PORTRAIT:
                            display_format_bw1x(refresh_info, true);
                            break;
                        case DISPLAY_LANDSCAPE:
                            display_format_bw1y(refresh_info, true);
                            break;
                    }
                    break;
                case DEVICE_COLOR_BW1BE:
                    switch(display->orientation) {
                        case DISPLAY_PORTRAIT:
                            display_format_bw1x(refresh_info, false);
                            break;
                        case DISPLAY_LANDSCAPE:
                            display_format_bw1y(refresh_info, false);
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    return ret;
}

esp_err_t display_get_config(display_t *display, int type)
{
    ESP_ERROR_CHECK(display ? ESP_OK : ESP_ERR_INVALID_ARG);
    ESP_ERROR_CHECK(display->get_config ? ESP_OK : ESP_ERR_INVALID_STATE);
    return display->get_config(display, type);
}

esp_err_t display_init(display_t *display, const display_initialization_t *init)
{
    ESP_ERROR_CHECK(display ? ESP_OK : ESP_ERR_INVALID_ARG);
    ESP_ERROR_CHECK(display->init ? ESP_OK : ESP_ERR_INVALID_STATE);
    return display->init(display, init);
}

esp_err_t display_done(display_t *display)
{
    ESP_ERROR_CHECK(display ? ESP_OK : ESP_ERR_INVALID_ARG);
    ESP_ERROR_CHECK(display->done ? ESP_OK : ESP_ERR_INVALID_STATE);
    return display->done(display);
}

esp_err_t display_refresh(const display_bitmap_t *bitmap)
{
    ESP_ERROR_CHECK(bitmap ? ESP_OK : ESP_ERR_INVALID_ARG);
    const display_t * display = bitmap->display;
    ESP_ERROR_CHECK(display ? ESP_OK : ESP_ERR_INVALID_STATE);
    ESP_ERROR_CHECK(display->refresh ? ESP_OK : ESP_ERR_INVALID_STATE);
    return display->refresh(bitmap);
}

