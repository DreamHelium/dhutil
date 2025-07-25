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

#include "dh_string_util.h"
#include "glib.h"
#include <stdlib.h>
#include <string.h>

#define dh_new(len, type) malloc ((len) * sizeof (type))

char *
dh_strdup (const char *o_str)
{
#if (defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L)                  \
    || _POSIX_C_SOURCE >= 200809L
    return strdup (o_str); // use strdup if provided
#else
    char *str = malloc ((strlen (o_str) + sizeof ("")) * sizeof (char));
    if (str)
        {
            strcpy (str, o_str);
            return str;
        }
    else
        return NULL;
#endif
}

static char *
strcpy_p (char *buf, const char *str)
{
#if _POSIX_C_SOURCE >= 200809L
    return stpcpy (buf, str);
#else
    strcpy (buf, str);
    for (; *buf; buf++)
        ;
    return buf;
#endif
}

char *
dh_str_array_cat (DhStrArray *arr)
{
    int len = 0;
    if (arr)
        {
            /* get length of all strings */
            for (int i = 0; i < arr->num; i++)
                len += strlen ((arr->val)[i]);

            char *out = dh_new ((len + sizeof ("")), char);
            char *out_d = out;

            if (out)
                {
                    memset (out, 0, len + strlen (""));
                    for (int i = 0; i < arr->num; i++)
                        {
                            strcpy (out_d, (arr->val)[i]);
                            while (*out_d)
                                {
                                    out_d++;
                                }
                        }
                    return out;
                }
            else
                return NULL;
        }
    else
        return NULL;
}

char *
dh_str_array_cat_with_split (DhStrArray *arr, const char *const split)
{
    if (arr)
        {
            int len = 0;
            for (int i = 0; i < arr->num; i++)
                {
                    len += strlen (arr->val[i]);
                    if (i != (arr->num - 1))
                        len += strlen (split);
                }
            len += 1;
            char *ret = dh_new (len, char);
            char *ret_p = ret;

            for (int i = 0; i < arr->num; i++)
                {
                    ret_p = strcpy_p (ret_p, arr->val[i]);
                    if (i != (arr->num - 1))
                        ret_p = strcpy_p (ret_p, split);
                }
            return ret;
        }
    else
        return NULL;
}

gboolean
dh_str_find_char (const char *str, char find_char)
{
    while (*str)
        {
            if (*str == find_char)
                return TRUE;
            str++;
        }
    return FALSE;
}

static guint
find_char (const char *str, char key)
{
    guint i = 0;
    while (*str)
        {
            if (*str == key)
                i++;
            str++;
        }
    return i;
}

guint
dh_str_array_find_char (DhStrArray *arr, char key)
{
    guint char_num = 0;
    for (int i = 0; i < arr->num; i++)
        {
            char_num += find_char (arr->val[i], key);
        }
    return char_num;
}

int *
dh_str_array_find_include_chars (DhStrArray *arr, char *key, int *len)
{
    *len = 0;
    int *ret = NULL;
    char **arr_val = arr->val;
    for (int i = 0; *arr_val; arr_val++)
        {
            if (strstr (*arr_val, key))
                {
                    int real_len = *len + 1;
                    *len = real_len;
                    ret = g_realloc (ret, sizeof (int) * real_len);
                    ret[real_len - 1] = i;
                }
            i++;
        }
    return ret;
}

DhStrArray *
dh_str_array_init (const char *str)
{
    DhStrArray *arr = g_new (DhStrArray, 1);
    arr->num = 1;
    arr->val = g_new (char *, 2);
    arr->val[0] = g_strdup (str);
    arr->val[1] = NULL;
    return arr;
}

int
dh_str_array_add_str (DhStrArray **arr, const char *str)
{
    if (*arr == NULL)
        {
            DhStrArray *arr_init = dh_str_array_init (str);
            *arr = arr_init;
            return 0;
        }
    else
        {
            DhStrArray *o_arr = *arr;
            char **p_arr = g_renew (char *, o_arr->val, o_arr->num + 2);
            o_arr->val = p_arr;
            o_arr->val[o_arr->num] = g_strdup (str);
            o_arr->val[(o_arr->num) + 1] = NULL;
            o_arr->num = o_arr->num + 1;
            return 0;
        }
}

DhStrArray *
dh_str_array_dup (DhStrArray *arr)
{
    DhStrArray *ret = NULL;
    for (int i = 0; i < arr->num; i++)
        dh_str_array_add_str (&ret, arr->val[i]);
    return ret;
}

gboolean
dh_str_array_find_repeated (DhStrArray *arr, const char *str)
{
    if (dh_str_array_find (arr, str) == -1)
        return FALSE;
    return TRUE;
}

gboolean
dh_str_array_equal (const DhStrArray *arr1, const DhStrArray *arr2,
                    gboolean ignore_order)
{
    if (arr1 && arr2)
        {
            for (int i = 0; i < arr1->num; i++)
                {
                    gboolean ret = FALSE;
                    for (int j = 0; j < arr2->num; j++)
                        {
                            if (!g_str_equal (arr1->val[i], arr2->val[j]))
                                ret = FALSE;
                            ret = TRUE;
                            if (!ignore_order && i != j)
                                ret = FALSE;
                        }
                    if (!ret)
                        return ret;
                }
            return TRUE;
        }
    return FALSE;
}

int
dh_str_array_find (DhStrArray *arr, const char *str)
{
    if (arr)
        {
            for (int i = 0; i < arr->num; i++)
                {
                    if (g_str_equal ((arr->val)[i], str))
                        return i;
                }
        }
    return -1;
}

char **
dh_str_array_dup_to_plain (DhStrArray *arr)
{
    char **ret = dh_new (arr->num + 1, char *);
    for (int i = 0; i < arr->num && ret; i++)
        {
            ret[i] = dh_strdup (arr->val[i]);
        }
    if (ret)
        ret[arr->num] = NULL;
    return ret;
}

void
dh_str_array_free_plain (char **strv)
{
    char **strv_d = strv;
    for (; *strv_d && strv_d; strv_d++)
        {
            free (*strv_d);
        }
    free (strv);
}

void
dh_str_array_free (DhStrArray *arr)
{
    if (arr)
        {
            for (int i = 0; i < ((arr->num) + 1); i++)
                g_free (arr->val[i]);
            g_free (arr->val);
            g_free (arr);
        }
}

DhStrArray *
dh_str_array_cat_str_array (DhStrArray *arr1, DhStrArray *arr2,
                            const char *const split)
{
    if (arr1->num != arr2->num)
        return NULL;
    DhStrArray *ret = NULL;
    for (int i = 0; i < arr1->num; i++)
        {
            int len = strlen (arr1->val[i]) + strlen (arr2->val[i])
                      + strlen (split) + 1;
            char *buf = dh_new (len, char);
            char *buf_p = buf;
            buf_p = strcpy_p (buf_p, arr1->val[i]);
            buf_p = strcpy_p (buf_p, split);
            buf_p = strcpy_p (buf_p, arr2->val[i]);

            dh_str_array_add_str (&ret, buf);
            free (buf);
        }
    return ret;
}

int
dh_getline (char **input, size_t *n, FILE *stream)
{
    return dh_getdelim (input, n, '\n', stream);
}

int
dh_getdelim (char **input, size_t *n, int delim, FILE *stream)
{
#if ((defined __STDC_ALLOC_LIB__ && __STDC_WANT_LIB_EXT2__ == 1)              \
     || (_POSIX_C_SOURCE - 0) >= 200809L)
    ssize_t ret = getdelim (
        input, n, delim, stream); // If provide getdelim, recommend using this.
    if (ret != -1)
        return 1;
    else
        return -1;
#else // my own getline implement

    if (!input || !n) // determine whether input/n is valid
        {
            errno = EINVAL;
            return -1;
        }

    if (*n <= 0)
        *n = 5; // Initize a 5-length buffer
    char *new_input = (char *)realloc (*input, *n * sizeof (char));

    if (new_input)
        {
            *input = new_input;
            char read_char;
            size_t char_num = 0;
            while ((read_char = fgetc (stream)) != EOF)
                {
                    if (char_num == *n) // not enough memory for input
                        {
                            *n += 5;
                            char *re_input
                                = (char *)realloc (*input, *n * sizeof (char));
                            if (re_input)
                                *input = re_input;
                            else
                                {
                                    *n -= 5;
                                    return -1;
                                }
                        }
                    (*input)[char_num] = read_char; // write the num in
                    char_num++;
                    if (read_char == delim)
                        break; // Read finish
                }
            if (read_char != EOF)
                {
                    if (char_num == *n)
                        {
                            *n += 5;
                            char *final_input
                                = (char *)realloc (*input, *n * sizeof (char));
                            if (final_input)
                                *input = final_input;
                            else
                                {
                                    *n -= 5;
                                    return -1;
                                }
                        }
                    (*input)[char_num] = '\0';
                    return 1;
                }
            else
                return -1;
        }
    else
        return -1;

#endif
}

/* Code from libgnt's utility:
 * https://keep.imfreedom.org/libgnt/libgnt/file/tip/gntutils.c */
int
dh_getprintlen (const char *str)
{
    if (!g_utf8_validate (str, -1, NULL))
        return -1;
    int len = 0;
    for (; *str; str = g_utf8_next_char (str))
        {
            gunichar c = g_utf8_get_char (str);
            if (g_unichar_iszerowidth (c))
                len += 0;
            else if (g_unichar_iswide (c))
                len += 2;
            else
                len += 1;
        }
    return len;
}

int *
dh_getprint_singlelen (const char *str, int *size)
{
    if (!g_utf8_validate (str, -1, NULL) || !size)
        return NULL;
    int size_d = 0;
    int *ret = NULL;
    for (; *str; str = g_utf8_next_char (str))
        {
            gunichar c = g_utf8_get_char (str);
            int single_size = 0;
            if (g_unichar_iszerowidth (c))
                single_size = 0;
            else if (g_unichar_iswide (c))
                single_size = 2;
            else
                single_size = 1;
            size_d++;
            ret = g_realloc (ret, size_d * sizeof (int));
            ret[size_d - 1] = single_size;
        }
    *size = size_d;
    return ret;
}

char *
dh_getprint_str (const char *str, int printlen)
{
    if (printlen >= dh_getprintlen (str) || printlen == -1)
        return g_strdup (str);
    else
        {
            /* Replace last three character with "..." */
            int str_size = 0;
            int *str_single_size = dh_getprint_singlelen (str, &str_size);

            int shrink_str_size = 0;
            int j = 0;

            for (; shrink_str_size < (printlen - 3); j++)
                {
                    shrink_str_size += str_single_size[j];
                    g_message ("%d %d", j, str_single_size[j]);
                }
            if (shrink_str_size > (printlen - 3))
                {
                    shrink_str_size -= str_single_size[j - 1];
                    j--;
                }
            int end_size = printlen - shrink_str_size;
            char *ret = g_strdup (str);
            char *ret_head = ret;
            for (int i = 0; i < j; i++, ret = g_utf8_next_char (ret))
                ;
            for (int i = 0; i < end_size; i++)
                ret[i] = '.';
            ret[end_size] = 0;
            g_free (str_single_size);
            return ret_head;
        }
}
