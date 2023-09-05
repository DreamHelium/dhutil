/*  dh_general_isoc - General ISO-C class
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

#ifndef DH_GENERAL_ISOC_H
#define DH_GENERAL_ISOC_H

#include <glib-object.h>

G_BEGIN_DECLS

#define DH_TYPE_GENERAL_ISOC dh_general_isoc_get_type()
G_DECLARE_FINAL_TYPE (DhGeneralIsoc, dh_general_isoc, DH, GENERAL_ISOC, GObject)

DhGeneralIsoc* dh_general_isoc_new();

G_END_DECLS

#endif
