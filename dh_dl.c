#include "dh_dl.h"
#include "dh_file_util.h"

typedef struct DhDownloadSt
{
    const char *uri;
    const char *dest;
    DhProgressCallback *progress_callback;
    gpointer data;
    gboolean rewrite_data;
} DhDownloadSt;

int
dh_file_download_file (const char *uri, const char *dir, gboolean rewrite_file)
{
    return dh_file_download_full_arg (uri, dir, NULL, NULL, rewrite_file);
}

static int
progress_callback (void *clientp, curl_off_t dltotal, curl_off_t dlnow,
                   curl_off_t ultotal, curl_off_t ulnow)
{
    printf ("%ld/%ld\n", dlnow, dltotal);
    return 0; /* all is good */
}

int
dh_file_download_full_arg (const char *uri, const char *dest,
                           DhProgressCallback callback, gpointer data,
                           gboolean rewrite_file)
{
    CURL *curl = curl_easy_init ();
    if (curl)
        {
            char *uri_file_name = strrchr (uri, '/') + 1;

            gchar *dir_path
                = g_build_path (G_DIR_SEPARATOR_S, dest, uri_file_name, NULL);

            if (!rewrite_file)
                {
                    /* Check whether the file exists */
                    if (dh_file_exist (dir_path))
                        {
                            g_free (dir_path);
                            curl_easy_cleanup (curl);
                            return -1;
                        }
                }

            gchar *tmp_file = g_strconcat (uri_file_name, ".dhtmpf", NULL);

            gchar *tmp_dir_path = g_build_path (
                G_DIR_SEPARATOR_S, g_get_tmp_dir (), tmp_file, NULL);
            g_free (tmp_file);

            if (!dh_file_exist (tmp_dir_path))
                dh_file_create (tmp_dir_path, TRUE);
            FILE *f = fopen (tmp_dir_path, "r+b");
            fseek (f, 0, SEEK_END);
            long pos = ftell (f);
            // fseek(f, 0, SEEK_SET);
            CURLcode res;
            curl_easy_setopt (curl, CURLOPT_URL, uri);
            curl_easy_setopt (curl, CURLOPT_WRITEDATA, f);
            if (data)
                curl_easy_setopt (curl, CURLOPT_XFERINFODATA, data);
            curl_easy_setopt (curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt (curl, CURLOPT_RESUME_FROM_LARGE, pos);

            if (!callback)
                callback = progress_callback;
            curl_easy_setopt (curl, CURLOPT_XFERINFOFUNCTION, callback);
            res = curl_easy_perform (curl);
            curl_easy_cleanup (curl);
            fclose (f);

            if (res == 0)
                {
                    dh_file_copy (tmp_dir_path, dir_path,
                                  G_FILE_COPY_OVERWRITE);
                    GFile *file = g_file_new_for_path (tmp_dir_path);
                    g_file_delete (file, NULL, NULL);
                }
            g_free (dir_path);
            g_free (tmp_dir_path);

            return res;
        }
    else
        return -1;
}

static void
download_func (GTask *task, gpointer source_object, gpointer task_data,
               GCancellable *cancellable)
{
    DhDownloadSt *data = task_data;
    int ret = dh_file_download_full_arg (data->uri, data->dest,
                                         data->progress_callback, data->data,
                                         data->rewrite_data);
    g_task_return_int (task, ret);
}

void
dh_file_download_async (const char *uri, const char *dest,
                        DhProgressCallback progress_callback, gpointer data,
                        gboolean rewrite_file,
                        GAsyncReadyCallback finish_callback,
                        gpointer callback_data)
{
    DhDownloadSt *full_data = g_new0 (DhDownloadSt, 1);
    full_data->uri = uri;
    full_data->dest = dest;
    full_data->progress_callback = progress_callback;
    full_data->data = data;
    full_data->rewrite_data = rewrite_file;
    GTask *task = g_task_new (NULL, NULL, finish_callback, callback_data);
    g_task_set_task_data (task, full_data, g_free);
    g_task_run_in_thread (task, download_func);
    g_object_unref (task);
}
