#ifndef SPHERE_H
#define SPHERE_H

#include "math/vector.h"
#include "exporter/color.h"

typedef struct {
    Color color;
    float r;
    Vector center;
} Sphere;

#endif