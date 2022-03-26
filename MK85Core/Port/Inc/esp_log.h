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
#ifndef COMPONENTS_PORT_INC_ESP_LOG_H_
#define COMPONENTS_PORT_INC_ESP_LOG_H_

#define ESP_LOGI(tag, format, ... )  printf("%s: "format"\n", tag, ##__VA_ARGS__)
#define ESP_LOGD(tag, format, ... )  printf("%s: "format"\n", tag, ##__VA_ARGS__)

#endif /* COMPONENTS_PORT_INC_ESP_LOG_H_ */
