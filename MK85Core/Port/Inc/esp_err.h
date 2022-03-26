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
#ifndef COMPONENTS_PORT_INC_ESP_ERR_H_
#define COMPONENTS_PORT_INC_ESP_ERR_H_


typedef enum {
    ESP_OK = 0,
    ESP_FAIL = -1,
    ESP_ERR_INVALID_ARG = -2,
    ESP_ERR_INVALID_STATE = -3,
    ESP_ERR_NO_MEM = -4
} esp_err_t;


#ifndef DEBUG
#define ESP_ERROR_CHECK(x) do {           \
        esp_err_t __err_rc = (x);         \
        (void) sizeof(__err_rc);          \
    } while(0)
#else
#include <stdlib.h>
#include <stdio.h>
#define ESP_ERROR_CHECK(x) do {                            \
        esp_err_t __err_rc = (x);                          \
        if (__err_rc != ESP_OK) {                          \
            printf("Assert at %s:%d", __FILE__, __LINE__); \
        	exit(1);                                       \
        }                                                  \
    } while(0)
#endif

#include "stm32f4xx_hal.h"
#define esp_timer_get_time() (HAL_GetTick()*1000)

#endif /* COMPONENTS_PORT_INC_ESP_ERR_H_ */
