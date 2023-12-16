#ifndef SCENE_H
#define SCENE_H

#include "models/sphere.h"
#include "models/ray.h"
#include "models/material.h"

#include "scene/light.h"

typedef enum {
    NONE,
    SPHERE
} ModelType;

typedef struct DynamicArray {
    void* tab;
    unsigned int nb;
    unsigned int max;
} DynamicArray;


typedef struct Scene {
    DynamicArray spheres;
    DynamicArray lights;
    DynamicArray materials;
    Color ambiant;
    Color sky;
    unsigned int maxBounces;
} Scene;

typedef struct HitInfo {
    ModelType type;
    unsigned int modelIndice;
    unsigned int materialIndice;
    double distance2;
    Vector hitPoint;
    Vector hitNormal;
    Ray originalRay;
} HitInfo;

void initScene(Scene *scene);
void destroyScene(Scene *scene);

void addModel(Scene *scene, Sphere *sphere);
void addLight(Scene *scene, PointLight *pLight);

/**
 * Ajoute dans la tableau des matériaux de la scène, le matériaux en argument, et renvoie son indice
*/
unsigned int addMaterial(Scene *scene, Material *mat);

Color computeSkyColor(Scene *scene, Ray *ray);

/**
 * Renvoie une structure contenant les infos d'un hit
*/
HitInfo launchRay(Scene *scene, Ray *ray);
/**
 * Lance un rayon dans la scène, et décris tous ce qu'il touche
*/
HitInfo debugRay(Scene *scene, Ray *ray);
Color drawPixel(Scene *scene, Ray *ray);

/**
 * Calcule la couleur au point hit:
 * Abiant + diffuse + specular
*/
Color computeColor(Scene *scene, HitInfo *hit);

#endif