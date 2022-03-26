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
#include <malloc.h>
#include "mk85_lcd.h"

#define MK85_LCD_UPDATE_STEPS 25

typedef void (*mk85_lcd_update_func_t)(mk85_lcd_info_t* info, uint8_t b);

static void mk85_lcd_update_pos00(mk85_lcd_info_t* info, uint8_t b)
{
    info->ext_pic = b & 0x01;
    info->s_pic = b & 0x02;
    info->f_pic = b & 0x04;
    info->updated_int |= MK85_LCD_UPDATE_PIC_EXT | MK85_LCD_UPDATE_PIC_S | MK85_LCD_UPDATE_PIC_F;
    info->update_time = MK85_LCD_UPDATE_STEPS;
}

static void mk85_lcd_update_pos01(mk85_lcd_info_t* info, uint8_t b)
{
    info->run_pic = b & 0x01;
    info->wrt_pic = b & 0x02;
    info->deg_pic = b & 0x10;
    info->updated_int |= MK85_LCD_UPDATE_PIC_RUN | MK85_LCD_UPDATE_PIC_WRT | MK85_LCD_UPDATE_PIC_DEG;
    info->update_time = MK85_LCD_UPDATE_STEPS;
}

static void mk85_lcd_update_pos02(mk85_lcd_info_t* info, uint8_t b)
{
}

static void mk85_lcd_update_pos03(mk85_lcd_info_t* info, uint8_t b)
{
    info->rad_pic = b & 0x01;
    info->updated_int |= MK85_LCD_UPDATE_PIC_RAD;
    info->update_time = MK85_LCD_UPDATE_STEPS;
}

static void mk85_lcd_update_pos04(mk85_lcd_info_t* info, uint8_t b)
{
    info->gra_pic = b & 0x01;
    info->updated_int |= MK85_LCD_UPDATE_PIC_GRA;
    info->update_time = MK85_LCD_UPDATE_STEPS;
}

static void mk85_lcd_update_pos05(mk85_lcd_info_t* info, uint8_t b)
{
    info->tr_pic = b & 0x01;
    info->updated_int |= MK85_LCD_UPDATE_PIC_TR;
    info->update_time = MK85_LCD_UPDATE_STEPS;
}

static void mk85_lcd_update_pos06(mk85_lcd_info_t* info, uint8_t b)
{
    if (b & 0x01) info->digits[0] |= 0x02;
    else info->digits[0] &= ~0x02;
    if (b & 0x02) info->digits[0] |= 0x08;
    else info->digits[0] &= ~0x08;
    if (b & 0x04) info->digits[0] |= 0x10;
    else info->digits[0] &= ~0x10;
    if (b & 0x08) info->digits[0] |= 0x40;
    else info->digits[0] &= ~0x40;
    if (b & 0x10) info->digits[0] |= 0x20;
    else info->digits[0] &= ~0x20;
    info->updated_int |= MK85_LCD_UPDATE_DIGIT_0;
    info->update_time = MK85_LCD_UPDATE_STEPS;
}

static void mk85_lcd_update_pos07(mk85_lcd_info_t* info, uint8_t b)
{
    if (b & 0x01) info->digits[0] |= 0x04;
    else info->digits[0] &= ~0x04;
    if (b & 0x02) info->digits[0] |= 0x01;
    else info->digits[0] &= ~0x01;

    if (b & 0x04) info->digits[1] |= 0x02;
    else info->digits[1] &= ~0x02;
    if (b & 0x08) info->digits[1] |= 0x08;
    else info->digits[1] &= ~0x08;
    if (b & 0x10) info->digits[1] |= 0x10;
    else info->digits[1] &= ~0x10;
    info->updated_int |= MK85_LCD_UPDATE_DIGIT_0 | MK85_LCD_UPDATE_DIGIT_1;
    info->update_time = MK85_LCD_UPDATE_STEPS;
}

static void mk85_lcd_update_pos08(mk85_lcd_info_t* info, uint8_t b)
{
    if (b & 0x01) info->digits[1] |= 0x40;
    else info->digits[1] &= ~0x40;
    if (b & 0x02) info->digits[1] |= 0x20;
    else info->digits[1] &= ~0x20;
    if (b & 0x04) info->digits[1] |= 0x04;
    else info->digits[1] &= ~0x04;
    if (b & 0x08) info->digits[1] |= 0x01;
    else info->digits[1] &= ~0x01;

    if (b & 0x10) info->digits[2] |= 0x02;
    else info->digits[2] &= ~0x02;
    info->updated_int |= MK85_LCD_UPDATE_DIGIT_1 | MK85_LCD_UPDATE_DIGIT_2;
    info->update_time = MK85_LCD_UPDATE_STEPS;
}

static void mk85_lcd_update_pos09(mk85_lcd_info_t* info, uint8_t b)
{
    if (b & 0x01) info->digits[2] |= 0x08;
    else info->digits[2] &= ~0x08;
    if (b & 0x02) info->digits[2] |= 0x10;
    else info->digits[2] &= ~0x10;
    if (b & 0x04) info->digits[2] |= 0x40;
    else info->digits[2] &= ~0x40;
    if (b & 0x08) info->digits[2] |= 0x20;
    else info->digits[2] &= ~0x20;
    if (b & 0x10) info->digits[2] |= 0x04;
    else info->digits[2] &= ~0x04;
    //if (b & 0x20) info->digits[2] |= 0x04;
    //else info->digits[2] &= ~0x04;
    info->updated_int |= MK85_LCD_UPDATE_DIGIT_2;
    info->update_time = MK85_LCD_UPDATE_STEPS;
}

static void mk85_lcd_update_pos10(mk85_lcd_info_t* info, uint8_t b)
{
    if (b & 0x01) info->digits[2] |= 0x01;
    else info->digits[2] &= ~0x01;

    if (b & 0x02) info->digits[3] |= 0x02;
    else info->digits[3] &= ~0x02;
    if (b & 0x04) info->digits[3] |= 0x08;
    else info->digits[3] &= ~0x08;
    if (b & 0x08) info->digits[3] |= 0x10;
    else info->digits[3] &= ~0x10;
    if (b & 0x10) info->digits[3] |= 0x40;
    else info->digits[3] &= ~0x40;
    info->updated_int |= MK85_LCD_UPDATE_DIGIT_2 | MK85_LCD_UPDATE_DIGIT_3;
    info->update_time = MK85_LCD_UPDATE_STEPS;
}

static void mk85_lcd_update_pos11(mk85_lcd_info_t* info, uint8_t b)
{
    if (b & 0x01) info->digits[3] |= 0x20;
    else info->digits[3] &= ~0x20;
    if (b & 0x02) info->digits[3] |= 0x04;
    else info->digits[3] &= ~0x04;
    if (b & 0x04) info->digits[3] |= 0x01;
    else info->digits[3] &= ~0x01;
    info->stop_pic = b & 0x08;
    info->updated_int |= MK85_LCD_UPDATE_DIGIT_3;
    info->update_time = MK85_LCD_UPDATE_STEPS;
}

static void mk85_lcd_update_pos12(mk85_lcd_info_t* info, uint8_t b)
{
    info->cursor_shape = b & 0x10;
    uint8_t pos = b & 0x0f;
    if (pos != info->cursor_pos) {
        if (pos < 12) {
            info->updated_int |= 1 << pos;
            info->update_time = MK85_LCD_UPDATE_STEPS;
        }
        if (info->cursor_pos < 12) {
            info->updated_int |= 1 << info->cursor_pos; 
            info->update_time = MK85_LCD_UPDATE_STEPS;
        }
        info->cursor_pos = pos;
    }
}

static void mk85_lcd_update(mk85_lcd_t* lcd, uint16_t ofs, uint8_t b)
{
    int charPos = ofs >> 3;
    int line = ofs & 7;
    if (line > 0) {
        int i;
        int mask = 1 << (line - 1);
        int x = charPos * 5;
        for (i = 0; i < 5; ++i, ++x, b >>= 1) {
            if (b & 1) lcd->draw_info.pixels[x] |= mask;
            else lcd->draw_info.pixels[x] &= ~mask;
        }
        if (charPos < 12) {
            lcd->draw_info.updated_int |= (1 << charPos);
            lcd->draw_info.update_time = MK85_LCD_UPDATE_STEPS;
        }

    }
    else {
        static mk85_lcd_update_func_t funcs[] = {
            mk85_lcd_update_pos00,
            mk85_lcd_update_pos01,
            mk85_lcd_update_pos02,
            mk85_lcd_update_pos03,
            mk85_lcd_update_pos04,
            mk85_lcd_update_pos05,
            mk85_lcd_update_pos06,
            mk85_lcd_update_pos07,
            mk85_lcd_update_pos08,
            mk85_lcd_update_pos09,
            mk85_lcd_update_pos10,
            mk85_lcd_update_pos11,
            mk85_lcd_update_pos12
        };
        if (charPos < 13) {
            funcs[charPos](&lcd->draw_info, b);
        }
    }
}


static uint8_t mk85_lcd_read_byte(struct pdp11_device* dev, uint16_t ofs)
{
    return 0xff;
}

static uint16_t mk85_lcd_read_word(struct pdp11_device* dev, uint16_t ofs)
{
    return 0xffff;
}

static void mk85_lcd_write_byte(struct pdp11_device* dev, uint16_t ofs, uint8_t val)
{
    mk85_lcd_t* lcd = (mk85_lcd_t*)dev->data;
    mk85_lcd_update(lcd, ofs, val);
}

static void mk85_lcd_write_word(struct pdp11_device* dev, uint16_t ofs, uint16_t val)
{
    mk85_lcd_t* lcd = (mk85_lcd_t*)dev->data;
    mk85_lcd_update(lcd, ofs, (uint8_t)val);
    mk85_lcd_update(lcd, ofs | 1, val >> 8);
}

pdp11_result_t mk85_lcd_create(mk85_lcd_t** plcd)
{
    PDP11_CHECK_RESULT(plcd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    mk85_lcd_t* lcd = (mk85_lcd_t*)malloc(sizeof(mk85_lcd_t));
    PDP11_CHECK_RESULT(lcd ? PDP11_RESULT_OK : PDP11_RESULT_NO_MEM);

    memset(lcd, 0, sizeof(mk85_lcd_t));

    *plcd = lcd;
    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_lcd_init(mk85_lcd_t* lcd)
{
    PDP11_CHECK_RESULT(lcd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    PDP11_CHECK_RESULT(lcd->dev ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_STATE);

    lcd->dev->allow_op = PDP11_OP_WRITE;
    lcd->dev->base = MK85_LCD_MEM_BASE;
    lcd->dev->size = MK85_LCD_MEM_SIZE;
    lcd->dev->data = lcd;
    lcd->dev->read_byte = mk85_lcd_read_byte;
    lcd->dev->read_word = mk85_lcd_read_word;
    lcd->dev->write_byte = mk85_lcd_write_byte;
    lcd->dev->write_word = mk85_lcd_write_word;

    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_lcd_done(mk85_lcd_t* lcd)
{
    PDP11_CHECK_RESULT(lcd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    free(lcd);

    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_lcd_reset(mk85_lcd_t* lcd)
{
    PDP11_CHECK_RESULT(lcd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_lcd_step(mk85_lcd_t* lcd)
{
    PDP11_CHECK_RESULT(lcd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    mk85_lcd_info_t *info = &lcd->draw_info;
    if (info->update_time == 0) {
        if (info->updated_int) {
            info->updated |= info->updated_int;
            info->updated_int = 0;
        }
    }
    else {
        info->update_time--;
    }
 
    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_lcd_cursor(mk85_lcd_t* lcd)
{
    PDP11_CHECK_RESULT(lcd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    lcd->draw_info.cur_on = !lcd->draw_info.cur_on;
    if (lcd->draw_info.cursor_pos < 12) {
        lcd->draw_info.updated_int |= (1 << lcd->draw_info.cursor_pos);
        lcd->draw_info.update_time = MK85_LCD_UPDATE_STEPS;
    }

    return PDP11_RESULT_OK;
}

