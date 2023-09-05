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

#include "dh_generaliface.h"
#include <stdarg.h>

G_DEFINE_INTERFACE (DhGeneral, dh_general, G_TYPE_OBJECT)

static void dh_general_default_init(DhGeneralInterface* iface)
{
    iface->v_printer = NULL;
    iface->option_printer = NULL;
    iface->selector = NULL;
}

int dh_printf(DhGeneral* self, const char* str, ...)
{
    DhGeneralInterface* iface;

    g_return_val_if_fail(DH_IS_GENERAL(self), -1);

    iface = DH_GENERAL_GET_IFACE(self);
    g_return_val_if_fail(iface->v_printer != NULL, -1);
    va_list va;
    va_start(va, str);
    int ret = iface->v_printer(self, str, va);
    va_end(va);
    return ret;
}

int dh_option_printer(DhGeneral* self, int opt, const char* str)
{
    DhGeneralInterface* iface;

    g_return_val_if_fail(DH_IS_GENERAL(self), -1);

    iface = DH_GENERAL_GET_IFACE(self);
    g_return_val_if_fail(iface->option_printer != NULL, -1);
    return iface->option_printer(self, opt, str);
}


int dh_selector(DhGeneral* self, const char* tip, int opt, const char* opt_name, ...)
{
    DhGeneralInterface* iface;

    g_return_val_if_fail(DH_IS_GENERAL(self), -100);

    iface = DH_GENERAL_GET_IFACE(self);
    g_return_val_if_fail(iface->selector != NULL, -100);
    va_list va;
    va_start(va, opt_name);
    int ret = iface->selector(self, tip, opt, opt_name, va);
    va_end(va);
    return ret;
}
