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
#include <stdlib.h>
#include "esp_log.h"
#include "display.h"

static const char __attribute__((unused)) *TAG = "bitmap";

esp_err_t display_bitmap_create(display_bitmap_t **pbitmap) {
    esp_err_t ret = ESP_OK;
    ESP_ERROR_CHECK(pbitmap ? ESP_OK : ESP_ERR_INVALID_ARG);
    display_bitmap_t *bitmap = (display_bitmap_t*) malloc(
            sizeof(display_bitmap_t));
    ESP_ERROR_CHECK(bitmap ? ESP_OK : ESP_ERR_NO_MEM);
    bzero(bitmap, sizeof(display_bitmap_t));
    *pbitmap = bitmap;
    return ret;
}

esp_err_t display_bitmap_init(display_bitmap_t *bitmap,
        const struct display *display) {
    esp_err_t ret = ESP_OK;
    ESP_ERROR_CHECK(bitmap ? ESP_OK : ESP_ERR_INVALID_ARG);
    ESP_ERROR_CHECK(display ? ESP_OK : ESP_ERR_INVALID_ARG);
    bitmap->display = display;
    const display_rectangle_t *b = &bitmap->bounds;
    display_orientation_t orientation = display->orientation;
    bitmap->extra_size = display->hardware->bitmap_extra_size;
    void *data = NULL; //  bitmap->header;
    ESP_ERROR_CHECK(!data ? ESP_OK : ESP_ERR_INVALID_STATE);
    int data_size = 0;
    switch (bitmap->format) {
    case DEVICE_COLOR_ARGB8888:
        bitmap->bpp = 32;
        bitmap->columns = b->width;
        bitmap->rows = b->height;
        data_size = (b->width * b->height * bitmap->bpp) >> 3;
        break;
    case DEVICE_COLOR_RGB555:
        bitmap->bpp = 16;
        bitmap->columns = b->width;
        bitmap->rows = b->height;
        data_size = (b->width * b->height * bitmap->bpp) >> 3;
        break;
    case DEVICE_COLOR_BW1LE:
    case DEVICE_COLOR_BW1BE:
        switch (orientation) {
        case DISPLAY_PORTRAIT:
            bitmap->columns = (((b->left + b->width + 7) & ~7) - (b->left & ~7)) >> 3;
            bitmap->rows = b->height;
            break;
        case DISPLAY_LANDSCAPE:
            bitmap->columns = b->width;
            bitmap->rows = (((b->top + b->height + 7) & ~7) - (b->top & ~7)) >> 3;
            break;
        }
        data_size = bitmap->columns * bitmap->rows;
        bitmap->bpp = 1;
        break;
    default:
        ESP_ERROR_CHECK(ESP_ERR_INVALID_STATE);
        break;
    }
    if (bitmap->format == DEVICE_COLOR_UNKNOWN)
        bitmap->format = display->hardware->default_format;

//    ESP_LOGI(TAG, "bitmap init bounds l: %3d, t: %3d, w: %d, h: %d", b->left, b->top, b->width, b->height);
//    ESP_LOGI(TAG, "bitmap init columns: %d, rows: %d", bitmap->columns, bitmap->rows);
//    ESP_LOGI(TAG, "bitmap init size: %d", bitmap->size);

    ESP_ERROR_CHECK(data_size ? ESP_OK : ESP_ERR_INVALID_STATE);
    int size = data_size + bitmap->extra_size;
    void *buffer = malloc(size);
    ESP_ERROR_CHECK(buffer ? ESP_OK : ESP_ERR_NO_MEM);
    bzero(buffer, size);
    bitmap->buffer = buffer;
    bitmap->size = size;
    bitmap->extra = buffer;
    bitmap->data = (uint8_t*) bitmap->extra + bitmap->extra_size;
    bitmap->data_size = data_size;
    return ret;
}

esp_err_t display_bitmap_done(display_bitmap_t *bitmap) {
    esp_err_t ret = ESP_OK;
    ESP_ERROR_CHECK(bitmap ? ESP_OK : ESP_ERR_INVALID_ARG);
    ESP_ERROR_CHECK(bitmap->buffer ? ESP_OK : ESP_ERR_INVALID_STATE);
    free(bitmap->buffer);
    bitmap->data = NULL;
    bitmap->buffer = NULL;
    bitmap->extra = NULL;
    free(bitmap);
    return ret;
}

