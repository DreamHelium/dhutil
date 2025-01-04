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
    void* v = dh_vector_bool_new();
    dh_vector_bool_append_bit(v, 1, 0b1);
    int64_t val = dh_vector_bool_get_part_from_top(v, 0, 0);
    printf("%ld\n", val);
    return 0;
}
