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

#include <functional>
#include <stdexcept>
#include <vector>
#include <cstdint>

static void push_back_bit(std::vector<bool>&b, int bit_len, int val)
{
    /* For example bit_len = 4 aka 1111 */
    for(int i = bit_len - 1  ; i >= 0 ; i--)
    {
        int and_val = 1 << i;
        int res_val = val & and_val;
        b.push_back(res_val == and_val);
    }
}

static int64_t get_part(std::vector<bool> b, int s, int e)
{
    if(e - s > 63 || s > e)
        throw std::bad_function_call();
    if(e >= b.size())
        throw std::range_error("Out of range!");
    int64_t ret = 0;
    for(int i = s ; i <= e ; i++)
    {
        int j = i - s; /* start from 0 */
        ret |= ((int64_t)b.at(i) << (63 - j));
    }
    return ret;
}

extern "C"
{
    void* dh_vector_bool_new()
    {
        return (void*) new std::vector<bool>;
    }

    void dh_vector_bool_append_bit(void* v, int len, int value)
    {
        std::vector<bool>* vec = dynamic_cast<std::vector<bool>*>((std::vector<bool>*)v);
        push_back_bit(*vec, len, value);
    }

    int64_t dh_vector_bool_get_part_from_top(void* v, int start, int end)
    {
        std::vector<bool>* vec = dynamic_cast<std::vector<bool>*>((std::vector<bool>*)v);
        return get_part(*vec, start, end);
    }
}