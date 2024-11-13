/*  dh_nc_rl - Ncurses Readline replacement by Dream Helium
    Copyright (C) 2024 Dream Helium
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

#ifndef DH_NC_RL_H
#define DH_NC_RL_H

#include "dh_string_util.h"
#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* GList<DhArg> */
typedef GList DhArgs;

void dh_nc_rl_set_default_arg_tip(const char* tip);
char dh_nc_rl_get_arg(DhArgs* args);
DhArgs* dh_args_add_arg(DhArgs* o_args, char arg, DhStrArray* full_arg, char* description, int arg_num);
void dh_args_free(DhArgs* arg);

#ifdef __cplusplus
}
#endif

#endif /* DH_NC_RL_H */