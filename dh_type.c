#include "dh_type.h"
#include "dh_string_util.h"

typedef struct Info
{
    void *data;
    char *description;
    GDateTime *time;
    void *other_info;
    DhFreeFunc other_info_free;
    GRWLock info_lock;
} Info;

typedef struct Infos
{
    GHashTable *infos; /* <const char*, info> */
    DhStrArray *uuid_array;
    DhStrArray *cached_uuid;
    GRWLock infos_lock;
    GPtrArray *notifiers;
} Infos;

typedef struct Notifier
{
    DhNotifier notifier;
    void *main_klass;
} Notifier;

static DhStrArray *arr = NULL;
static GPtrArray *free_arr = NULL;
static GPtrArray *infos_arr = NULL;

static void
update (Infos *infos)
{
    for (int i = 0; i < infos->notifiers->len; i++)
        {
            Notifier *notifier = infos->notifiers->pdata[i];
            notifier->notifier (notifier->main_klass);
        }
}

static Info *
find_info (int type, const char *uuid)
{
    Infos *infos = infos_arr->pdata[type];
    return g_hash_table_lookup (infos->infos, uuid);
}

static Infos *
infos_new ()
{
    Infos *infos = g_new0 (Infos, 1);
    infos->infos
        = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
    infos->uuid_array = NULL;
    infos->cached_uuid = NULL;
    infos->notifiers = g_ptr_array_new_with_free_func (g_free);
    g_rw_lock_init (&infos->infos_lock);
    return infos;
}

static void
info_free (gpointer data, int type)
{
    Info *info = data;
    DhFreeFunc free_func = free_arr->pdata[type];
    if (free_func)
        free_func (info->data);
    g_free (info->description);
    g_date_time_unref (info->time);
    if (info->other_info_free)
        info->other_info_free (info->other_info);
    g_rw_lock_clear (&info->info_lock);
    g_free (info);
}

static void
infos_free (int type)
{
    Infos *infos = infos_arr->pdata[type];
    if (infos)
        {
            GList *values = g_hash_table_get_values (infos->infos);
            GList *values_i = values;
            while (values_i)
                {
                    /* It's info inside */
                    info_free (values_i->data, type);
                    values_i = values_i->next;
                }
            g_list_free (values);
            g_hash_table_destroy (infos->infos);
            dh_str_array_free (infos->uuid_array);
            dh_str_array_free (infos->cached_uuid);
            g_rw_lock_clear (&infos->infos_lock);
            g_ptr_array_free (infos->notifiers, TRUE);
            g_free (infos);
        }
}

int
dh_type_register (const char *type, DhFreeFunc func)
{
    int ret = dh_str_array_find (arr, type);
    if (ret == -1)
        {
            dh_str_array_add_str (&arr, type);
            if (!free_arr)
                free_arr = g_ptr_array_new ();
            g_ptr_array_add (free_arr, func);
            if (!infos_arr)
                infos_arr = g_ptr_array_new ();
            g_ptr_array_add (infos_arr, infos_new ());
            return arr->num - 1;
        }
    return ret;
}

int
dh_type_get_type (const char *type)
{
    return dh_type_register (type, NULL);
}

void
dh_type_free ()
{
    dh_str_array_free (arr);

    if (infos_arr)
        {
            for (int i = 0; i < infos_arr->len; i++)
                infos_free (i);
            g_ptr_array_free (infos_arr, TRUE);
        }
    if (free_arr)
        g_ptr_array_free (free_arr, TRUE);
    arr = NULL;
}

void
dh_info_new (int type, void *data, GDateTime *time, const gchar *description,
             void *other_data, DhFreeFunc func)
{
    Info *info = g_new0 (Info, 1);
    g_rw_lock_init (&info->info_lock);
    g_rw_lock_writer_lock (&info->info_lock);

    info->data = data;
    info->time = time;
    info->description = g_strdup (description);
    info->other_info_free = func;
    info->other_info = other_data;

    Infos *infos = infos_arr->pdata[type];
    GHashTable *table = infos->infos;
    gchar *uuid = g_uuid_string_random ();
    /* table of the infos should be locked */
    g_rw_lock_writer_lock (&infos->infos_lock);
    g_hash_table_insert (table, uuid, info);
    dh_str_array_add_str (&infos->uuid_array, uuid);
    g_rw_lock_writer_unlock (&infos->infos_lock);

    g_rw_lock_writer_unlock (&info->info_lock);

    update (infos);
}

void
dh_info_remove (int type, const gchar *uuid)
{
    char *uuid_d = g_strdup (uuid);
    Infos *infos = infos_arr->pdata[type];
    Info *info = g_hash_table_lookup (infos->infos, uuid_d);
    info_free (info, type);
    g_rw_lock_writer_lock(&infos->infos_lock);
    dh_str_array_remove (&infos->uuid_array, uuid_d);
    g_hash_table_remove (infos->infos, uuid_d);
    g_rw_lock_writer_unlock(&infos->infos_lock);
    g_free (uuid_d);

    update (infos);
}

void *
dh_info_get_data (int type, const gchar *uuid)
{
    return find_info (type, uuid)->data;
}

const char *
dh_info_get_description (int type, const gchar *uuid)
{
    return find_info (type, uuid)->description;
}

GDateTime *
dh_info_get_time (int type, const gchar *uuid)
{
    return find_info (type, uuid)->time;
}

void
dh_info_reset_description (int type, const gchar *uuid,
                           const gchar *description)
{
    Info *info = find_info (type, uuid);
    g_free (info->description);
    info->description = g_strdup (description);
}

void
dh_info_reset_data (int type, const gchar *uuid, void *data)
{
    Info *info = find_info (type, uuid);
    if (free_arr->pdata[type])
        {
            DhFreeFunc func = free_arr->pdata[type];
            func (info->data);
        }
    info->data = data;
}

gboolean
dh_info_reader_trylock (int type, const gchar *uuid)
{
    Info *info = find_info (type, uuid);
    return g_rw_lock_reader_trylock (&info->info_lock);
}

void
dh_info_reader_unlock (int type, const gchar *uuid)
{
    Info *info = find_info (type, uuid);
    g_rw_lock_reader_unlock (&info->info_lock);
}

gboolean
dh_info_writer_trylock (int type, const gchar *uuid)
{
    Infos *infos = infos_arr->pdata[type];
    Info *info = find_info (type, uuid);
    gboolean ret = g_rw_lock_writer_trylock (&info->info_lock);
    update (infos);
    return ret;
}

void
dh_info_writer_unlock (int type, const gchar *uuid)
{
    Infos *infos = infos_arr->pdata[type];
    Info *info = g_hash_table_lookup (infos->infos, uuid);
    g_rw_lock_writer_unlock (&info->info_lock);
    update (infos);
}

void
dh_info_set_uuid (int type, DhStrArray *uuid)
{
    Infos *infos = infos_arr->pdata[type];
    if (infos->cached_uuid)
        {
            dh_str_array_free (infos->cached_uuid);
            infos->cached_uuid = NULL;
        }
    infos->cached_uuid = dh_str_array_dup (uuid);
}

void
dh_info_set_single_uuid (int type, const char *uuid)
{
    DhStrArray *arr = dh_str_array_init (uuid);
    dh_info_set_uuid (type, arr);
    dh_str_array_free (arr);
}

const DhStrArray *
dh_info_get_uuid (int type)
{
    Infos *infos = infos_arr->pdata[type];
    return infos->cached_uuid;
}

const DhStrArray *
dh_info_get_all_uuid (int type)
{
    Infos *infos = infos_arr->pdata[type];
    return infos->uuid_array;
}

void
dh_info_add_notifier (int type, DhNotifier func, void *main_klass)
{
    Notifier *n = g_new0 (Notifier, 1);
    n->notifier = func;
    n->main_klass = main_klass;
    Infos *infos = infos_arr->pdata[type];
    g_ptr_array_add (infos->notifiers, n);
}

void
dh_info_remove_notifier (int type, void *main_klass)
{
    Infos *infos = infos_arr->pdata[type];
    for (int i = 0; i < infos->notifiers->len; i++)
        {
            Notifier *n = infos->notifiers->pdata[i];
            if (n->main_klass == main_klass)
                {
                    g_ptr_array_remove (infos->notifiers, n);
                    break;
                }
        }
}
