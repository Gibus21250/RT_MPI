#ifndef SPHERE_H
#define SPHERE_H

#include "utils/vector.h"
#include "utils/color.h"

typedef struct Sphere {
    Color color;
    float r;
    Vector center;
} Sphere;

#endif