#ifndef RAY_H
#define RAY_H

#include "math/vector.h"
#include "sphere.h"

typedef struct {
    Vector o;
    Vector v;
} Ray;

double intersectSphere(Ray *r, Sphere *sphere);

#endif