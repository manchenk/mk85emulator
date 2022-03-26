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

#define PDP11_BUS_MAX_DEVICE 8
#define MK85_DEFAULT_RAM_SIZE 2048

    struct pdp11_device;

    typedef uint8_t (*pdp11_device_read_byte_t)(struct pdp11_device* dev, uint16_t addr);
    typedef uint16_t (*pdp11_device_read_word_t)(struct pdp11_device* dev, uint16_t addr);
    typedef void (*pdp11_device_write_byte_t)(struct pdp11_device* dev, uint16_t addr, uint8_t val);
    typedef void (*pdp11_device_write_word_t)(struct pdp11_device* dev, uint16_t addr, uint16_t val);

    typedef enum {
        PDP11_OP_READ = 1,
        PDP11_OP_WRITE = 2
    } pdp11_devop_t;


    typedef struct pdp11_device {
        pdp11_devop_t allow_op;
        uint16_t base;
        uint16_t size;
        void* data;
        pdp11_device_read_byte_t read_byte;
        pdp11_device_read_word_t read_word;
        pdp11_device_write_byte_t write_byte;
        pdp11_device_write_word_t write_word;
    } pdp11_device_t;

    typedef struct {
        uint8_t* rom;
        uint8_t* ram;
        int rom_size;
        int ram_size;
        pdp11_device_t** device;
    } pdp11_bus_t;

    pdp11_result_t pdp11_bus_create(pdp11_bus_t** bus);
    pdp11_result_t pdp11_bus_init(pdp11_bus_t* bus);
    pdp11_result_t pdp11_bus_done(pdp11_bus_t* bus);
    pdp11_result_t pdp11_bus_reset(pdp11_bus_t* bus);

    pdp11_device_t* pdp11_bus_new_device(pdp11_bus_t* bus);

    uint16_t pdp11_bus_read_word(pdp11_bus_t* bus, uint16_t addr);
    void pdp11_bus_write_word(pdp11_bus_t* bus, uint16_t addr, uint16_t val);
    uint8_t pdp11_bus_read_byte(pdp11_bus_t* bus, uint16_t addr);
    void pdp11_bus_write_byte(pdp11_bus_t* bus, uint16_t addr, uint8_t val);


#ifdef __cplusplus
}
#endif


