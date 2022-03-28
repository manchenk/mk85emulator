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
#include "app.h"

#include "esp_log.h"
#include "video.h"
#include "mk85_mb.h"
#include "rom.h"
#include "ram.h"

static const char __attribute__((unused)) *TAG = "app";

static void app_init_oled(app_t *app)
{
    ESP_ERROR_CHECK(video_create(&app->oled));
    ESP_ERROR_CHECK(display_get_config(app->oled, VIDEO_TYPE_320x240));
    display_t *oled = app->oled;
    video_t *device = (video_t *)oled->device;
    device->video_mem = app->video_memory;
    display_initialization_t init = {
        orientation: DISPLAY_PORTRAIT,
        flip_vertically: false,
        flip_horizontally: false
    };
    ESP_ERROR_CHECK(display_init(oled, &init));

    app->lcd->draw_info = &app->core->lcd->draw_info;
    app->lcd->display = oled;
    ESP_ERROR_CHECK(app_lcd_init(app->lcd));
}

static void app_init_calc(app_t *app)
{
    app->core->bus->rom_size = sizeof(rom_bin);
    app->core->bus->rom = rom_bin;
    app->core->bus->ram_size = sizeof(ram_bin);
    app->core->bus->ram = ram_bin;

    ESP_ERROR_CHECK(mk85_mb_init(app->core));
    ESP_ERROR_CHECK(mk85_mb_reset(app->core));
}

esp_err_t app_create(app_t **papp)
{
    ESP_ERROR_CHECK(papp ? ESP_OK : ESP_ERR_INVALID_ARG);
    app_t *app = (app_t *)malloc(sizeof(app_t));
    ESP_ERROR_CHECK(app ? ESP_OK : ESP_ERR_NO_MEM);
    bzero(app, sizeof(app_t));

    ESP_ERROR_CHECK(mk85_mb_create(&app->core));
    ESP_ERROR_CHECK(app_lcd_create(&app->lcd));
    ESP_ERROR_CHECK(touch_kbd_create(&app->keyboard));

    *papp = app;
    return ESP_OK;
}

esp_err_t app_init(app_t *app)
{
    ESP_ERROR_CHECK(app ? ESP_OK : ESP_ERR_INVALID_ARG);
    ESP_ERROR_CHECK(app->video_memory ? ESP_OK : ESP_ERR_INVALID_STATE);

    ESP_ERROR_CHECK(touch_kbd_init(app->keyboard));

    app_init_oled(app);
    app_init_calc(app);

    return ESP_OK;
}

static void app_set_key(app_t *app, mk85_key_t key)
{
    static mk85_key_t prev_key = MK85_KBD_VK_NO_KEY;
    if (prev_key != key) {
        prev_key = key;
        ESP_ERROR_CHECK(mk85_mb_set_key(app->core, key));
    }
}

esp_err_t app_run(app_t *app)
{
    ESP_ERROR_CHECK(app ? ESP_OK : ESP_ERR_INVALID_ARG);

    app_set_key(app, MK85_KBD_VK_NO_KEY);

    uint16_t pc = 0;
    bool_t get_key = FALSE;

    while (1) {
        pc = PDP11_CPU_PC(app->core->cpu);
        get_key = (app->core->ctrl->data & MK85_CONTROL_STOP_CLOCK) == 0;
        if (pc == 0x0820) {
            get_key = TRUE;
        }

        if (get_key) {
            mk85_key_t key = MK85_KBD_VK_NO_KEY;
            ESP_ERROR_CHECK(touch_kbd_get_key(app->keyboard, &key));
            app_set_key(app, key);
        }

        ESP_ERROR_CHECK(mk85_mb_step(app->core));
        ESP_ERROR_CHECK(app_lcd_step(app->lcd));

        uint64_t time = esp_timer_get_time();
        static uint64_t last_time = 0;
        if (last_time < time) {
            ESP_ERROR_CHECK(mk85_lcd_cursor(app->core->lcd));
            last_time = time + 500000;
        }
    }

    return ESP_OK;

}

esp_err_t app_done(app_t *app)
{
    ESP_ERROR_CHECK(app ? ESP_OK : ESP_ERR_INVALID_ARG);

    ESP_ERROR_CHECK(touch_kbd_done(app->keyboard));
    ESP_ERROR_CHECK(app_lcd_done(app->lcd));
    ESP_ERROR_CHECK(mk85_mb_done(app->core));
    free(app);

    return ESP_OK;

}




