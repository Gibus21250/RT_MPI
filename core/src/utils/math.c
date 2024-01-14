#include "utils/math.h"
#include <stdlib.h>

double random_double()
{
    return rand() / (RAND_MAX + 1.0);
}