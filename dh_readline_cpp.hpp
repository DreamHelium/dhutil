/*  dh_readline_cpp - Readline support
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

#ifndef DH_READLINE_CPP_H
#define DH_READLINE_CPP_H

#ifdef __cplusplus
extern "C"{
#endif

void init_readline(const char* name);
void dhutil_set_completion(void* args);

#ifdef __cplusplus
}
#endif

#endif /* DH_READLINE_CPP_H */