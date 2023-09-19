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

#include "dh_general_isoc.h"
#include "dh_generaliface.h"
#include "dh_string_util.h"
#include <stdio.h>
#include <stdlib.h>

static void dh_general_isoc_interface_init(DhGeneralInterface* iface);

struct _DhIsoc
{
    GObject parent_instance;
};

G_DEFINE_FINAL_TYPE_WITH_CODE(DhIsoc, dh_isoc, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(DH_TYPE_GENERAL, dh_general_isoc_interface_init))

static int dh_vprintf(DhGeneral* self, const char* str, va_list va)
{
    return vprintf(str, va);
}

static int option_printer(DhGeneral* self, int opt, const char* str, va_list va)
{
    int ret = 0;
    ret += printf("[%2d] ", opt);
    ret += vprintf(str, va);
    ret += printf("\n");
    return ret;
}

static int selector(DhGeneral* self, const char* tip, int opt, const char* opt_name, va_list va)
{
    return dh_default_selector(tip, opt, opt_name);
}

static void new_win(DhGeneral* self, gboolean need_nw_cmd)
{
    if(need_nw_cmd)
    {
#ifdef G_OS_WIN32
        system("cls");
#else
        system("clear");
#endif
    }
}

static void dh_general_isoc_interface_init(DhGeneralInterface* iface)
{
    iface->v_printer = dh_vprintf;
    iface->option_printer = option_printer;
    iface->selector = selector;
    iface->new_win = new_win;
}

static void dh_isoc_init(DhIsoc* self)
{
}

static void dh_isoc_class_init(DhIsocClass* klass)
{
}

DhIsoc* dh_general_isoc_new()
{
    DhIsoc* isoc = g_object_new(DH_TYPE_ISOC, NULL);
    return isoc;
}
