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
#include <stdlib.h>
#include <string.h>
#include "touch_kbd.h"

static void touch_kbd_get_state(touch_kbd_t *kbd)
{
    kbd->get_state(&kbd->info);
    kbd->info.touch.x = (kbd->kx * kbd->info.touch.x + kbd->bx) >> 10;
    kbd->info.touch.y = (kbd->ky * kbd->info.touch.y + kbd->by) >> 10;

}

static void touch_kbd_wait_press_state(touch_kbd_t *kbd, bool_t is_pressed)
{
    while(1) {
        touch_kbd_get_state(kbd);
        HAL_Delay(10);
        if (kbd->info.pressed == is_pressed) {
            uint16_t end_time = HAL_GetTick() + 100;
            while (1) {
                touch_kbd_get_state(kbd);
                HAL_Delay(10);
                if (kbd->info.pressed != is_pressed) {
                    break;
                }
                else if (HAL_GetTick() > end_time) {
                    return;
                }
            }
        }
    }
}

static void touch_kbd_get_point(touch_kbd_t *kbd, display_point_t *disp, display_point_t *touch)
{
  /* Draw the cross */
    touch_kbd_point_t pnt_on = {
            point: disp,
            state: TRUE
    };
    kbd->draw_point(&pnt_on);

  touch_kbd_wait_press_state(kbd, TRUE);

  touch->x = kbd->info.touch.x;
  touch->y = kbd->info.touch.y;

  touch_kbd_wait_press_state(kbd, FALSE);

  touch_kbd_point_t pnt_off = {
          point: disp,
          state: FALSE
  };
  kbd->draw_point(&pnt_off);
}


static void touch_kbd_calibration(touch_kbd_t *kbd)
{
//    if (kbd->calibrate_screen)
//        kbd->calibrate_screen();
//    status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
//    if (status != TS_OK) {
//        if (kbd->error_screen)
//            kbd->error_screen();
//        while (1) { }
//    }

    const int delta = 15;
    display_point_t dlt = { x : delta, y : delta };
    display_point_t drb = { kbd->disp_size.width - delta, kbd->disp_size.height - delta};
    display_point_t tlt, trb;
    while (1) {
        kbd->kx = 1 << 10;
        kbd->bx = 0;
        kbd->ky = 1 << 10;
        kbd->by = 0;
        touch_kbd_get_point(kbd, &dlt, &tlt);
        touch_kbd_get_point(kbd, &drb, &trb);
        if ((trb.x - tlt.x) == 0 || (trb.y - tlt.y) == 0)
            continue;
        kbd->kx = ((drb.x - dlt.x) << 10) / (trb.x - tlt.x);
        kbd->bx = (dlt.x << 10) - kbd->kx * tlt.x;
        kbd->ky = ((drb.y - dlt.y) << 10) / (trb.y - tlt.y);
        kbd->by = (dlt.y << 10) - kbd->ky * tlt.y;
        kbd->done = TRUE;
        break;
    }
}

static int touch_kbd_get_block_index(const display_rectangle_t *block, const display_size_t *step, display_point_t *t)
{
    display_point_t p, n;
    int idx = -1;
    for (p.y = block->top, n.y = 0; p.y < block->top + block->height; p.y += step->height, n.y++) {
        if (t->y < p.y) {
            if (n.y > 0) {
                for (p.x = block->left, n.x = 0; p.x < block->left + block->width; p.x += step->width, n.x++) {
                    if (t->x < p.x) {
                        if (n.x > 0) {
                            int cols = block->width / step->width;
                            idx = cols * (n.y - 1) + n.x - 1;
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
    return idx;
}

static mk85_key_t touch_kbd_get_key_int(touch_kbd_t *kbd) {
    const display_rectangle_t block_0 = {
            left: 40,
            top: 6,
            width: 89,
            height: 312
    };
    const display_rectangle_t block_1 = {
            left: 128,
            top: 190,
            width: 112,
            height: 127
    };
    const display_size_t step = { width: 22, height: 31 };

    static const mk85_key_t key_array_0[] = {
        MK85_KBD_VK_Z, MK85_KBD_VK_A, MK85_KBD_VK_Q, MK85_KBD_VK_INIT,
        MK85_KBD_VK_X, MK85_KBD_VK_S, MK85_KBD_VK_W, MK85_KBD_VK_NO_KEY,
        MK85_KBD_VK_C, MK85_KBD_VK_D, MK85_KBD_VK_E, MK85_KBD_VK_NO_KEY,
        MK85_KBD_VK_V, MK85_KBD_VK_F, MK85_KBD_VK_R, MK85_KBD_VK_NO_KEY,
        MK85_KBD_VK_B, MK85_KBD_VK_G, MK85_KBD_VK_T, MK85_KBD_VK_NO_KEY,
        MK85_KBD_VK_N, MK85_KBD_VK_H, MK85_KBD_VK_Y, MK85_KBD_VK_MODE,
        MK85_KBD_VK_M, MK85_KBD_VK_J, MK85_KBD_VK_U, MK85_KBD_VK_LEFT,
        MK85_KBD_VK_SPACE, MK85_KBD_VK_K, MK85_KBD_VK_I, MK85_KBD_VK_RIGHT,
        MK85_KBD_VK_EQUAL, MK85_KBD_VK_L, MK85_KBD_VK_O, MK85_KBD_VK_FUNCS,
        MK85_KBD_VK_EE, MK85_KBD_VK_ANS, MK85_KBD_VK_P, MK85_KBD_VK_FUNCF
    };

    static const mk85_key_t key_array_1[] = {
        MK85_KBD_VK_0, MK85_KBD_VK_1, MK85_KBD_VK_4, MK85_KBD_VK_7,
        MK85_KBD_VK_AC, MK85_KBD_VK_POINT, MK85_KBD_VK_2, MK85_KBD_VK_5,
        MK85_KBD_VK_8, MK85_KBD_VK_DEL, MK85_KBD_VK_EXE, MK85_KBD_VK_3,
        MK85_KBD_VK_6, MK85_KBD_VK_9, MK85_KBD_VK_STOP, MK85_KBD_VK_EXE,
        MK85_KBD_VK_PLUS, MK85_KBD_VK_MINUS, MK85_KBD_VK_MUL, MK85_KBD_VK_DIV
    };

    touch_kbd_get_state(kbd);

    if (kbd->info.pressed) {
        int idx = touch_kbd_get_block_index(&block_0, &step, &kbd->info.touch);
        if (idx >= 0) {
            if (kbd->prv_key == key_array_0[idx]) {
                if (++(kbd->press_counter) > 10) {
                    kbd->cur_key = kbd->prv_key;
                }
            }
            else {
                kbd->press_counter = 0;
                kbd->prv_key = key_array_0[idx];
            }
        }
        else {
            idx = touch_kbd_get_block_index(&block_1, &step, &kbd->info.touch);
            if (idx >= 0) {
                if (kbd->prv_key == key_array_1[idx]) {
                    if (++(kbd->press_counter) > 10) {
                        kbd->cur_key = kbd->prv_key;
                    }
                }
                else {
                    kbd->press_counter = 0;
                    kbd->prv_key = key_array_1[idx];
                }
            }
        }
        kbd->release_counter = 0;
    }
    else {
        if (++(kbd->release_counter) > 100) {
            kbd->prv_key = MK85_KBD_VK_NO_KEY;
            kbd->cur_key = MK85_KBD_VK_NO_KEY;
        }
    }

    return kbd->cur_key;
}

esp_err_t touch_kbd_create(touch_kbd_t **pkbd)
{
    PDP11_CHECK_RESULT(pkbd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    touch_kbd_t* kbd = (touch_kbd_t*)malloc(sizeof(touch_kbd_t));
    PDP11_CHECK_RESULT(kbd ? PDP11_RESULT_OK : PDP11_RESULT_NO_MEM);
    memset(kbd, 0, sizeof(touch_kbd_t));

    *pkbd = kbd;
    return PDP11_RESULT_OK;
}

esp_err_t touch_kbd_init(touch_kbd_t *kbd)
{
    PDP11_CHECK_RESULT(kbd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    PDP11_CHECK_RESULT(kbd->get_state ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_STATE);
    PDP11_CHECK_RESULT(kbd->draw_point ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_STATE);
    PDP11_CHECK_RESULT(kbd->keyboard_screen ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_STATE);
    PDP11_CHECK_RESULT((kbd->disp_size.width && kbd->disp_size.height) ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_STATE);

    kbd->kx = 1 << 10;
    kbd->bx = 0;
    kbd->ky = 1 << 10;
    kbd->by = 0;
    kbd->done = FALSE;
    kbd->info.pressed = FALSE;
    kbd->info.touch.x = 0;
    kbd->info.touch.y = 0;
    kbd->press_counter = 0;
    kbd->release_counter = 0;
    kbd->cur_key = MK85_KBD_VK_NO_KEY;
    kbd->prv_key = MK85_KBD_VK_NO_KEY;

    touch_kbd_calibration(kbd);
    kbd->keyboard_screen();

    return PDP11_RESULT_OK;
}

esp_err_t touch_kbd_get_key(touch_kbd_t *kbd, mk85_key_t *key)
{
    PDP11_CHECK_RESULT(kbd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    PDP11_CHECK_RESULT(key ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    *key = touch_kbd_get_key_int(kbd);
    return PDP11_RESULT_OK;
}
esp_err_t touch_kbd_done(touch_kbd_t *kbd)
{
    PDP11_CHECK_RESULT(kbd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    free(kbd);
    return PDP11_RESULT_OK;
}
