/*  dh_generaliface - General Interface
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

#ifndef DH_GENERALIFACE_H
#define DH_GENERALIFACE_H

#include <glib-object.h>
#include <gobject/gtype.h>

G_BEGIN_DECLS

#define DH_TYPE_GENERAL dh_general_get_type()
G_DECLARE_INTERFACE (DhGeneral, dh_general, DH, GENERAL, GObject)

struct _DhGeneralInterface{
    GTypeInterface parent;

    void (*new_win)(DhGeneral* self, gboolean need_new_win_in_cmd);
    int (*v_printer)(DhGeneral* self, const char* str, va_list va);
    int (*option_printer)(DhGeneral* self, int opt, const char* str);
    int (*selector)(DhGeneral* self, const char* tip, int opt, const char* opt_name, va_list va);

};

int dh_printf(DhGeneral* self, const char* str, ...);
int dh_option_printer(DhGeneral* self, int opt, const char* str);
int dh_selector(DhGeneral* self, const char* tip, int opt, const char* opt_name, ...);
void dh_new_win(DhGeneral* self, gboolean need_new_win_in_cmd);

G_END_DECLS

#endif
