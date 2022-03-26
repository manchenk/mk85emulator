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
#include "mk85_kbd.h"

static mk85_key_info_t mk85_keys[] = {
    { 0, 0x000 }, // MK85_KBD_VK_NO_KEY
    { 2, 0x00c }, // MK85_KBD_VK_AC
    { 2, 0x014 }, // MK85_KBD_VK_DEL
    { 2, 0x024 }, // MK85_KBD_VK_DIV
    { 2, 0x044 }, // MK85_KBD_VK_9
    { 2, 0x084 }, // MK85_KBD_VK_6
    { 2, 0x104 }, // MK85_KBD_VK_PLUS
    { 2, 0x018 }, // MK85_KBD_VK_INIT
    { 2, 0x028 }, // MK85_KBD_VK_7
    { 2, 0x048 }, // MK85_KBD_VK_MUL
    { 2, 0x088 }, // MK85_KBD_VK_MINUS
    { 2, 0x108 }, // MK85_KBD_VK_0
    { 2, 0x030 }, // MK85_KBD_VK_8
    { 2, 0x050 }, // MK85_KBD_VK_4
    { 2, 0x090 }, // MK85_KBD_VK_1
    { 2, 0x110 }, // MK85_KBD_VK_POINT
    { 2, 0x060 }, // MK85_KBD_VK_5
    { 2, 0x0a0 }, // MK85_KBD_VK_2
    { 2, 0x120 }, // MK85_KBD_VK_EXE
    { 2, 0x0c0 }, // MK85_KBD_VK_3
    { 4, 0x00c }, // MK85_KBD_VK_MODE
    { 4, 0x014 }, // MK85_KBD_VK_LEFT
    { 4, 0x024 }, // MK85_KBD_VK_FUNCS
    { 4, 0x044 }, // MK85_KBD_VK_U
    { 4, 0x084 }, // MK85_KBD_VK_H
    { 4, 0x104 }, // MK85_KBD_VK_M
    { 4, 0x018 }, // MK85_KBD_VK_RIGHT
    { 4, 0x028 }, // MK85_KBD_VK_FUNCF
    { 4, 0x048 }, // MK85_KBD_VK_I
    { 4, 0x088 }, // MK85_KBD_VK_J
    { 4, 0x108 }, // MK85_KBD_VK_N
    { 4, 0x030 }, // MK85_KBD_VK_Y
    { 4, 0x050 }, // MK85_KBD_VK_O
    { 4, 0x090 }, // MK85_KBD_VK_K
    { 4, 0x110 }, // MK85_KBD_VK_SPACE
    { 4, 0x060 }, // MK85_KBD_VK_P
    { 4, 0x0a0 }, // MK85_KBD_VK_L
    { 4, 0x120 }, // MK85_KBD_VK_EQUAL
    { 4, 0x0c0 }, // MK85_KBD_VK_ANS
    { 4, 0x140 }, // MK85_KBD_VK_EE
    { 8, 0x00c }, // MK85_KBD_VK_W
    { 8, 0x014 }, // MK85_KBD_VK_S
    { 8, 0x024 }, // MK85_KBD_VK_F
    { 8, 0x044 }, // MK85_KBD_VK_Q
    { 8, 0x084 }, // MK85_KBD_VK_C
    { 8, 0x018 }, // MK85_KBD_VK_E
    { 8, 0x028 }, // MK85_KBD_VK_G
    { 8, 0x048 }, // MK85_KBD_VK_X
    { 8, 0x088 }, // MK85_KBD_VK_A
    { 8, 0x030 }, // MK85_KBD_VK_Z
    { 8, 0x050 }, // MK85_KBD_VK_D
    { 8, 0x090 }, // MK85_KBD_VK_R
    { 8, 0x060 }, // MK85_KBD_VK_B
    { 8, 0x0a0 }, // MK85_KBD_VK_T
    { 8, 0x0c0 }, // MK85_KBD_VK_V
};

static void mk85_kbd_set_columns(mk85_kbd_t* kbd)
{
    uint16_t* ptr = (uint16_t*)kbd->data;
    if (ptr[1] & kbd->cur_key->row) {
        ptr[0] = kbd->cur_key->col;
    }
    else {
        ptr[0] = 0;
    }
}


static uint8_t mk85_kbd_read_byte(struct pdp11_device* dev, uint16_t ofs)
{
    mk85_kbd_t* kbd = (mk85_kbd_t*)dev->data;
    return kbd->data[ofs];
}

static uint16_t mk85_kbd_read_word(struct pdp11_device* dev, uint16_t ofs)
{
    mk85_kbd_t* kbd = (mk85_kbd_t*)dev->data;
    uint8_t* data = kbd->data + ofs;
    return *(uint16_t*)data;
}

static void mk85_kbd_write_byte(struct pdp11_device* dev, uint16_t ofs, uint8_t val)
{
    mk85_kbd_t* kbd = (mk85_kbd_t*)dev->data;
    kbd->data[ofs] = val;

    mk85_kbd_set_columns(kbd);
}

static void mk85_kbd_write_word(struct pdp11_device* dev, uint16_t ofs, uint16_t val)
{
    mk85_kbd_t* kbd = (mk85_kbd_t*)dev->data;
    uint8_t* data = kbd->data + ofs;
    *(uint16_t*)data = val;

    mk85_kbd_set_columns(kbd);
}


pdp11_result_t mk85_kbd_create(mk85_kbd_t** pkbd)
{
    PDP11_CHECK_RESULT(pkbd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    mk85_kbd_t* kbd = (mk85_kbd_t*)malloc(sizeof(mk85_kbd_t));
    PDP11_CHECK_RESULT(kbd ? PDP11_RESULT_OK : PDP11_RESULT_NO_MEM);

    memset(kbd, 0, sizeof(mk85_kbd_t));

    *pkbd = kbd;
    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_kbd_init(mk85_kbd_t* kbd)
{
    PDP11_CHECK_RESULT(kbd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    PDP11_CHECK_RESULT(kbd->dev ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_STATE);

    kbd->dev->allow_op = PDP11_OP_READ | PDP11_OP_WRITE;
    kbd->dev->base = MK85_KBD_MEM_BASE;
    kbd->dev->size = MK85_KBD_MEM_SIZE;
    kbd->dev->data = kbd;
    kbd->dev->read_byte = mk85_kbd_read_byte;
    kbd->dev->read_word = mk85_kbd_read_word;
    kbd->dev->write_byte = mk85_kbd_write_byte;
    kbd->dev->write_word = mk85_kbd_write_word;

    kbd->state = MK85_KBD_STATE_WAIT_KEY;
    kbd->cur_key = &mk85_keys[MK85_KBD_VK_NO_KEY];

    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_kbd_done(mk85_kbd_t* kbd)
{
    PDP11_CHECK_RESULT(kbd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    free(kbd);
    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_kbd_reset(mk85_kbd_t* kbd)
{
    PDP11_CHECK_RESULT(kbd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    return PDP11_RESULT_OK;
}

// MK85_KBD_STATE_WAIT_KEY
static void mk85_kbd_state_wait_key(mk85_kbd_t* kbd)
{
    if (kbd->buf_rd != kbd->buf_wr) {
        mk85_key_t key = kbd->key_buf[kbd->buf_rd];
        kbd->buf_rd += 1;
        kbd->buf_rd &= (MK85_KBD_BUF_SIZE - 1);

        if (key != MK85_KBD_VK_STOP) {
            kbd->cur_key = &mk85_keys[key];
            kbd->counter = MK85_KBD_DOWN_TIME;
            kbd->state = MK85_KBD_STATE_DOWN;
            mk85_kbd_set_columns(kbd);
        }
    }
}

// MK85_KBD_STATE_DOWN
static void mk85_kbd_state_down(mk85_kbd_t* kbd)
{
    if (--kbd->counter == 0) {
        kbd->cur_key = &mk85_keys[MK85_KBD_VK_NO_KEY];
        kbd->counter = MK85_KBD_UP_TIME;
        kbd->state = MK85_KBD_STATE_UP;
        mk85_kbd_set_columns(kbd);
    }
}

// MK85_KBD_STATE_UP
static void mk85_kbd_state_up(mk85_kbd_t* kbd)
{
    if (--kbd->counter == 0) {
        kbd->state = MK85_KBD_STATE_WAIT_KEY;
    }
}


static const mk85_kbd_state_func_t mk85_kbd_states[] = {
    mk85_kbd_state_wait_key,    // MK85_KBD_STATE_WAIT_KEY
    mk85_kbd_state_down,        // MK85_KBD_STATE_DOWN
    mk85_kbd_state_up           // MK85_KBD_STATE_UP
};

pdp11_result_t mk85_kbd_step(mk85_kbd_t* kbd)
{
    PDP11_CHECK_RESULT(kbd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    mk85_kbd_state_func_t func = mk85_kbd_states[kbd->state];
    func(kbd);

    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_kbd_set_key(mk85_kbd_t* kbd, mk85_key_t key)
{
    PDP11_CHECK_RESULT(kbd ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    kbd->key_buf[kbd->buf_wr] = key;
    kbd->buf_wr += 1;
    kbd->buf_wr &= (MK85_KBD_BUF_SIZE - 1);

    return PDP11_RESULT_OK;
}
