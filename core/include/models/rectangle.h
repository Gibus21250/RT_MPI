#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "utils/vector.h"

typedef struct Rectangle {
    Vector center;
    Vector normal;
    double l, L;
    unsigned int materialIndice;
} Rectangle;

#endif