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

#ifdef __cplusplus
extern "C" {
#endif

#include "pdp11_bus.h"

#define MK85_LCD_MEM_BASE 0x80
#define MK85_LCD_MEM_SIZE 0x80
#define MK85_LCD_PIX_SIZE 60
#define MK85_LCD_POS_SIZE 5
#define MK85_LCD_DIG_SIZE 4

typedef enum {
    MK85_LCD_UPDATE_RANK_00  = 0x00000001,
    MK85_LCD_UPDATE_RANK_01  = 0x00000002,
    MK85_LCD_UPDATE_RANK_02  = 0x00000004,
    MK85_LCD_UPDATE_RANK_03  = 0x00000008,
    MK85_LCD_UPDATE_RANK_04  = 0x00000010,
    MK85_LCD_UPDATE_RANK_05  = 0x00000020,
    MK85_LCD_UPDATE_RANK_06  = 0x00000040,
    MK85_LCD_UPDATE_RANK_07  = 0x00000080,
    MK85_LCD_UPDATE_RANK_08  = 0x00000100,
    MK85_LCD_UPDATE_RANK_09  = 0x00000200,
    MK85_LCD_UPDATE_RANK_10  = 0x00000400,
    MK85_LCD_UPDATE_RANK_11  = 0x00000800,
    MK85_LCD_UPDATE_DIGIT_0  = 0x00001000,
    MK85_LCD_UPDATE_DIGIT_1  = 0x00002000,
    MK85_LCD_UPDATE_DIGIT_2  = 0x00004000,
    MK85_LCD_UPDATE_DIGIT_3  = 0x00008000,
    MK85_LCD_UPDATE_PIC_EXT  = 0x00010000,
    MK85_LCD_UPDATE_PIC_S    = 0x00020000,
    MK85_LCD_UPDATE_PIC_F    = 0x00040000,
    MK85_LCD_UPDATE_PIC_RUN  = 0x00080000,
    MK85_LCD_UPDATE_PIC_WRT  = 0x00100000,
    MK85_LCD_UPDATE_PIC_DEG  = 0x00200000,
    MK85_LCD_UPDATE_PIC_RAD  = 0x00400000,
    MK85_LCD_UPDATE_PIC_GRA  = 0x00800000,
    MK85_LCD_UPDATE_PIC_TR   = 0x01000000,
    MK85_LCD_UPDATE_PIC_STOP = 0x02000000
} mk85_lcd_update_t;

typedef struct {
    uint8_t pixels[MK85_LCD_PIX_SIZE];
    bool_t cur_on;
    bool_t ext_pic;
    bool_t s_pic;
    bool_t f_pic;
    bool_t run_pic;
    bool_t wrt_pic;
    bool_t deg_pic;
    bool_t rad_pic;
    bool_t gra_pic;
    bool_t tr_pic;
    bool_t stop_pic;
    uint8_t digits[MK85_LCD_DIG_SIZE];
    bool_t cursor_shape;
    uint8_t cursor_pos;
    mk85_lcd_update_t updated_int;
    mk85_lcd_update_t updated;
    int update_time;
} mk85_lcd_info_t;

typedef struct {
    pdp11_device_t* dev;
    mk85_lcd_info_t draw_info;
} mk85_lcd_t;

pdp11_result_t mk85_lcd_create(mk85_lcd_t** plcd);
pdp11_result_t mk85_lcd_init(mk85_lcd_t* lcd);
pdp11_result_t mk85_lcd_done(mk85_lcd_t* lcd);
pdp11_result_t mk85_lcd_reset(mk85_lcd_t* lcd);
pdp11_result_t mk85_lcd_step(mk85_lcd_t* lcd);
pdp11_result_t mk85_lcd_cursor(mk85_lcd_t* lcd);

#ifdef __cplusplus
}
#endif
