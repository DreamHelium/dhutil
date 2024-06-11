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


#define DH_USE_DHLRC_FILE_UTIL
#include "dh_string_util.h"
#include "dh_file_util.h"
#include "dh_list_util.h"
#include <stdlib.h>
#include <string.h>
#include <gio/gio.h>

static int internal_strcmp(gconstpointer a, gconstpointer b)
{
    return strcmp(a, b);
}

static int strstr_to_int(gconstpointer element, gconstpointer user_data)
{
    char* ret = strstr(element, user_data);
    if(ret) return 0;
    else return -1;
}

GList* dh_file_list_create(const char* pos)
{
    GFile* dir = g_file_new_for_path(pos);
    GError* err = NULL;

    /* To use in Windows, we need attribute
     * But why don't we need in Linux? */
    GFileEnumerator* gfe = g_file_enumerate_children(dir, G_FILE_ATTRIBUTE_STANDARD_NAME, G_FILE_QUERY_INFO_NONE, NULL, &err);
    if(gfe == NULL)
    {
        g_object_unref(dir);
        return NULL;
    }
    GList* list = NULL;
    GFileInfo* info = NULL;
    while((info = g_file_enumerator_next_file(gfe, NULL, &err)) != NULL)
    {
        list = g_list_prepend(list, dh_strdup(g_file_info_get_name(info)));
        g_object_unref(info);
    }
    g_file_enumerator_close(gfe, NULL, &err);
    g_object_unref(dir);
    g_object_unref(gfe);
    list = g_list_sort(list, internal_strcmp);
    return list;
}

GList *dh_file_list_search_in_dir(const char *pos, const char *name)
{
    GList* filelist = dh_file_list_create(pos);
    if(filelist == NULL)
        return NULL;
    GList* result = dh_search_in_list(filelist, name);
    g_list_free_full(filelist, free);
    return result;
}

gboolean dh_file_create(const char* filepos, gboolean is_file)
{
    GFile* file = g_file_new_for_path(filepos);
    gboolean ret = dh_file_create_gfile(file, is_file);
    g_object_unref(file);
    return ret;
}

gboolean dh_file_create_gfile(GFile* file, gboolean is_file)
{
    if(!file)
        return FALSE;
    GError* error = NULL;
    if(file)
    {
        if(g_file_query_exists(file, NULL)) /* If file exists, delete file */
        {
            if(!g_file_delete(file, NULL, &error)) /* Couldn't delete file */
                goto error_handle;
        }
        /* I'll use a dummy way to create file
         * First, I'll mkdir */
        if(!g_file_make_directory_with_parents(file, NULL, &error))
            goto error_handle;
        if(is_file)
        {
            if(!g_file_delete(file, NULL, &error))
                goto error_handle;
            GFileOutputStream* gfos = g_file_create(file, G_FILE_CREATE_NONE, NULL, &error);
            if(error)
                goto error_handle;
            g_object_unref(gfos);
            return TRUE;
        }
        else return TRUE; /* It's directory */
    }
    else return FALSE;

error_handle:
    g_free(error);
    return FALSE;
}

gboolean dh_file_exist(const char* filepos)
{
    GFile* file = g_file_new_for_path(filepos);
    if(file)
    {
        gboolean ret = g_file_query_exists(file, NULL);
        g_object_unref(file);
        return ret;
    }
    else return FALSE;
}


char* dh_read_file(const char* filepos, gsize* size)
{
    GFile* file = g_file_new_for_path(filepos);
    if(file)
    {
        if(g_file_query_exists(file, NULL))
        {
            char* contents = NULL;
            if(g_file_load_contents(file, NULL, &contents, size, NULL, NULL))
            {
                g_object_unref(file);
                return contents;
            }
            else
            {
                g_free(contents);
                g_object_unref(file);
                return NULL;
            }
        }
        else
        {
            g_object_unref(file);
            return NULL;
        }
    }
    else return NULL;
}


gboolean dh_write_file(const char* filepos, char* content, gsize count)
{
    GFile* file = g_file_new_for_path(filepos);
    gboolean ret = dh_write_file_gfile(file, content, count);
    g_object_unref(file);
    return ret;
}

gboolean dh_write_file_gfile(GFile* file, char* content, gsize count)
{
    if(!file)
        return FALSE;
    if(!dh_file_create_gfile(file, TRUE))
        return FALSE;
    GFileIOStream* fios = g_file_open_readwrite(file, NULL, NULL);
    if(fios)
    {
        GOutputStream* os = g_io_stream_get_output_stream(G_IO_STREAM(fios));
        int ret_d = g_output_stream_write(os, content, count, NULL, NULL);
        gboolean ret = (ret_d == -1? FALSE : TRUE);
        g_object_unref(fios);
        return ret;
    }
    else
    {
        return FALSE;
    }
}

gboolean dh_file_is_directory(const char *filepos)
{
    GFile* file = g_file_new_for_path(filepos);
    GFileType type = g_file_query_file_type(file, G_FILE_QUERY_INFO_NONE, NULL);
    gboolean ret = FALSE;
    if(type == G_FILE_TYPE_DIRECTORY)
        ret = TRUE;
    g_object_unref(file);
    return ret;
}
