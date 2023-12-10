#ifndef RAY_H
#define RAY_H

#include "utils/vector.h"
#include "sphere.h"

typedef struct Ray {
    Vector o;
    Vector v;
} Ray;

double intersectSphere(Ray *r, Sphere *sphere);

/**
 * Avance de t dans la direction du rayon
*/
Ray move(Ray *r, double t);

#endif