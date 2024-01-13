#include "models/tore.h"

Vector calculateToreNormal(Vector *point, Tore *tore)
{
    double r2 = sqrt(tore->r);
    double R2 = sqrt(tore->R);

    Vector normal = {
        4.0 * point->x * (sqrt(point->x) + sqrt(point->y) + sqrt(point->z) - r2 - R2),
        4.0 * point->y * (sqrt(point->x) + sqrt(point->y) + sqrt(point->z) - r2 - R2),
        4.0 * point->z * (sqrt(point->x) + sqrt(point->y) + sqrt(point->z) - r2 - R2) + 8 * R2 * point->z,
        };

    normalize(&normal);

    return normal;
}