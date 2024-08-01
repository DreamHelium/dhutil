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

#include <stdio.h>
#include <gmodule.h>
#include <gio/gio.h>

#ifdef __cplusplus
extern "C"{
#endif

GList*  dh_file_list_create(const char* pos);
GList*  dh_file_list_search_in_dir(const char* pos, const char* name);
gboolean dh_file_exist(const char* filepos);
/* Unlike GIO's create, it just create a file */
gboolean dh_file_create(const char* filepos, gboolean is_file);
gboolean dh_file_create_gfile(GFile* file, gboolean is_file);
/* Use GIO's load_contents method */
char* dh_read_file(const char* filepos, gsize* size);
gboolean dh_write_file(const char* filepos, char* content, gsize count);
gboolean dh_write_file_gfile(GFile* file, char* content, gsize count);
gboolean dh_file_is_directory(const char* filepos);
gboolean dh_file_copy(const char* source, const char* dest, GFileCopyFlags flags);
gboolean dh_file_copy_dir(const char* source, const char* dest, GFileCopyFlags flags);
gboolean dh_file_download_file(const char* uri, const char* dir, GFileCopyFlags flags);


#ifdef __cplusplus
}
#endif

#endif // FILE_UTIL_H
