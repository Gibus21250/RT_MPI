#ifndef SCENE_H
#define SCENE_H

#include "models/sphere.h"
#include "models/ray.h"
#include "scene/light.h"

typedef enum {
    NONE,
    SPHERE
} ModelType;

typedef struct ModelsArray {
    void* tab;
    unsigned int nb;
    unsigned int max;
} ModelsArray;


typedef struct Scene {
    ModelsArray spheres;
    ModelsArray lights;
    Color ambiant;
} Scene;

typedef struct Payload {
    ModelType type;
    unsigned int indice;
    Color color;
} Payload;

void initScene(Scene *scene);
void destroyScene(Scene *scene);

/**
 * Renvoie la couleur résultant après le lancement du rayon dans la scène
*/
Color launchRay(Scene *scene, Ray *ray);
char isShadowed(Scene *scene, Ray *ray);

void addModel(Scene *scene, Sphere *sphere);
void addLight(Scene *scene, PointLight *pLight);

#endif