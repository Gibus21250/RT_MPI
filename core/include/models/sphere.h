#ifndef SPHERE_H
#define SPHERE_H

#include "utils/vector.h"

typedef struct Sphere {
    Vector center;
    float r;
    unsigned int materialIndice;
} Sphere;

#endif