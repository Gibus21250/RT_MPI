#ifndef CAMERA_H
#define CAMERA_H

#include "utils/vector.h"

typedef struct {

    Vector position;
    Vector lookAt;
    Vector up;
    double distance;
    double fov;

} Camera;

#endif