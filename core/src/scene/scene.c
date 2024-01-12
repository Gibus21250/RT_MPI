#include "scene/scene.h"

#include "utils/vector.h"
#include "utils/math.h"
#include "models/material.h"

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#define STEP 10

void initScene(Scene *scene)
{
    // Les initialisations se font dans l'odre de l'ENUM ModelType, sans NONE

    // Initialisation de la structure de sphere
    scene->spheres.tab = malloc(STEP * sizeof(Sphere));
    scene->spheres.nb = 0;
    scene->spheres.max = STEP;

    // On init le tableau des materiaux
    scene->materials.tab = malloc(STEP * sizeof(Material));
    scene->materials.nb = 0;
    scene->materials.max = STEP;
}

void destroyScene(Scene *scene)
{
    free(scene->spheres.tab);
    free(scene->materials.tab);
}

HitInfo debugRay(Scene *scene, Ray *ray)
{
    // On s'occupe de récupérer le plus proche model hit par le rayon
    HitInfo closestHit = {NONE, -1, -1, DBL_MAX, {0, 0, 0}, {0, 0, 0}, *ray};

    // Array des Spheres
    DynamicArray *sphereArray = &scene->spheres;
    printf("Rayon: o:%lf %lf %lf dir: %lf %lf %lf\n",
        ray->o.x, ray->o.y, ray->o.z,
        ray->v.x, ray->v.y, ray->v.z
    );
    printf("Parcours de toutes les sphère: %d sphere(s)\n", sphereArray->nb);
    // Pour chaque sphere
    for (unsigned int i = 0; i < sphereArray->nb; ++i)
    {
        Sphere *sphere = &((Sphere *)sphereArray->tab)[i];

        printf("-------------------------\nInfo sphère:\nCentre: %lf %lf %lf\nr: %f\nmatIndice: %d\n",
        sphere->center.x, sphere->center.y, sphere->center.z, sphere->r, sphere->materialIndice
        );

        double tTmp = intersectSphere(ray, sphere);

        printf("\tClosest t: %lf\n", tTmp);

        // printf("%lf\n", tTmp);
        if (tTmp > 0)
        {
            // On récupère le point à l'intersection
            Vector hitPoint = move(ray, tTmp).o;
            printf("\t\tPosition hit: %lf %lf %lf\n", hitPoint.x, hitPoint.y, hitPoint.z);
            // Vecteur origine -> hitPoint
            Vector cHit = sub(&hitPoint, &ray->o);
            printf("\t\tVecteur cHit: %lf %lf %lf\n", cHit.x, cHit.y, cHit.z);
            // dist carré
            double dist = dot(&cHit, &cHit);

            printf("\t\tDistance2 origine -> hitPoint: %lf\n", dist);
            // Colision
            if (dist < closestHit.distance2)
            {
                printf("\t\t\tHit plus proche!\n");
                closestHit.type = SPHERE;
                closestHit.modelIndice = i;
                closestHit.materialIndice = sphere->materialIndice;
                closestHit.distance2 = dist;
                closestHit.hitPoint = hitPoint;
                closestHit.hitNormal = sub(&hitPoint, &sphere->center);
                normalize(&closestHit.hitNormal);
            }
        }
    }
    return closestHit;
}

HitInfo launchRay(Scene *scene, Ray *ray)
{

    // On s'occupe de récupérer le plus proche model hit par le rayon
    HitInfo closestHit = {NONE, -1, -1, DBL_MAX, {0, 0, 0}, {0, 0, 0}, *ray};

    // Array des Spheres
    DynamicArray *sphereArray = &scene->spheres;

    // Pour chaque sphere
    for (unsigned int i = 0; i < sphereArray->nb; ++i)
    {
        Sphere *sphere = &((Sphere *)sphereArray->tab)[i];

        double tTmp = intersectSphere(ray, sphere);

        // printf("%lf\n", tTmp);
        if (tTmp > 0)
        {   
            // On récupère le point à l'intersection
            Vector hitPoint = move(ray, tTmp).o;

            // Vecteur origine -> hitPoint
            Vector cHit = sub(&hitPoint, &ray->o);
            // dist carré
            double dist = dot(&cHit, &cHit);
            // printf("Colision!\n", tTmp);
            // Colision
            if (dist < closestHit.distance2)
            {
                closestHit.type = SPHERE;
                closestHit.modelIndice = i;
                closestHit.materialIndice = sphere->materialIndice;
                closestHit.distance2 = dist;
                closestHit.hitPoint = hitPoint;
                closestHit.hitNormal = sub(&hitPoint, &sphere->center);
                normalize(&closestHit.hitNormal);

            }
        }
    }
    return closestHit;
}

void* pointerFrom(Scene *scene, ModelType type, unsigned int ind)
{
    switch (type)
    {
    case SPHERE:
        DynamicArray *sphereArray = &scene->spheres;
        //printf("Pointer asked: %p\n", &((Sphere *)sphereArray->tab)[sphereArray->nb]);
        return &((Sphere *)sphereArray->tab)[ind];
        break;
    
    default:
        break;
    }
}

unsigned int addModel(Scene *scene, Sphere *sphere)
{
    DynamicArray *sphereArray = &scene->spheres;
    // Si la taille du tableau ne suffis plus, on l'agrandi de STEP
    if (sphereArray->nb == sphereArray->max)
    {
        sphereArray->max += STEP;
        sphereArray->tab = realloc(sphereArray->tab, sphereArray->max * sizeof(Sphere));
    }
    ((Sphere *)sphereArray->tab)[sphereArray->nb] = *sphere;
    sphereArray->nb++;

    return sphereArray->nb-1;
}

unsigned int addMaterial(Scene *scene, Material *pMat)
{
    DynamicArray *materialArray = &scene->materials;
    // Si la taille du tableau ne suffis plus, on l'agrandi de STEP
    if (materialArray->nb == materialArray->max)
    {
        materialArray->max += STEP;
        materialArray->tab = realloc(materialArray->tab, materialArray->max * sizeof(Material));
    }

    // On sauvegarde le matériaux dans le tableau
    ((Material *)materialArray->tab)[materialArray->nb] = *pMat;
    materialArray->nb++;

    return (materialArray->nb - 1);
}