#ifndef DH_TYPE_H
#define DH_TYPE_H

#include "dh_macro.h"
#include "dh_string_util.h"
#include <glib.h>

G_BEGIN_DECLS

/* Such as DH_TYPE_REGISTER(DhList, dh, list) */

#define DH_TYPE_REGISTER_GET_FUNC(type, TYPE_PREFIX, TYPE_SUFFIX, free_func)  \
    int TYPE_PREFIX##_##TYPE_SUFFIX##_get_type ()                             \
    {                                                                         \
        return dh_type_register (#type, free_func);                           \
    }

#define DH_GET_TYPE(TYPE_PREFIX, TYPE_SUFFIX)                                 \
    int TYPE_PREFIX##_##TYPE_SUFFIX##_get_type ();

typedef void (*DhNotifier) (void *);
typedef void (*DhFreeFunc) (void *);

int dh_type_register (const char *type, DhFreeFunc func);
int dh_type_get_type (const char *type);
void dh_type_free ();

void dh_info_new (int type, void *data, GDateTime *time,
                  const gchar *description, void *other_data, DhFreeFunc func);
void dh_info_remove (int type, const gchar *uuid);

void *dh_info_get_data (int type, const gchar *uuid);
const char *dh_info_get_description (int type, const gchar *uuid);
GDateTime *dh_info_get_time (int type, const gchar *uuid);

void dh_info_reset_description (int type, const gchar *uuid,
                                const gchar *description);
void dh_info_reset_data (int type, const gchar *uuid, void *data);

gboolean dh_info_reader_trylock (int type, const gchar *uuid);
void dh_info_reader_unlock (int type, const gchar *uuid);
gboolean dh_info_writer_trylock (int type, const gchar *uuid);
void dh_info_writer_unlock (int type, const gchar *uuid);

void dh_info_set_uuid (int type, DH_DUP DhStrArray *uuid);
void dh_info_set_single_uuid (int type, const char *uuid);
const DhStrArray *dh_info_get_uuid (int type);
const DhStrArray *dh_info_get_all_uuid (int type);

void dh_info_add_notifier (int type, DhNotifier func, void *main_klass);
void dh_info_remove_notifier (int type, void *main_klass);

G_END_DECLS

#endif // DH_TYPE_H
