#ifndef RAY_H
#define RAY_H

#include "vector.h"

#include "Sphere.h"

typedef struct {
    Vector o;
    Vector v;
} Ray;

char intersectSphere(Ray *r, Sphere *sphere);

#endif