/*  dh_out_ooc - ooc type of dh_out
    Copyright (C) 2022 Dream Helium
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

#include "dh_data_ooc.h"
#include <stdint.h>

#define SET_AND_GET_INT(num) \
void dh_data_set_int##num(DhData* data, gint##num val) \
{ \
   g_value_set_int64(&(data->value), val); \
   data->byte = (num / 8); \
} \
gint##num dh_data_get_int##num(DhData* data) \
{ \
    g_return_val_if_fail(data->byte == (num / 8), 0); \
    return g_value_get_int64(&(data->value));\
}

static gboolean byte_range_check(gint64 val, gint byte);

struct _DhData
{
    GObject parent_instance;

    GValue value;
    guint len;
    guint byte;
};

G_DEFINE_TYPE(DhData, dh_data, G_TYPE_OBJECT)

static void dh_data_class_init(DhDataClass *klass)
{
}

static void dh_data_init(DhData *self)
{
    self->len = 0;
    self->byte = 0;
}

DhData* dh_data_new(GType g_type)
{
    DhData* data = g_object_new(DH_TYPE_DATA, NULL);
    g_value_init( &(data->value), g_type);
    return data;
}

DhData* dh_data_new_int()
{
    return dh_data_new(G_TYPE_INT64);
}

gboolean dh_data_is_int(DhData* data)
{
    return G_VALUE_HOLDS_INT64(&(data->value));
}

gboolean dh_data_is_float(DhData* data)
{
    return G_VALUE_HOLDS_FLOAT(&(data->value));
}

gboolean dh_data_is_double(DhData* data)
{
    return G_VALUE_HOLDS_DOUBLE(&(data->value));
}

gboolean dh_data_is_char(DhData* data)
{
    return G_VALUE_HOLDS_CHAR(&(data->value));
}

gboolean dh_data_is_string(DhData* data)
{
    return G_VALUE_HOLDS_STRING(&(data->value));
}

gboolean dh_data_is_pointer(DhData* data)
{
    return G_VALUE_HOLDS_POINTER(&(data->value));
}

/** Setter and getter for int* */
SET_AND_GET_INT(8);
SET_AND_GET_INT(16);
SET_AND_GET_INT(32);
SET_AND_GET_INT(64);

/** Setter and getter for char */
void dh_data_set_char(DhData* data, gchar val)
{
    g_value_set_schar(&(data->value), val);
}

gchar dh_data_get_char(DhData* data)
{
    return g_value_get_schar(&(data->value));
}

static gboolean byte_range_check(gint64 num, gint byte)
{
    // 8, 16, 32, 64 bit
    if(byte == 1)
    {
        if(num > INT8_MAX || num < INT8_MIN)
            return 0;
        else return 1;
    }
    else if(byte == 2)
    {
        if(num > INT16_MAX || num < INT16_MIN)
            return 0;
        else return 1;
    }
    else if(byte == 4)
    {
        if(num > INT32_MAX || num < INT32_MIN)
            return 0;
        else return 1;
    }
    else if(byte == 8) return 1;
    else return 0;
}
