#ifndef PLANE_H
#define PLANE_H

#include "utils/vector.h"

typedef struct Plane {
    Vector center;
    Vector normal;
    unsigned int materialIndice;
} Plane;

#endif