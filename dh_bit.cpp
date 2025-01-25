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
#include <glib.h>

static void push_back_bit(std::vector<bool>&b, int bit_len, int64_t val)
{
    /* For example bit_len = 4 aka 1111 */
    /* abcd -> [0]d [1]c [2]b [3]a */
    for(int i = bit_len - 1  ; i >= 0 ; i--)
    {
        int64_t and_val = 1 << i;
        int64_t res_val = val & and_val;
        bool value = (and_val == res_val);
        b.push_back(value);
    }
}

static int64_t get_part(std::vector<bool> b, int s, int e)
{
    int size = b.size();
    if(e - s > 63 || s > e)
        throw std::bad_function_call();
    if(e >= b.size())
        throw std::range_error("Out of range!");
    int64_t ret = 0;
    // std::bitset<64> bits;
    // for(int i = s ; i < (s + 64) ; i++)
    // {
    //     int j = 63 + s - i;
    //     if(i <= e)
    //         bits[j] = b[i];
    // }
    for(int i = s ; i <= e ; i++)
    {
        int j = 63 + s - i; /* start from 0 */
        int64_t m = b[i];
        ret |= (m << j);
    }
    return ret;
}

extern "C"
{
    void* dh_vector_bool_new()
    {
        return (void*) new std::vector<bool>;
    }

    void dh_vector_bool_free(void* v)
    {
        std::vector<bool>* vec = dynamic_cast<std::vector<bool>*>((std::vector<bool>*)v);
        delete vec;
    }

    void dh_vector_bool_append_bit(void* v, int len, int64_t value)
    {
        std::vector<bool>* vec = dynamic_cast<std::vector<bool>*>((std::vector<bool>*)v);
        push_back_bit(*vec, len, value);
    }

    int64_t dh_vector_bool_get_part_from_top(void* v, int start, int end)
    {
        std::vector<bool>* vec = dynamic_cast<std::vector<bool>*>((std::vector<bool>*)v);
        return get_part(*vec, start, end);
    }

    int64_t dh_vector_bool_get_part_by_bit(void* v, int start, int bit)
    {
        return dh_vector_bool_get_part_from_top(v, start, start + bit - 1);
    }
}