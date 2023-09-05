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

struct _DhGeneralIsoc
{
    GObject parent_instance;
};

static int dh_vprintf(DhGeneral* self, const char* str, va_list va)
{
    return vprintf(str, va);
}

static int option_printer(DhGeneral* self, int opt, const char* str)
{
    return dh_default_OptionPrinter(opt, str);
}

static int selector(DhGeneral* self, const char* tip, int opt, const char* opt_name, va_list va)
{
    return dh_default_selector(tip, opt, opt_name);
}

static void dh_general_isoc_interface_init(DhGeneralInterface* iface)
{
    iface->v_printer = dh_vprintf;
    iface->option_printer = option_printer;
    iface->selector = selector;
}

G_DEFINE_FINAL_TYPE_WITH_CODE(DhGeneralIsoc, dh_general_isoc, DH_TYPE_GENERAL_ISOC, G_IMPLEMENT_INTERFACE(DH_TYPE_GENERAL, dh_general_isoc_interface_init))

static void dh_general_isoc_init(DhGeneralIsoc* self)
{
}

static void dh_general_isoc_class_init(DhGeneralIsocClass* klass)
{
}

DhGeneralIsoc* dh_general_isoc_new()
{
    return g_object_new(DH_TYPE_GENERAL_ISOC, NULL);
}
