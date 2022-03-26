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

#define MK85_KBD_MEM_BASE 0x100
#define MK85_KBD_MEM_SIZE 4
#define MK85_KBD_BUF_SIZE 8
#define MK85_KBD_DOWN_TIME 75
#define MK85_KBD_UP_TIME 5

typedef enum {
    MK85_KBD_VK_NO_KEY,
    MK85_KBD_VK_AC,
    MK85_KBD_VK_DEL,
    MK85_KBD_VK_DIV,
    MK85_KBD_VK_9,
    MK85_KBD_VK_6,
    MK85_KBD_VK_PLUS,
    MK85_KBD_VK_INIT,
    MK85_KBD_VK_7,
    MK85_KBD_VK_MUL,
    MK85_KBD_VK_MINUS,
    MK85_KBD_VK_0,
    MK85_KBD_VK_8,
    MK85_KBD_VK_4,
    MK85_KBD_VK_1,
    MK85_KBD_VK_POINT,
    MK85_KBD_VK_5,
    MK85_KBD_VK_2,
    MK85_KBD_VK_EXE,
    MK85_KBD_VK_3,
    MK85_KBD_VK_MODE,
    MK85_KBD_VK_LEFT,
    MK85_KBD_VK_FUNCS,
    MK85_KBD_VK_U,
    MK85_KBD_VK_H,
    MK85_KBD_VK_M,
    MK85_KBD_VK_RIGHT,
    MK85_KBD_VK_FUNCF,
    MK85_KBD_VK_I,
    MK85_KBD_VK_J,
    MK85_KBD_VK_N,
    MK85_KBD_VK_Y,
    MK85_KBD_VK_O,
    MK85_KBD_VK_K,
    MK85_KBD_VK_SPACE,
    MK85_KBD_VK_P,
    MK85_KBD_VK_L,
    MK85_KBD_VK_EQUAL,
    MK85_KBD_VK_ANS,
    MK85_KBD_VK_EE,
    MK85_KBD_VK_W,
    MK85_KBD_VK_S,
    MK85_KBD_VK_F,
    MK85_KBD_VK_Q,
    MK85_KBD_VK_C,
    MK85_KBD_VK_E,
    MK85_KBD_VK_G,
    MK85_KBD_VK_X,
    MK85_KBD_VK_A,
    MK85_KBD_VK_Z,
    MK85_KBD_VK_D,
    MK85_KBD_VK_R,
    MK85_KBD_VK_B,
    MK85_KBD_VK_T,
    MK85_KBD_VK_V,
    MK85_KBD_VK_STOP
} mk85_key_t;

typedef struct {
    uint16_t row;
    uint16_t col;
} mk85_key_info_t;


typedef enum {
    MK85_KBD_STATE_WAIT_KEY,
    MK85_KBD_STATE_DOWN,
    MK85_KBD_STATE_UP,

} mk85_kbd_state_t;


typedef struct {
    mk85_key_t key_buf[MK85_KBD_BUF_SIZE];
    int buf_rd;
    int buf_wr;


    mk85_kbd_state_t state;
    int counter;
    mk85_key_info_t* cur_key;

    uint8_t data[MK85_KBD_MEM_SIZE];
    pdp11_device_t* dev;
} mk85_kbd_t;

typedef void (*mk85_kbd_state_func_t)(mk85_kbd_t* kbd);

pdp11_result_t mk85_kbd_create(mk85_kbd_t** pkbd);
pdp11_result_t mk85_kbd_init(mk85_kbd_t* kbd);
pdp11_result_t mk85_kbd_done(mk85_kbd_t* kbd);
pdp11_result_t mk85_kbd_reset(mk85_kbd_t* kbd);
pdp11_result_t mk85_kbd_step(mk85_kbd_t* kbd);
pdp11_result_t mk85_kbd_set_key(mk85_kbd_t* kbd, mk85_key_t key);


#ifdef __cplusplus
}
#endif
