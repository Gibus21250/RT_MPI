#include <stdio.h>
#include "math/vector.h"

int main(int argc, char const **argv)
{
    Vector v1 = {1.0, 0, 0};

    printf("%f %f %f\n", v1.x, v1.y, v1.z);
    normalize(&v1);

    printf("%f %f %f\n", v1.x, v1.y, v1.z);

    Vector v2 = {0, 1, 0};

    Vector v3 = cross(&v1, &v2);

    printf("%f %f %f\n", v3.x, v3.y, v3.z);
    return 0;
}

/*
MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello from process %d\n", rank);

    MPI_Finalize();
*/
