#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils/color.h"

typedef enum MaterialType {
    VISIBLE = 1,
    DIFFUSE = 2,
    REFRACTIF = 4,
    EMISSIVE = 8
} MaterialType;

typedef struct Material
{
    MaterialType type;
    Color albedo;
    double roughness;
    double refractionFactor;
    Color emissive;
} Material;

#endif