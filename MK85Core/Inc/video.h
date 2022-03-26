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

#include "display.h"

#define VIDEO_DEVICE_HEIGHT 320
#define VIDEO_DEFAULT_WIDTH 240
#define VIDEO_DEFAULT_HEIGHT 320
#define VIDEO_DEFAULT_BPP 32

typedef enum video_type {
    VIDEO_TYPE_320x240 = 0
} video_type_t;

typedef struct video_config {
    display_rectangle_t view;
    display_hardware_config_t hardware;
    uint32_t width;
    uint32_t height;
    uint32_t bpp;
} video_config_t;


typedef struct video {
    video_config_t config;
    display_orientation_t orientation;
    void *video_mem;
} video_t;

esp_err_t video_create(display_t **pdisp);
