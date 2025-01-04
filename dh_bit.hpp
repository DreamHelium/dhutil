/*  dh_bit - Bit control
    Copyright (C) 2025 Dream Helium
    This file is part of dhutil.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#ifndef DH_BIT_H
#define DH_BIT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* std::vector<bool> is an optimized type for bit storage
 * Thus I might not move this to pure C unless I found a better way
 * ~Maybe will move the part.~ */
void* dh_vector_bool_new();
void dh_vector_bool_append_bit(void* v, int len, int value);
int64_t dh_vector_bool_get_part_from_top(void* v, int start, int end);


#ifdef __cplusplus
}
#endif

#endif /* DH_BIT_H */