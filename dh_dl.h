#ifndef DHLRC_DH_DL_H
#define DHLRC_DH_DL_H

#include <gio/gio.h>
#include <curl/curl.h>

G_BEGIN_DECLS

typedef int DhProgressCallback (void *, curl_off_t, curl_off_t, curl_off_t,
                                curl_off_t);

int dh_file_download_file (const char *uri, const char *dir,
                           gboolean rewrite_file);
int dh_file_download_full_arg (const char *uri, const char *dest,
                               DhProgressCallback callback, gpointer data,
                               gboolean rewrite_file);
void dh_file_download_async (const char *uri, const char *dest,
                             DhProgressCallback progress_callback,
                             gpointer data, gboolean rewrite_file,
                             GAsyncReadyCallback finish_callback,
                             gpointer callback_data);

G_END_DECLS

#endif // DHLRC_DH_DL_H
