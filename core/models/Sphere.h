#ifndef SPHERE_H
#define SPHERE_H

#include "vector.h"
#include "color.h"

typedef struct {
    Color color;
    float r;
    Vector center;
} Sphere;

#endif