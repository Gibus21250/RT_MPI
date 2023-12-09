#include <stdio.h>
#include <stdlib.h>
#include "math/vector.h"
#include "exporter/color.h"
#include "exporter/saver.h"

int main(int argc, char const **argv)
{
    Vector v1 = {1.0, 0, 0};

    printf("%f %f %f\n", v1.x, v1.y, v1.z);
    normalize(&v1);

    printf("%f %f %f\n", v1.x, v1.y, v1.z);

    Vector v2 = {0, 1, 0};

    Vector v3 = cross(&v1, &v2);

    printf("%f %f %f\n", v3.x, v3.y, v3.z);

    unsigned int l = 1920, L = 1080;

    Color *image = (Color*) malloc(l * L * sizeof(Color));

    for (unsigned int i = 0; i < l; ++i)
    {
        for (unsigned int j = 0; j < L; ++j)
        {
            // Générer des valeurs aléatoires pour les composantes rouge, verte et bleue
            image[i * L + j].r = (float) rand() / RAND_MAX; // Composante rouge entre 0 et 1
            image[i * L + j].g = (float) rand() / RAND_MAX; // Composante verte entre 0 et 1
            image[i * L + j].b = (float) rand() / RAND_MAX; // Composante bleue entre 0 et 1
        }
    }

    printf("save");

    save(image, l, L, "test.ppm");

    free(image);
    return 0;
}

/*
MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello from process %d\n", rank);

    MPI_Finalize();
*/
