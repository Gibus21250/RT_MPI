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
    Color specular;             //Pour phong
    double roughness;           //Spéculaire biais
    double metalness;           //Pas encore utilisé
    double shininess;           //Pour phong
    double refractionFactor;    //Pas encore utilisé
    double emissionPower;       //Puissance d'emission (0 --> 1)
    Color emissionColor;        //Couleur d'emission
} Material;

#endif