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
#include "mk85_lcd.h"

#define LCD_RANK_NUM 12
#define LCD_DIGIT_NUM 4

typedef enum {
    APP_LCD_UPDATE_RANK_00  = 0,
    APP_LCD_UPDATE_RANK_01  = 1,
    APP_LCD_UPDATE_RANK_02  = 2,
    APP_LCD_UPDATE_RANK_03  = 3,
    APP_LCD_UPDATE_RANK_04  = 4,
    APP_LCD_UPDATE_RANK_05  = 5,
    APP_LCD_UPDATE_RANK_06  = 6,
    APP_LCD_UPDATE_RANK_07  = 7,
    APP_LCD_UPDATE_RANK_08  = 8,
    APP_LCD_UPDATE_RANK_09  = 9,
    APP_LCD_UPDATE_RANK_10  = 10,
    APP_LCD_UPDATE_RANK_11  = 11,
    APP_LCD_UPDATE_DIGIT_0  = 12,
    APP_LCD_UPDATE_DIGIT_1  = 13,
    APP_LCD_UPDATE_DIGIT_2  = 14,
    APP_LCD_UPDATE_DIGIT_3  = 15,
    APP_LCD_UPDATE_PIC_EXT  = 16,
    APP_LCD_UPDATE_PIC_S    = 17,
    APP_LCD_UPDATE_PIC_F    = 18,
    APP_LCD_UPDATE_PIC_RUN  = 19,
    APP_LCD_UPDATE_PIC_WRT  = 20,
    APP_LCD_UPDATE_PIC_DEG  = 21,
    APP_LCD_UPDATE_PIC_RAD  = 22,
    APP_LCD_UPDATE_PIC_GRA  = 23,
    APP_LCD_UPDATE_PIC_TR   = 24,
    APP_LCD_UPDATE_PIC_STOP = 25,
    APP_LCD_UPDATE_MAX  = 26
} app_lcd_update_t;


typedef struct {
    display_t *display;
    mk85_lcd_info_t *draw_info;
    app_lcd_update_t cur_item;
    display_bitmap_t *bmp[APP_LCD_UPDATE_MAX];
} app_lcd_t;

esp_err_t app_lcd_create(app_lcd_t **plcd);
esp_err_t app_lcd_init(app_lcd_t *lcd);
esp_err_t app_lcd_done(app_lcd_t *lcd);
esp_err_t app_lcd_step(app_lcd_t *lcd);


