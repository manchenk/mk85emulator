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
#include <malloc.h>
#include <string.h>
#include "mk85_mb.h"

pdp11_result_t mk85_mb_create(mk85_mb_t** pmb)
{
    PDP11_CHECK_RESULT(pmb ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    mk85_mb_t* mb = (mk85_mb_t*)malloc(sizeof(mk85_mb_t));
    PDP11_CHECK_RESULT(mb ? PDP11_RESULT_OK : PDP11_RESULT_NO_MEM);

    memset(mb, 0, sizeof(mk85_mb_t));

    PDP11_CHECK_RESULT(pdp11_bus_create(&mb->bus));
    PDP11_CHECK_RESULT(pdp11_cpu_create(&mb->cpu));
    PDP11_CHECK_RESULT(mk85_kbd_create(&mb->kbd));
    PDP11_CHECK_RESULT(mk85_lcd_create(&mb->lcd));
    PDP11_CHECK_RESULT(mk85_ctrl_create(&mb->ctrl));

    *pmb = mb;
    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_mb_init(mk85_mb_t* mb)
{
    PDP11_CHECK_RESULT(mb ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    mb->lcd->dev = *pdp11_bus_new_device(mb->bus);
    PDP11_CHECK_RESULT(mk85_lcd_init(mb->lcd));

    mb->kbd->dev = *pdp11_bus_new_device(mb->bus);
    PDP11_CHECK_RESULT(mk85_kbd_init(mb->kbd));

    mb->ctrl->dev = *pdp11_bus_new_device(mb->bus);
    PDP11_CHECK_RESULT(mk85_ctrl_init(mb->ctrl));

    mb->cpu->bus = mb->bus;
    PDP11_CHECK_RESULT(pdp11_cpu_init(mb->cpu));

    PDP11_CHECK_RESULT(pdp11_bus_init(mb->bus));

    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_mb_done(mk85_mb_t* mb)
{
    PDP11_CHECK_RESULT(mb ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    PDP11_CHECK_RESULT(pdp11_bus_done(mb->bus));
    PDP11_CHECK_RESULT(pdp11_cpu_done(mb->cpu));
    PDP11_CHECK_RESULT(mk85_ctrl_done(mb->ctrl));
    PDP11_CHECK_RESULT(mk85_kbd_done(mb->kbd));
    PDP11_CHECK_RESULT(mk85_lcd_done(mb->lcd));

    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_mb_reset(mk85_mb_t* mb)
{
    PDP11_CHECK_RESULT(mb ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    PDP11_CHECK_RESULT(mk85_lcd_reset(mb->lcd));
    PDP11_CHECK_RESULT(mk85_kbd_reset(mb->kbd));
    PDP11_CHECK_RESULT(mk85_ctrl_reset(mb->ctrl));
    PDP11_CHECK_RESULT(pdp11_bus_reset(mb->bus));
    PDP11_CHECK_RESULT(pdp11_cpu_reset(mb->cpu));

    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_mb_step(mk85_mb_t* mb)
{
    PDP11_CHECK_RESULT(mb ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);

    PDP11_CHECK_RESULT(mk85_kbd_step(mb->kbd));
    PDP11_CHECK_RESULT(mk85_lcd_step(mb->lcd));

    if ((mb->ctrl->data & MK85_CONTROL_STOP_CLOCK) != 0) {
        PDP11_CHECK_RESULT(pdp11_cpu_step(mb->cpu));
    }
    else {
        uint16_t* ptr = (uint16_t*)mb->kbd->data;
        if (ptr[0]) {
            mb->ctrl->data |= MK85_CONTROL_STOP_CLOCK;
            PDP11_CHECK_RESULT(pdp11_cpu_step(mb->cpu));
        }
    }

    return PDP11_RESULT_OK;
}

pdp11_result_t mk85_mb_set_key(mk85_mb_t* mb, mk85_key_t key)
{
    PDP11_CHECK_RESULT(mb ? PDP11_RESULT_OK : PDP11_RESULT_INVALID_ARG);
    if (key == MK85_KBD_VK_STOP)
        mb->cpu->halt = TRUE;
    else
        PDP11_CHECK_RESULT(mk85_kbd_set_key(mb->kbd, key));
    return PDP11_RESULT_OK;
}
