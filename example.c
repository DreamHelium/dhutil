#include "src/dh_general_isoc.h"
#include "src/dh_generaliface.h"

int main()
{
    DhIsoc* isoc = dh_general_isoc_new();
    DhGeneral* general = DH_GENERAL(isoc);

    dh_new_win(general, TRUE);
    dh_printf(general, "This is a simple example for DhGeneral interface, please select an option:\n");
    for(int i = 0 ; i < 3 ; i ++)
    {
        dh_option_printer(general, i, "%d", i);
    }
    int option = dh_selector(general, "Please select an option, or enter 'q' to exit: ", 3, "q");
    if(option == -1 || option == -100)
    /* q/empty/Terminated */
        return 0;
    else g_message("%d", option);
    return 0;
}
