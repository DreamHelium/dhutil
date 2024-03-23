/*  file_util - file utilities
    Copyright (C) 2022 Dream Helium
    This file is part of litematica_reader_c.

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

#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <cjson/cJSON.h>
#include <gmodule.h>

#ifdef DH_USE_DHLRC_FILE_UTIL
int     dhlrc_write_file(char* pos, char* content, size_t count);
char*   dhlrc_read_file(const char* filepos, int* size);
int     dhlrc_mkconfig();
int     dhlrc_confirm_config_exist();
int     dhlrc_file_exist(const char* filepos);
char*   dhlrc_config_content(const char* str);
cJSON*  dhlrc_file_to_json(const char* pos);
#endif

GList*  dh_file_list_create(const char* pos);
GList*  dh_file_list_search_in_dir(const char* pos, const char* name);


#ifdef __cplusplus
}
#endif

#endif // FILE_UTIL_H
