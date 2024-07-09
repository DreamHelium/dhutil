#include "dh_validator.h"
#include <locale.h>
#include <stdio.h>

int main()
{
    // DhDoubleValidator* validator = dh_double_validator_new();
    // double min = -1;
    // double max = 100;
    // DH_VALIDATOR_GET_INTERFACE(validator)->set_range(DH_VALIDATOR(validator), &min, &max);

    DhIntArrayValidator* validator = dh_int_array_validator_new();
    gint64 min = 0;
    gint64 max = 100;
    dh_int_array_validator_set_allow_repeated(validator, FALSE);
    dh_int_array_validator_set_split_str(validator, ",");
    dh_int_array_validator_add_range(validator, min, max);
    DhOut* out = dh_out_new();
    DhArgInfo* args = dh_arg_info_new();
    dh_arg_info_add_arg(args, 't', "test", "test for full description");
    GValue ret;
    dh_out_read_and_output(out, "test:", "dhutil", args, DH_VALIDATOR(validator), TRUE, &ret);
    if(G_VALUE_HOLDS_CHAR(&ret))
        g_print("%c\n", g_value_get_schar(&ret));
    else if(G_VALUE_HOLDS_POINTER(&ret))
    {
        GList* list = g_value_get_pointer(&ret);
        for(int i = 0 ; i < g_list_length(list) ; i++)
        {
            g_print("%ld\n", *(gint64*)g_list_nth_data(list, i));
        }
        g_list_free(list);
    }
    g_object_unref(validator);
    g_object_unref(out);
    g_object_unref(args);
    return 0;
}
