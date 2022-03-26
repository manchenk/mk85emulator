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

#define MK85_CTRL_MEM_BASE 0x104
#define MK85_CTRL_MEM_SIZE 2

    typedef enum {
    MK85_CONTROL_STOP_CLOCK = 0x0400,
    MK85_COLTROL_DIV8_CLOCK = 0x0800,
    MK85_COLTROL_POWER_DOWN = 0x1000
} mk85_control_info_t;

typedef struct {
    mk85_control_info_t data;
    pdp11_device_t* dev;
} mk85_ctrl_t;

pdp11_result_t mk85_ctrl_create(mk85_ctrl_t** pctrl);
pdp11_result_t mk85_ctrl_init(mk85_ctrl_t* ctrl);
pdp11_result_t mk85_ctrl_done(mk85_ctrl_t* ctrl);
pdp11_result_t mk85_ctrl_reset(mk85_ctrl_t* ctrl);
pdp11_result_t mk85_ctrl_step(mk85_ctrl_t* ctrl);


#ifdef __cplusplus
}
#endif
