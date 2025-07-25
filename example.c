#include "dh_file_util.h"
#include "dh_validator_cpp.hpp"
#include <stdio.h>

#include "dh_string_util.h"
#include <math.h>

int
main ()
{
    // void* validator = dh_int_validator_new(0, 100, 10);
    // GValue val;
    // dh_get_output(validator, NULL, "test", &val);
    // if(G_VALUE_HOLDS_INT64(&val))
    // {
    //     printf("%ld\n", g_value_get_int64(&val));
    // }
    // void* v = dh_vector_bool_new();
    // dh_vector_bool_append_bit(v, 7, 0b1111111);
    // dh_vector_bool_append_bit(v, 7, 0);
    // int64_t val = dh_vector_bool_get_part_by_bit(v, 0, 14);
    // printf("%ld\n", val);

    // GList* filelist = dh_file_list_create_recursive("/home/dream_he/下载");
    // GList* f_d = filelist;
    // for(; f_d ; f_d = f_d->next)
    //     printf("%s\n", (char*)f_d->data);
    // g_list_free_full(filelist, free);

    DhStrArray *arr = NULL;
    dh_str_array_add_str (&arr, "hello orld");
    dh_str_array_add_str (&arr, "test world");
    int len = 0;
    int *get_val = dh_str_array_find_include_chars (arr, "world", &len);
    for (int i = 0; i < len; i++)
        {
            g_message ("%d", get_val[i]);
        }
    g_free (get_val);
    dh_str_array_free (arr);
    return 0;
}
