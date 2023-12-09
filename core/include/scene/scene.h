#ifndef SCENE_H
#define SCENE_H

#include "models/sphere.h"
#include "models/ray.h"

enum ModelType {
    NONE,
    SPHERE
};

typedef struct
{
    void* tab;
    unsigned int nb;
    unsigned int max;
} ModelsArray;


typedef struct {
    ModelsArray spheres;
} Scene;

void initScene(Scene *scene);
void destroyScene(Scene *scene);

/**
 * Renvoie la couleur résultant après le lancement du rayon dans la scène
*/
Color launchRay(Scene *scene, Ray *ray);

void addModel(Scene *scene, Sphere *sphere);

#endif