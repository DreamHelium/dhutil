#include "dh_file_util.h"
#include "dh_validator_cpp.hpp"
#include <stdio.h>
#include "dh_bit.hpp"
#include <math.h>

int main()
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
    GList* filelist = dh_file_list_create_recursive("/home/dream_he/下载");
    GList* f_d = filelist;
    for(; f_d ; f_d = f_d->next)
        printf("%s\n", (char*)f_d->data);
    g_list_free_full(filelist, free);
    return 0;
}
