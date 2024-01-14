#ifndef SCENE_H
#define SCENE_H

#include "models/ray.h"
#include "models/material.h"

#include "scene/light.h"

typedef enum {
    NONE,
    CAMERA,
    PLANE,
    RECTANGLE,
    SPHERE,
    CUBE,
    TORE
} ModelType;

typedef struct DynamicArray {
    void *tab;
    unsigned int nb;
    unsigned int max;
} DynamicArray;


typedef struct Scene {
    DynamicArray spheres;
    DynamicArray cubes;
    DynamicArray tores;
    DynamicArray planes;
    DynamicArray rectangles;
    DynamicArray materials;
    Color sky;
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

unsigned int addModel(Scene *scene, void *element, ModelType type);

void* pointerFrom(Scene *scene, ModelType type, unsigned int ind);

/**
 * Ajoute dans la tableau des matériaux de la scène, le matériaux en argument, et renvoie son indice
*/
unsigned int addMaterial(Scene *scene, Material *mat);

/**
 * Renvoie une structure contenant les infos d'un hit de la scène
*/
HitInfo launchRay(Scene *scene, Ray *ray);
/**
 * Lance un rayon dans la scène, et décris tous ce qu'il touche
*/
HitInfo debugRay(Scene *scene, Ray *ray);

#endif