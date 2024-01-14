#ifndef CUBE_H
#define CUBE_H

#include "utils/vector.h"
#include "utils/color.h"

typedef struct Cube {
    Vector min;
    Vector max;
    unsigned int materialIndice;
} Cube;

Vector computeCubeNormal(Cube *cube,Vector *point);

#endif