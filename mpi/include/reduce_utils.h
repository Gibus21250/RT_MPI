#ifndef REDUCE_UTILS_H
#define REDUCE_UTILS_H

#include <mpi.h>

extern MPI_Op ACCUMULATOR_REDUCE_OP;

void initReduceOp();
void deleteReduceOp();
void accumulator_reduce(void *in, void *out, unsigned *len, MPI_Datatype *dtype);

#endif