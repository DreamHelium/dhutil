/*  dh_deprecated - Deprecated function names
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

#ifndef DH_DEPRECATED_H
#define DH_DEPRECATED_H

/* <dh_file_util.h>  */

#ifdef DH_USE_DEPRECATED_FUNCNAMES
#ifdef DH_USE_DHLRC_FILE_UTIL
#define dhlrc_WriteFile         dhlrc_write_file
#define dhlrc_ReadFile          dhlrc_read_file
#define dhlrc_ConfigExist       dhlrc_confirm_config_exist
#define dhlrc_FileExist         dhlrc_file_exist
#define dhlrc_ConfigContent     dhlrc_config_content
#define dhlrc_FileToJSON        dhlrc_file_to_json
#endif

#define dh_FileList_Create      dh_file_list_create
#define dh_FileList_SearchInDir dh_file_list_search_in_dir

/* <dh_string_util.h> */

#define dh_StrArray                 DhStrArray
#define dh_StrArray_cat             dh_str_array_cat
#define dh_StrArray_Init            dh_str_array_init
#define dh_StrArray_AddStr          dh_str_array_add_str
#define dh_StrArray_FindRepeated    dh_str_array_find_repeated
#define dh_StrArray_FindChar        dh_str_array_find_char
#define dh_StrArray_Free            dh_str_array_free

#endif

#endif
