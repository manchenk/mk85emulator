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
#include "mk85_ctrl.h"

static uint8_t mk85_ctrl_read_byte(struct pdp11_device* dev, uint16_t ofs)
{
    uint8_t* data = dev->data;
    return data[ofs];
}

static uint16_t mk85_ctrl_read_word(struct pdp11_device* dev, uint16_t ofs)
{
    uint8_t* data = (uint8_t*)dev->data + ofs;
    return *(uint16_t*)data;
}

static void mk85_ctrl_write_byte(struct pdp11_device* dev, uint16_t ofs, uint8_t val)
{
    uint8_t* data = dev->data;
    data[ofs] = val;
}

static void mk85_ctrl_write_word(struct pdp11_device* dev, uint16_t ofs, uint16_t val)
{
    uint8_t* data = (uint8_t*)dev->data + ofs;
    *(uint16_t*)data = val;
}


pdp11_result_t mk85_ctrl_create(mk85_ctrl_t** pctrl)
{
    PDP11_CHECK_RESULT(pctrl ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    mk85_ctrl_t* ctrl = (mk85_ctrl_t*)malloc(sizeof(mk85_ctrl_t));
    PDP11_CHECK_RESULT(ctrl ? PDP11_RESULT_OK : PDP11_RESULT_NO_MEM);

    memset(ctrl, 0, sizeof(mk85_ctrl_t));

    *pctrl = ctrl;
    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_ctrl_init(mk85_ctrl_t* ctrl)
{
    PDP11_CHECK_RESULT(ctrl ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    PDP11_CHECK_RESULT(ctrl->dev ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_STATE);

    ctrl->data = MK85_CONTROL_STOP_CLOCK;

    ctrl->dev->allow_op = PDP11_OP_READ | PDP11_OP_WRITE;
    ctrl->dev->base = MK85_CTRL_MEM_BASE;
    ctrl->dev->size = MK85_CTRL_MEM_SIZE;
    ctrl->dev->data = &ctrl->data;
    ctrl->dev->read_byte = mk85_ctrl_read_byte;
    ctrl->dev->read_word = mk85_ctrl_read_word;
    ctrl->dev->write_byte = mk85_ctrl_write_byte;
    ctrl->dev->write_word = mk85_ctrl_write_word;

    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_ctrl_done(mk85_ctrl_t* ctrl)
{
    PDP11_CHECK_RESULT(ctrl ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    free(ctrl);

    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_ctrl_reset(mk85_ctrl_t* ctrl)
{
    PDP11_CHECK_RESULT(ctrl ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_ctrl_step(mk85_ctrl_t* ctrl)
{
    PDP11_CHECK_RESULT(ctrl ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    return PDP11_RESULT_OK;
}

