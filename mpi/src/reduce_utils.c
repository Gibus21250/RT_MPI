#include "reduce_utils.h"
#include "utils/color.h"

MPI_Op ACCUMULATOR_REDUCE_OP;

void accumulator_reduce(void *in, void *inout, unsigned *len, MPI_Datatype *dtype)
{
    Color *in_array = (Color*) in;
    Color *out_array = (Color*) inout;

    for (unsigned i = 0; i < *len; i++)
    {
        out_array[i] = addColor(&in_array[i], &out_array[i]);
    }
}

void initReduceOp()
{
    // -> commutatif (1 = oui car c'est une simple addition)
    MPI_Op_create(accumulator_reduce, 1, &ACCUMULATOR_REDUCE_OP);
}

void deleteReduceOp()
{
    MPI_Op_free(&ACCUMULATOR_REDUCE_OP);
}