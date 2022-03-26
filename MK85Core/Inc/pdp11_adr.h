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

#include "pdp11_types.h"

typedef enum pdp11_cpu_addressing {
    PDP11_ADDRESS_DIRECT = 0,
    PDP11_ADDRESS_INDIRECT,
    PDP11_ADDRESS_AUTOINCREMENT,
    PDP11_ADDRESS_AUTOINCREMENT_INDIRECT,
    PDP11_ADDRESS_AUTODECREMENT,
    PDP11_ADDRESS_AUTODECREMENT_INDIRECT,
    PDP11_ADDRESS_INDEX,
    PDP11_ADDRESS_INDEX_INDIRECT
} pdp11_cpu_addressing_t;

struct pdp11_cpu;

uint16_t pdp11_cpu_get_address(struct pdp11_cpu* cpu, int idx, pdp11_cpu_addressing_t mode, bool_t is_word);

#ifdef __cplusplus
}
#endif

