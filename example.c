#include "dh_validator_cpp.hpp"
#include <stdio.h>

int main()
{
    void* validator = dh_int_validator_new(0, 100, 10);
    GValue val;
    dh_get_output(validator, NULL, "test", &val);
    if(G_VALUE_HOLDS_INT64(&val))
    {
        printf("%ld\n", g_value_get_int64(&val));
    }
    return 0;
}
