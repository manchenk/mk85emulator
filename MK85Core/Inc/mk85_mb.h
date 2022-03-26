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
#include "pdp11_cpu.h"
#include "mk85_kbd.h"
#include "mk85_ctrl.h"
#include "mk85_lcd.h"

typedef struct {
    pdp11_cpu_t* cpu;
    pdp11_bus_t* bus;
    mk85_kbd_t* kbd;
    mk85_ctrl_t* ctrl;
    mk85_lcd_t* lcd;
} mk85_mb_t;

pdp11_result_t mk85_mb_create(mk85_mb_t** pmb);
pdp11_result_t mk85_mb_init(mk85_mb_t* mb);
pdp11_result_t mk85_mb_done(mk85_mb_t* mb);
pdp11_result_t mk85_mb_reset(mk85_mb_t* mb);
pdp11_result_t mk85_mb_step(mk85_mb_t* mb);
pdp11_result_t mk85_mb_set_key(mk85_mb_t* mb, mk85_key_t key);

#ifdef __cplusplus
}
#endif
