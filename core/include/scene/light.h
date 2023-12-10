#ifndef LIGHT_H
#define LIGHT_H

#include "utils/vector.h"
#include "utils/color.h"

//Point light
typedef struct PointLight
{
    Vector position;
    double intensity;
    Color color;
} PointLight;


#endif