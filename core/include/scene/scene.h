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
    Color sky;
} Scene;

typedef struct HitInfo {
    ModelType type;
    unsigned int indice;
    double distance;
    Vector hitPoint;
    Vector hitNormal;
    Color hitColor;
} HitInfo;

void initScene(Scene *scene);
void destroyScene(Scene *scene);

/**
 * Renvoie la couleur résultant après le lancement du rayon dans la scène
*/
HitInfo launchRay(Scene *scene, Ray *ray);
Color drawPixel(Scene *scene, Ray *ray);

void addModel(Scene *scene, Sphere *sphere);
void addLight(Scene *scene, PointLight *pLight);

#endif