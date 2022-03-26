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
#include "esp_err.h"
#include "video.h"

static const char __attribute__((unused)) *TAG = "video";

static video_config_t video_default_config = {
    view: {
        left: 0,
        top: 0,
        width: VIDEO_DEFAULT_WIDTH,
        height: VIDEO_DEFAULT_HEIGHT
    },
    hardware: {
        bitmap_extra_size: 0,
        bpp: VIDEO_DEFAULT_BPP,
        default_format: DEVICE_COLOR_ARGB8888,
        palette: NULL,
        palette_count: 0,
        type: VIDEO_TYPE_320x240
    },
    width: VIDEO_DEFAULT_WIDTH,
    height: VIDEO_DEVICE_HEIGHT,
};

static esp_err_t video_get_default_config(video_config_t *cfg)
{
    ESP_ERROR_CHECK(cfg ? ESP_OK : ESP_ERR_INVALID_ARG);
    *cfg = video_default_config;
    return ESP_OK;
}

static esp_err_t video_device_init(video_t *dev)
{
    esp_err_t r = ESP_OK;
    return r;
}

static esp_err_t video_bitmap_output(const video_t *device, const display_bitmap_t *bitmap)
{
    esp_err_t err = ESP_OK;
    const video_config_t *config = &device->config;
    const display_rectangle_t *b = &bitmap->bounds;
    const display_rectangle_t *v = &config->view;
    int lt = (device->orientation != DISPLAY_LANDSCAPE ? b->left + v->left : b->top + v->top);
    int tp = (device->orientation == DISPLAY_LANDSCAPE ? b->left + v->left : b->top + v->top);
    int rt = lt + (device->orientation != DISPLAY_LANDSCAPE ? b->width : b->height) - 1;
    int bt = tp + (device->orientation == DISPLAY_LANDSCAPE ? b->width : b->height) - 1;

    //ESP_LOGI(TAG, "bitmap bounds l:%d, t:%d, w:%d, h:%d", b->left, b->top, b->width, b->height);
    //ESP_LOGI(TAG, "device view   l:%d, t:%d, w:%d, h:%d", v->left, v->top, v->width, v->height);
    //ESP_LOGI(TAG, "out bounds:   l:%d, t:%d, r:%d, b:%d", lt, tp, rt, bt);
    //ESP_LOG_BUFFER_HEXDUMP(TAG, bitmap->data, bitmap->data_size, ESP_LOG_INFO);

    uint8_t *src = bitmap->data;
    int length = ((rt - lt + 1) * config->bpp) >> 3;
    for (int y = tp; y <= bt; y++) {
        uint8_t *dst = (uint8_t *)device->video_mem + (((y * config->width + lt) * config->bpp) >> 3);
        memcpy(dst, src, length);
        src += length;
    }
    return err;
}

static esp_err_t video_init(video_t *device)
{
    esp_err_t r = ESP_OK;
    ESP_ERROR_CHECK(device->video_mem ? ESP_OK : ESP_ERR_INVALID_STATE);
    ESP_ERROR_CHECK(video_device_init(device));
    return r;
}

static esp_err_t video_done(video_t *device)
{
    return ESP_OK;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

esp_err_t video_display_get_config(const display_t *display, int type)
{
    esp_err_t r = ESP_OK;
    ESP_ERROR_CHECK(display ? ESP_OK : ESP_ERR_INVALID_ARG);
    video_t *device = (video_t *)display->device;
    ESP_ERROR_CHECK(device ? ESP_OK : ESP_ERR_INVALID_STATE);
    r = video_get_default_config(&device->config);
    ESP_ERROR_CHECK(r);
    return r;
}

esp_err_t video_display_init(display_t *display, const display_initialization_t *init)
{
    esp_err_t r = ESP_OK;
    ESP_ERROR_CHECK(display ? ESP_OK : ESP_ERR_INVALID_ARG);
    video_t *device = (video_t *)display->device;
    ESP_ERROR_CHECK(device ? ESP_OK : ESP_ERR_INVALID_STATE);

    video_config_t *config = &device->config;

    display_rectangle_t *b = &display->bounds;
    display_rectangle_t *v = &config->view;
    int width, height;
    if (init->orientation == DISPLAY_LANDSCAPE) {
        int t = v->left;
        v->left = v->top;
        v->top = t;
        t = v->width;
        v->width = v->height;
        v->height = t;
        display->orientation = DISPLAY_LANDSCAPE;
        device->orientation = DISPLAY_LANDSCAPE;

        width = config->height;
        height = config->width;
        if (init->flip_vertically) {
            b->left = width - (v->left + v->width);
            b->width = v->width;
        }
        else {
            b->left = v->left;
            b->width = v->width;
        }
        if (init->flip_horizontally) {
            b->top = height - (v->top + v->height);
            b->height = v->height;
        }
        else {
            b->top = v->top;
            b->height = v->height;
        }
    }
    else {
        display->orientation = DISPLAY_PORTRAIT;
        device->orientation = DISPLAY_PORTRAIT;

        width = config->width;
        height = config->height;
        if (init->flip_vertically) {
            b->left = width - (v->left + v->width);
            b->width = v->width;
            //config->scan_reverse = true;
        }
        else {
            b->left = v->left;
            b->width = v->width;
            //config->scan_reverse = false;
        }
        if (init->flip_horizontally) {
            b->top = height - (v->top + v->height);
            b->height = v->height;
            //config->segment_remap = true;
        }
        else {
            b->top = v->top;
            b->height = v->height;
            //config->segment_remap = false;
        }
    }
    display->hardware = &config->hardware;
    config->bpp = display->hardware->bpp;

    //ESP_LOGI(TAG, "display bounds l:%d, t:%d, w: %d, h: %d", b->left, b->top, b->width, b->height);

    ESP_ERROR_CHECK(video_init(device));
    return r;
}

esp_err_t video_display_done(display_t *display)
{
    esp_err_t r = ESP_OK;
    if (display) {
        video_t *device = (video_t *)display->device;
        r = video_done(device);
        ESP_ERROR_CHECK(r);
        if (display->device)
            free(display->device);
        free(display);
    }
    return r;
}

esp_err_t video_display_refresh(const display_bitmap_t *bitmap)
{
    esp_err_t r = ESP_OK;
    ESP_ERROR_CHECK(bitmap ? ESP_OK : ESP_ERR_INVALID_ARG);
    const display_t *display = bitmap->display;
    ESP_ERROR_CHECK(display ? ESP_OK : ESP_ERR_INVALID_STATE);
    video_t *device = (video_t *)display->device;
    ESP_ERROR_CHECK(device ? ESP_OK : ESP_ERR_INVALID_STATE);
    ESP_ERROR_CHECK(video_bitmap_output(device, bitmap));
    return r;
}

esp_err_t video_create(display_t **pdisplay)
{
    esp_err_t r = ESP_OK;
    ESP_ERROR_CHECK(pdisplay ? ESP_OK : ESP_ERR_INVALID_ARG);
    display_t *display = (display_t *)malloc(sizeof(display_t));
    ESP_ERROR_CHECK(display ? ESP_OK : ESP_ERR_NO_MEM);
    bzero(display, sizeof(display_t));
    display->device = (video_t *)malloc(sizeof(video_t));
    ESP_ERROR_CHECK(display->device ? ESP_OK : ESP_ERR_NO_MEM);
    bzero(display->device, sizeof(video_t));

    display->background.get = NULL;
    display->background.args = NULL;
    display->get_config = video_display_get_config;
    display->init = video_display_init;
    display->done = video_display_done;
    display->refresh = video_display_refresh;
    *pdisplay = display;
    return r;
}
