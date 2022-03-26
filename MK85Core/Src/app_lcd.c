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
#include "colors.h"
#include "app_lcd.h"

static const char __attribute__((unused)) *TAG = "app_lcd";

uint8_t app_lcd_pic_s[] = { 0x26, 0x49, 0x49, 0x32 };
uint8_t app_lcd_pic_f[] = { 0x7f, 0x09, 0x09, 0x01 };

display_rectangle_t app_lcd_rectangles[APP_LCD_UPDATE_MAX] = {
    { left: 40, top:  10, width: 24, height: 10 },
    { left: 40, top:  22, width: 24, height: 10 },
    { left: 40, top:  34, width: 24, height: 10 },
    { left: 40, top:  46, width: 24, height: 10 },
    { left: 40, top:  58, width: 24, height: 10 },
    { left: 40, top:  70, width: 24, height: 10 },
    { left: 40, top:  82, width: 24, height: 10 },
    { left: 40, top:  94, width: 24, height: 10 },
    { left: 40, top: 106, width: 24, height: 10 },
    { left: 40, top: 118, width: 24, height: 10 },
    { left: 40, top: 130, width: 24, height: 10 },
    { left: 40, top: 142, width: 24, height: 10 },

    { left: 16, top: 106, width: 16, height:  8 },
    { left: 16, top: 114, width: 16, height:  8 },
    { left: 16, top: 122, width: 16, height:  8 },
    { left: 16, top: 130, width: 16, height:  8 },

    { left:  8, top:  10, width:  8, height: 16 }, // EXT
    { left:  8, top:  30, width:  8, height:  4 }, // S
    { left: 16, top:  30, width:  8, height:  4 }, // F
    { left:  8, top:  38, width:  8, height: 14 }, // RUN
    { left: 16, top:  38, width:  8, height: 18 }, // WRT
    { left:  8, top:  80, width:  8, height: 14 }, // DEG
    { left: 16, top:  80, width:  8, height: 15 }, // RAD
    { left: 24, top:  80, width:  8, height: 16 }, // GRA
    { left:  8, top:  60, width:  8, height: 10 }, // TR
    { left: 16, top:  74, width:  8, height: 20 }, // STOP
};

static int app_lcd_update_rank(const display_point_t *p, const struct display_refresh_info *info, void *color) {
    app_lcd_t *lcd = (app_lcd_t*) info->color.args;
    ESP_ERROR_CHECK(lcd ? ESP_OK : ESP_ERR_INVALID_STATE);
    mk85_lcd_info_t *draw_info = lcd->draw_info;
    ESP_ERROR_CHECK(draw_info ? ESP_OK : ESP_ERR_INVALID_STATE);
    //ESP_LOGI(TAG, "cur_item = %d, p->x = %d, p->y = %d", lcd->cur_item, p->x, p->y);

    uint8_t *data_ptr = draw_info->pixels + lcd->cur_item * MK85_LCD_POS_SIZE;
    uint8_t data = data_ptr[p->y >> 1];
    if (draw_info->cur_on && lcd->cur_item == draw_info->cursor_pos) {
        data = draw_info->cursor_shape ? 0x40 : 0xff;
    }
    *(uint32_t*) color = (data >> (p->x / 3)) & 1 ? 0 : 0xffffffff;
    return 1;

}

static int app_lcd_update_digit(const display_point_t *p, const struct display_refresh_info *info, void *color) {
    static const uint8_t maskx[] = {
        0x01, 0x06, 0x06, 0x06, 0x06, 0x08, 0x30, 0x30,
        0x30, 0x30, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    static const uint8_t masky[] = {
        0x12, 0x49, 0x49, 0x49, 0x49, 0x24, 0x00, 0x00
    };
    app_lcd_t *lcd = (app_lcd_t*) info->color.args;
    ESP_ERROR_CHECK(lcd ? ESP_OK : ESP_ERR_INVALID_STATE);
    mk85_lcd_info_t *draw_info = lcd->draw_info;
    ESP_ERROR_CHECK(draw_info ? ESP_OK : ESP_ERR_INVALID_STATE);

    uint8_t data = draw_info->digits[lcd->cur_item - APP_LCD_UPDATE_DIGIT_0];
    *(uint32_t*) color = data & masky[p->y] & maskx[p->x] ? 0 : 0xffffffff;

    return 1;
}

display_color_callback_t app_lcd_get_color[APP_LCD_UPDATE_MAX] = {
    app_lcd_update_rank,
    app_lcd_update_rank,
    app_lcd_update_rank,
    app_lcd_update_rank,
    app_lcd_update_rank,
    app_lcd_update_rank,
    app_lcd_update_rank,
    app_lcd_update_rank,
    app_lcd_update_rank,
    app_lcd_update_rank,
    app_lcd_update_rank,
    app_lcd_update_rank,
    app_lcd_update_digit,
    app_lcd_update_digit,
    app_lcd_update_digit,
    app_lcd_update_digit,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

typedef void (*app_lcd_get_pic_t)(display_bitmap_t *bmp, mk85_lcd_info_t *info);

static void app_lcd_get_pic_ext(display_bitmap_t *bmp, mk85_lcd_info_t *info) {
    static const uint8_t pic[] = { 
        0x7f, 0x49, 0x49, 0x41, 0x00, 0x63, 0x14, 0x08,
        0x14, 0x63, 0x00, 0x01, 0x01, 0x7f, 0x01, 0x01
    };
    display_point_t p;
    display_color_argb8888_t *ptr = bmp->data;
    for (p.y = 0; p.y < bmp->bounds.height; ++p.y) {
        for (p.x = 0; p.x < bmp->bounds.width; ++p.x) {
            int ofs = p.y * bmp->bounds.width + p.x;
            (ptr++)->argb = info->ext_pic ? (pic[ofs >> 3] & (1 << (ofs & 7)) ? 0 : 0xffffffff) : 0xffffffff;
        }
    }
}

static void app_lcd_get_pic_s(display_bitmap_t *bmp, mk85_lcd_info_t *info) {
    static const uint8_t pic[] = { 0x26, 0x49, 0x49, 0x32 };
    display_point_t p;
    display_color_argb8888_t *ptr = bmp->data;
    for (p.y = 0; p.y < bmp->bounds.height; ++p.y) {
        for (p.x = 0; p.x < bmp->bounds.width; ++p.x) {
            int ofs = p.y * bmp->bounds.width + p.x;
            (ptr++)->argb = info->s_pic ? (pic[ofs >> 3] & (1 << (ofs & 7)) ? 0 : 0xffffffff) : 0xffffffff;
        }
    }
}

static void app_lcd_get_pic_f(display_bitmap_t *bmp, mk85_lcd_info_t *info) {
    static const uint8_t pic[] = { 0x7f, 0x09, 0x09, 0x01 };
    display_point_t p;
    display_color_argb8888_t *ptr = bmp->data;
    for (p.y = 0; p.y < bmp->bounds.height; ++p.y) {
        for (p.x = 0; p.x < bmp->bounds.width; ++p.x) {
            int ofs = p.y * bmp->bounds.width + p.x;
            (ptr++)->argb = info->f_pic ? (pic[ofs >> 3] & (1 << (ofs & 7)) ? 0 : 0xffffffff) : 0xffffffff;
        }
    }
}

static void app_lcd_get_pic_run(display_bitmap_t *bmp, mk85_lcd_info_t *info) {
    static const uint8_t pic[] = { 
        0x7f, 0x09, 0x39, 0x46, 0x00, 0x3f, 0x40, 0x40,
        0x3f, 0x00, 0x7f, 0x06, 0x18, 0x7f
    };
    display_point_t p;
    display_color_argb8888_t *ptr = bmp->data;
    for (p.y = 0; p.y < bmp->bounds.height; ++p.y) {
        for (p.x = 0; p.x < bmp->bounds.width; ++p.x) {
            int ofs = p.y * bmp->bounds.width + p.x;
            (ptr++)->argb = info->run_pic ? (pic[ofs >> 3] & (1 << (ofs & 7)) ? 0 : 0xffffffff) : 0xffffffff;
        }
    }
}

static void app_lcd_get_pic_wrt(display_bitmap_t *bmp, mk85_lcd_info_t *info) {
    static const uint8_t pic[] = { 
        0x03, 0x1c, 0x70, 0x0c, 0x70, 0x1c, 0x03, 0x00,
        0x7f, 0x09, 0x39, 0x46, 0x00, 0x01, 0x01, 0x7f,
        0x01, 0x01
    };
    display_point_t p;
    display_color_argb8888_t *ptr = bmp->data;
    for (p.y = 0; p.y < bmp->bounds.height; ++p.y) {
        for (p.x = 0; p.x < bmp->bounds.width; ++p.x) {
            int ofs = p.y * bmp->bounds.width + p.x;
            (ptr++)->argb = info->wrt_pic ? (pic[ofs >> 3] & (1 << (ofs & 7)) ? 0 : 0xffffffff) : 0xffffffff;
        }
    }
}

static void app_lcd_get_pic_deg(display_bitmap_t *bmp, mk85_lcd_info_t *info) {
    static const uint8_t pic[] = { 
        0x7f, 0x41, 0x41, 0x3e, 0x00, 0x7f, 0x49, 0x49,
        0x41, 0x00, 0x3e, 0x41, 0x51, 0x32
    };
    display_point_t p;
    display_color_argb8888_t *ptr = bmp->data;
    for (p.y = 0; p.y < bmp->bounds.height; ++p.y) {
        for (p.x = 0; p.x < bmp->bounds.width; ++p.x) {
            int ofs = p.y * bmp->bounds.width + p.x;
            (ptr++)->argb = info->deg_pic ? (pic[ofs >> 3] & (1 << (ofs & 7)) ? 0 : 0xffffffff) : 0xffffffff;
        }
    }
}

static void app_lcd_get_pic_rad(display_bitmap_t *bmp, mk85_lcd_info_t *info) {
    static const uint8_t pic[] = {
        0x7f, 0x09, 0x39, 0x46, 0x00, 0x70, 0x1c, 0x13,
        0x1c, 0x70, 0x00, 0x7f, 0x41, 0x41, 0x3e
    };
    display_point_t p;
    display_color_argb8888_t *ptr = bmp->data;
    for (p.y = 0; p.y < bmp->bounds.height; ++p.y) {
        for (p.x = 0; p.x < bmp->bounds.width; ++p.x) {
            int ofs = p.y * bmp->bounds.width + p.x;
            (ptr++)->argb = info->rad_pic ? (pic[ofs >> 3] & (1 << (ofs & 7)) ? 0 : 0xffffffff) : 0xffffffff;
        }
    }
}

static void app_lcd_get_pic_gra(display_bitmap_t *bmp, mk85_lcd_info_t *info) {
    static const uint8_t pic[] = {
        0x3e, 0x41, 0x51, 0x32, 0x00, 0x7f, 0x09, 0x39,
        0x46, 0x00, 0x70, 0x1c, 0x13, 0x1c, 0x70, 0x00
    };
    display_point_t p;
    display_color_argb8888_t *ptr = bmp->data;
    for (p.y = 0; p.y < bmp->bounds.height; ++p.y) {
        for (p.x = 0; p.x < bmp->bounds.width; ++p.x) {
            int ofs = p.y * bmp->bounds.width + p.x;
            (ptr++)->argb = info->gra_pic ? (pic[ofs >> 3] & (1 << (ofs & 7)) ? 0 : 0xffffffff) : 0xffffffff;
        }
    }
}

static void app_lcd_get_pic_tr(display_bitmap_t *bmp, mk85_lcd_info_t *info) {
    static const uint8_t pic[] = {
        0x01, 0x01, 0x7f, 0x01, 0x01, 0x00, 0x7f, 0x09, 0x39, 0x46
    };
    display_point_t p;
    display_color_argb8888_t *ptr = bmp->data;
    for (p.y = 0; p.y < bmp->bounds.height; ++p.y) {
        for (p.x = 0; p.x < bmp->bounds.width; ++p.x) {
            int ofs = p.y * bmp->bounds.width + p.x;
            (ptr++)->argb = info->tr_pic ? (pic[ofs >> 3] & (1 << (ofs & 7)) ? 0 : 0xffffffff) : 0xffffffff;
        }
    }
}

static void app_lcd_get_pic_stop(display_bitmap_t *bmp, mk85_lcd_info_t *info) {
    static const uint8_t pic[] = { 
        0x26, 0x49, 0x49, 0x32, 0x00, 0x01, 0x01, 0x7f,
        0x01, 0x01, 0x00, 0x3e, 0x41, 0x41, 0x3e, 0x00,
        0x7f, 0x09, 0x09, 0x06
    };
    display_point_t p;
    display_color_argb8888_t *ptr = bmp->data;
    for (p.y = 0; p.y < bmp->bounds.height; ++p.y) {
        for (p.x = 0; p.x < bmp->bounds.width; ++p.x) {
            int ofs = p.y * bmp->bounds.width + p.x;
            (ptr++)->argb = info->stop_pic ? (pic[ofs >> 3] & (1 << (ofs & 7)) ? 0 : 0xffffffff) : 0xffffffff;
        }
    }
}

app_lcd_get_pic_t app_lcd_pics[APP_LCD_UPDATE_MAX] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,

    app_lcd_get_pic_ext,
    app_lcd_get_pic_s,
    app_lcd_get_pic_f,
    app_lcd_get_pic_run,
    app_lcd_get_pic_wrt,
    app_lcd_get_pic_deg,
    app_lcd_get_pic_rad,
    app_lcd_get_pic_gra,
    app_lcd_get_pic_tr,
    app_lcd_get_pic_stop
};

esp_err_t app_lcd_create(app_lcd_t **plcd) {
    ESP_ERROR_CHECK(plcd ? ESP_OK : ESP_ERR_INVALID_ARG);
    app_lcd_t *lcd = (app_lcd_t*) malloc(sizeof(app_lcd_t));
    ESP_ERROR_CHECK(lcd ? ESP_OK : ESP_ERR_NO_MEM);
    bzero(lcd, sizeof(app_lcd_t));

    int i;
    for (i = 0; i < APP_LCD_UPDATE_MAX; ++i) {
        ESP_ERROR_CHECK(display_bitmap_create(&lcd->bmp[i]));
    }

    *plcd = lcd;
    return ESP_OK;

}

esp_err_t app_lcd_init(app_lcd_t *lcd) {
    ESP_ERROR_CHECK(lcd ? ESP_OK : ESP_ERR_INVALID_ARG);
    ESP_ERROR_CHECK(lcd->display ? ESP_OK : ESP_ERR_INVALID_STATE);
    ESP_ERROR_CHECK(lcd->draw_info ? ESP_OK : ESP_ERR_INVALID_STATE);
    display_t *display = lcd->display;

    int i;
    display_bitmap_t **bmp = lcd->bmp;
    for (i = 0; i < APP_LCD_UPDATE_MAX; ++i) {
        (*bmp)->bounds = app_lcd_rectangles[i];
        (*bmp)->format = DEVICE_COLOR_ARGB8888;
        ESP_ERROR_CHECK(display_bitmap_init(*bmp, display));
        bmp++;
    }

    return ESP_OK;
}

esp_err_t app_lcd_done(app_lcd_t *lcd) {
    ESP_ERROR_CHECK(lcd ? ESP_OK : ESP_ERR_INVALID_ARG);

    int i;
    for (i = 0; i < APP_LCD_UPDATE_MAX; ++i) {
        ESP_ERROR_CHECK(display_bitmap_done(lcd->bmp[i]));
    }

    free(lcd);
    return ESP_OK;
}

esp_err_t app_lcd_step(app_lcd_t *lcd) {
    ESP_ERROR_CHECK(lcd ? ESP_OK : ESP_ERR_INVALID_ARG);

    mk85_lcd_info_t *draw = lcd->draw_info;
    if (draw->updated) {
        app_lcd_update_t item;
        mk85_lcd_update_t mask = MK85_LCD_UPDATE_RANK_00;
        for (item = 0; item < APP_LCD_UPDATE_MAX; ++item, mask <<= 1) {
            lcd->cur_item = item;
            if (draw->updated & mask) {
                display_bitmap_t *bmp = lcd->bmp[item];

                display_color_callback_t color_func = app_lcd_get_color[item];
                if (color_func) {

                    display_refresh_info_t info = {
                        rectangle: {
                            left: 0,
                            top: 0,
                            width: bmp->bounds.width,
                            height: bmp->bounds.height
                        },
                        bitmap: bmp,
                        color: {
                            format: DISPLAY_COLOR_ARGB8888,
                            args: lcd,
                            get: color_func
                        }
                    };
                    ESP_ERROR_CHECK(display_bitmap_refresh(&info));
                }

                app_lcd_get_pic_t pic_func = app_lcd_pics[item];
                if (pic_func) {
                    pic_func(bmp, draw);
                }
                ESP_ERROR_CHECK(display_refresh(bmp));
            }
        }
        draw->updated = 0;
    }
    return ESP_OK;
}

