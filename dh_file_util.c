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

#include "dh_string_util.h"
#include "dh_file_util.h"
#include "dh_list_util.h"
#include "glib.h"
#include "glibconfig.h"
#include <stdlib.h>
#include <string.h>
#include <gio/gio.h>

typedef struct DhDownloadSt
{
    const char* uri;
    const char* dest;
    DhProgressCallback* progress_callback;
    gpointer data;
    gboolean rewrite_data;
} DhDownloadSt;

static int internal_strcmp(gconstpointer a, gconstpointer b)
{
    return strcmp(a, b);
}

static int minus_strcmp(gconstpointer a, gconstpointer b)
{
    return -strcmp(a, b);
}

static int strstr_to_int(gconstpointer element, gconstpointer user_data)
{
    char* ret = strstr(element, user_data);
    if(ret) return 0;
    else return -1;
}

GList*  dh_file_list_create_full(const char* pos, gboolean with_dir)
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
        const char* filename = g_file_info_get_name(info);
        if(!with_dir)
            list = g_list_prepend(list, dh_strdup(g_file_info_get_name(info)));
        else
        {
            char* new_filename = g_build_path(G_DIR_SEPARATOR_S, pos, filename, NULL);
            list = g_list_prepend(list, dh_strdup(new_filename));
            g_free(new_filename);
        }
        g_object_unref(info);
    }
    g_file_enumerator_close(gfe, NULL, &err);
    g_object_unref(dir);
    g_object_unref(gfe);
    list = g_list_sort(list, internal_strcmp);
    return list;
}

GList* dh_file_list_create(const char* pos)
{
    return dh_file_list_create_full(pos, FALSE);
}

GList* dh_file_list_create_recursive(const char* pos)
{
    GList* ret = dh_file_list_create_full(pos, TRUE);
    GList* ret_d = ret;
    for(; ret_d ; ret_d = ret_d->next)
    {
        char* filename = ret_d->data;
        if(g_file_test(filename, G_FILE_TEST_IS_DIR))
        {
            GList* add_to_list = dh_file_list_create_recursive(filename);
            GList* tmp_list = add_to_list;
            for(; tmp_list ; tmp_list = tmp_list->next)
                ret = g_list_prepend(ret, dh_strdup(tmp_list->data));
            g_list_free_full(add_to_list, free);
        }
    }
    ret = g_list_sort(ret, minus_strcmp);
    return ret;
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
    g_error_free(error);
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

gboolean dh_file_copy(const char *source, const char *dest, GFileCopyFlags flags)
{
    return dh_file_copy_full_arg(source, dest, flags, NULL, NULL, NULL, NULL);
}

gboolean dh_file_copy_full_arg(const char* source, const char* dest, GFileCopyFlags flags,
                               GCancellable* cancellable, GFileProgressCallback callback, 
                               gpointer data, GError** error)
{
    GFile* file_source = g_file_new_for_path(source);
    GFile* file_dest = g_file_new_for_path(dest);
    gboolean ret = g_file_copy(file_source, file_dest, flags, cancellable, callback, data, error);
    g_object_unref(file_source);
    g_object_unref(file_dest);
    return ret;
}

gboolean dh_file_copy_dir(const char* source, const char* dest, GFileCopyFlags flags)
{
    return dh_file_copy_dir_full_arg(source, dest, flags, NULL, NULL, NULL, NULL);
}

gboolean dh_file_copy_dir_full_arg(const char* source, const char* dest, GFileCopyFlags flags,
                               GCancellable* cancellable, GFileProgressCallback callback, 
                               gpointer data, GError** error)
{
    if(!dh_file_exist(dest))
        if(!dh_file_create(dest, FALSE))
            return FALSE;
    if(!dh_file_is_directory(source)) return FALSE;
    GList* dir_files = dh_file_list_create(source);
    gboolean ret = TRUE;
    for(int i = 0 ; i < g_list_length(dir_files) ; i++)
    {
        if(!ret) break;
        gchar* filename = g_list_nth_data(dir_files, i);
        gchar* dir = g_build_path(G_DIR_SEPARATOR_S, source, filename, NULL);
        gchar* dest_dir = g_build_path(G_DIR_SEPARATOR_S, dest, filename, NULL);
        if(dh_file_is_directory(dir))
        {
            ret = dh_file_copy_dir_full_arg(dir, dest, flags, cancellable, callback, data, error);
            g_free(dir);
            g_free(dest_dir);
            continue;
        }
        else ret = dh_file_copy_full_arg(dir, dest_dir, flags, cancellable, callback, data, error);
        g_free(dir);
        g_free(dest_dir);
    }

    g_list_free_full(dir_files, free);
    return ret;
}

char* dh_file_get_current_program_dir(const char* arg_zero)
{
    char* full_dir = g_find_program_in_path(arg_zero);
    char* dir = g_path_get_dirname(full_dir);
    g_free(full_dir);
    return dir;
}

int dh_file_download_file(const char *uri, const char *dir, gboolean rewrite_file)
{
    return dh_file_download_full_arg(uri, dir, NULL, NULL, rewrite_file);
}

static int progress_callback(void *clientp,
                                curl_off_t dltotal,
                                curl_off_t dlnow,
                                curl_off_t ultotal,
                                curl_off_t ulnow)
{
    printf("%ld/%ld\n", dlnow, dltotal);
    return 0; /* all is good */
}


int dh_file_download_full_arg(const char* uri, const char* dest, DhProgressCallback callback, gpointer data, gboolean rewrite_file)
{
    CURL* curl = curl_easy_init();
    if(curl)
    {
        char* uri_file_name = strrchr(uri, '/') + 1;

        gchar* dir_path = g_build_path(G_DIR_SEPARATOR_S, dest, uri_file_name, NULL);

        if(!rewrite_file)
        {
            /* Check whether the file exists */
            if(dh_file_exist(dir_path))
            {
                g_free(dir_path);
                return -1;
            }
        }

        gchar* tmp_file = g_strconcat(uri_file_name, ".dhtmpf", NULL);

        gchar* tmp_dir_path = g_build_path(G_DIR_SEPARATOR_S, g_get_tmp_dir(), tmp_file, NULL);
        g_free(tmp_file);

        if(!dh_file_exist(tmp_dir_path))
            dh_file_create(tmp_dir_path, TRUE);
        FILE* f = fopen(tmp_dir_path, "r+b");
        fseek(f, 0, SEEK_END);
        long pos = ftell(f);
        // fseek(f, 0, SEEK_SET);
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, uri);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
        if(data)
            curl_easy_setopt(curl, CURLOPT_XFERINFODATA, data);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, pos);

        if(!callback)
            callback = progress_callback;
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, callback);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(f);

        if(res == 0)
        {
            dh_file_copy(tmp_dir_path, dir_path, G_FILE_COPY_OVERWRITE);
            GFile* file = g_file_new_for_path(tmp_dir_path);
            g_file_delete(file, NULL, NULL);
        }
        g_free(dir_path);
        g_free(tmp_dir_path);

        return res;
    }
    else return -1;
}

static void download_func(GTask* task, gpointer source_object, gpointer task_data, GCancellable* cancellable)
{
    DhDownloadSt* data = task_data;
    int ret = dh_file_download_full_arg(data->uri, data->dest, data->progress_callback, data->data, data->rewrite_data);
    g_task_return_int(task, ret);
}

void dh_file_download_async(const char* uri, const char* dest, DhProgressCallback progress_callback, gpointer data, gboolean rewrite_file, GAsyncReadyCallback finish_callback, gpointer callback_data)
{
    DhDownloadSt* full_data = g_new0(DhDownloadSt, 1);
    full_data->uri = uri;
    full_data->dest = dest;
    full_data->progress_callback = progress_callback;
    full_data->data = data;
    full_data->rewrite_data = rewrite_file;
    GTask* task = g_task_new(NULL, NULL, finish_callback, callback_data);
    g_task_set_task_data(task, full_data, g_free);
    g_task_run_in_thread(task, download_func);
    g_object_unref(task);
}

void dh_file_rm_file(const char* pos)
{
    GFile* file = g_file_new_for_path(pos);
    g_file_delete(file, NULL, NULL);
    g_object_unref(file);
}