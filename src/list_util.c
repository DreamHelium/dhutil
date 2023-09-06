/*  list_util - simple glist utilities
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

#include "list_util.h"
#include "dh_string_util.h"

static int strstr_to_int(gconstpointer element, gconstpointer user_data)
{
    char* ret = strstr(element, user_data);
    if(ret) return 0;
    else return -1;
}

GList *dh_search_in_list(GList *list, const char *name)
{
    GList* ret = NULL;
    if(list == NULL)
        return NULL;
    else
    {
        while(list)
        {
            /* Find the first corresponding item */
            list = g_list_find_custom(list, name, strstr_to_int);
            if(list)
            {
                ret = g_list_append(ret, dh_strdup(list->data));
                /* Force update */
                list = list->next;
            }
            /* Else there's no corresponding item, finish */
        }
        return ret;
    }
}

GList* dh_search_in_list_custom(GList* list, gconstpointer data, GCompareFunc func)
{
    GList* ret = NULL;
    if(list == NULL)
        return NULL;
    else
    {
        while(list)
        {
            /* Find the first corresponding item */
            list = g_list_find_custom(list, data, func);
            if(list)
            {
                ret = g_list_append(ret, list->data);
                /* Force update */
                list = list->next;
            }
            /* Else there's no corresponding item, finish */
        }
        return ret;
    }
}
