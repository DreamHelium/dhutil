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

#ifndef DH_DATA_OOC_H
#define DH_DATA_OOC_H

#include <glib-object.h>

G_BEGIN_DECLS

#define DH_TYPE_DATA dh_data_get_type()
G_DECLARE_FINAL_TYPE(DhData, dh_data , DH ,DATA ,GObject )

/* DhData object intends to use int64/uint64 type + byte
 * variant to represent int8/16/32/64 values, so changing
 * the number is free.
 */


DhData* dh_data_new_int();
DhData* dh_data_new(GType g_type);

gboolean dh_data_is_int(DhData* data);
gboolean dh_data_is_float(DhData* data);
gboolean dh_data_is_double(DhData* data);
gboolean dh_data_is_char(DhData* data);
gboolean dh_data_is_string(DhData* data);
gboolean dh_data_is_pointer(DhData* data);

/** Getter and setter for int* type */

void dh_data_set_int8(DhData* data, gint8 val);
void dh_data_set_int16(DhData* data, gint16 val);
void dh_data_set_int32(DhData* data, gint32 val);
void dh_data_set_int64(DhData* data, gint64 val);

gint8 dh_data_get_int8(DhData* data);
gint16 dh_data_get_int16(DhData* data);
gint32 dh_data_get_int32(DhData* data);
gint64 dh_data_get_int64(DhData* data);

void dh_data_set_char(DhData* data, gchar val);
gchar dh_data_get_char(DhData* data);


G_END_DECLS

#endif /* DH_DATA_OOC_H */
