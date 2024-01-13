#ifndef RAY_H
#define RAY_H

#include "utils/vector.h"

typedef struct Ray {
    Vector o;
    Vector v;
} Ray;

/**
 * Avance de t dans la direction du rayon
*/
Ray move(Ray *r, double t);

#endif