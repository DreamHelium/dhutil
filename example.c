#include "src/dh_general_isoc.h"
#include "src/dh_generaliface.h"
#include "src/dh_validator.c"


// int main()
// {
//     DhIsoc* isoc = dh_general_isoc_new();
//     DhGeneral* general = DH_GENERAL(isoc);
//
//     dh_new_win(general, TRUE);
//     dh_printf(general, "This is a simple example for DhGeneral interface, please select an option:\n");
//     for(int i = 0 ; i < 3 ; i ++)
//     {
//         dh_option_printer(general, i, "%d", i);
//     }
//     int option = dh_selector(general, "Please select an option, or enter 'q' to exit: ", 3, "q");
//     if(option == -1 || option == -100)
//     /* q/empty/Terminated */
//         return 0;
//     else g_message("%d", option);
//     return 0;
// }

int main()
{
    DhDoubleValidator* validator = g_object_new(TYPE_DH_DOUBLE_VALIDATOR, NULL);
    double min = -1;
    double max = 100;
    DH_VALIDATOR_GET_INTERFACE(validator)->set_range(DH_VALIDATOR(validator), &min, &max);
    DhOut* out = g_object_new(TYPE_DH_OUT, NULL);
    DhArgInfo* args = g_object_new(TYPE_DH_ARG_INFO, NULL);
    dh_arg_info_add_arg(args, 't', "test", "test for full description");
    GValue ret;
    dh_out_read_and_output(out, "test:", "dhutil", args, DH_VALIDATOR(validator), &ret);
    if(G_VALUE_HOLDS_DOUBLE(&ret))
        g_print("%f\n", g_value_get_double(&ret));
    else g_print("%c\n", g_value_get_schar(&ret));
    g_object_unref(validator);
    g_object_unref(out);
    g_object_unref(args);
    return 0;
}
