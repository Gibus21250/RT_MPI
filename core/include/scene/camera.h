#ifndef CAMERA_H
#define CAMERA_H

#include "math/vector.h"

typedef struct
{
    Vector position;
    Vector lookAt;
    Vector up;
} Camera;

#endif