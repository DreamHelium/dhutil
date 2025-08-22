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

#include <glib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct DhStrArray DhStrArray;
    typedef struct DhString DhString;

    char *dh_strdup (const char *o_str);
    char *dh_str_array_cat (DhStrArray *arr); /* malloc'd */
    char *dh_str_array_cat_with_split (DhStrArray *arr, const char *split);
    DhStrArray *dh_str_array_init (const char *str);
    int dh_str_array_add_str (DhStrArray **arr, const char *str);
    DhStrArray *dh_str_array_dup (DhStrArray *arr);
    gboolean dh_str_array_find_repeated (DhStrArray *arr, const char *str);
    gboolean dh_str_array_equal (const DhStrArray *arr1,
                                 const DhStrArray *arr2,
                                 gboolean ignore_order);
    int dh_str_array_find (DhStrArray *arr, const char *str);
    void dh_str_array_remove (DhStrArray **arr, const char *str);
    guint dh_str_array_find_char (DhStrArray *arr, char key);
    int *dh_str_array_find_include_chars (DhStrArray *arr, char *key,
                                          int *len);
    void dh_str_array_free (DhStrArray *arr);
    char **dh_str_array_dup_to_plain (DhStrArray *arr); /* malloc'd */
    void dh_str_array_free_plain (char **strv);
    DhStrArray *dh_str_array_cat_str_array (DhStrArray *arr1, DhStrArray *arr2,
                                            const char *split);
    gboolean dh_str_find_char (const char *str, char find_char);

    DhString *dh_string_new_with_string (const char *str);
    void dh_string_free (DhString *str);
    void dh_string_replace_internal_string (DhString *string, const char *str);
    void dh_string_add_arg (DhString *string, const char *arg);
    void dh_string_replace_with_args (DhString *string);
    /* Temp string until you free `DhString` */
    char *dh_string_get_string (DhString *string);

    /** Use dh_getdelim */
    int dh_getline (char **input, size_t *n, FILE *stream);
#define dh_string_getline(input, n, stream) dh_getline (input, n, stream)
    int dh_getdelim (char **input, size_t *n, int delim, FILE *stream);

    /* Might not accurate, and if not UTF-8, return -1 */
    int dh_getprintlen (const char *str);
    int *dh_getprint_singlelen (const char *str, int *size);
    char *dh_getprint_str (const char *str, int printlen);

    typedef struct DhStrArray
    {
        char **val;
        int num;
#ifdef __cplusplus
        const char *
        operator[] (int i) const
        {
            if (i < num)
                return val[i];
            return nullptr;
        }
        int
        operator* () const
        {
            return num;
        }
#endif
    } DhStrArray;

#ifdef __cplusplus
}
#endif

#endif // DH_STRING_UTIL_H
