/*  dh_string_util - String utils by Dream Helium
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

#ifndef DH_STRING_UTIL_H
#define DH_STRING_UTIL_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <glib.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct DhStrArray{
    char** val;
    int num;
} DhStrArray;

char*       dh_strdup(const char *o_str);
char*       dh_str_array_cat(DhStrArray* arr);
DhStrArray* dh_str_array_init(const char* str);
int         dh_str_array_add_str(DhStrArray** arr ,const char* str);
gboolean    dh_str_array_find_repeated(DhStrArray* arr, const char* str);
guint       dh_str_array_find_char(DhStrArray* arr, char key);
void        dh_str_array_free(DhStrArray* arr);
gboolean    dh_str_find_char(const char* str, char find_char);

/** Use dh_getdelim */
int         dh_getline(char** input, size_t* n, FILE* stream);
#define     dh_string_getline(input, n, stream) dh_getline(input, n, stream)
int         dh_getdelim(char** input, size_t* n,int delim, FILE* stream);

/* Might not accurate, and if not UTF-8, return -1 */
int         dh_getprintlen(const char* str);
int*        dh_getprint_singlelen(const char* str, int* size);
char*       dh_getprint_str(const char* str, int printlen);

#ifdef __cplusplus
}
#endif

#endif // DH_STRING_UTIL_H
