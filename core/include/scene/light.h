#ifndef LIGHT_H
#define LIGHT_H

#include "math/vector.h"
#include "exporter/color.h"

//Point light
typedef struct PointLight
{
    Vector position;
    double intensity;
    Color color;
} PointLight;


#endif