#include "scene/scene.h"
#include "math/vector.h"

#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#define STEP 10

void initScene(Scene *scene)
{   

    //Les initialisations se font dans l'odre de l'ENUM ModelType, sans NONE

    //Initialisation de la structure de sphere
    scene->spheres.tab = malloc(STEP * sizeof(Sphere));
    scene->spheres.nb = 0;
    scene->spheres.max = STEP;
}

void destroyScene(Scene *scene)
{
    //Free models Sphere
    free(scene->spheres.tab);
}

Color launchRay(Scene *scene, Ray *ray)
{
    
    //printf("o: %lf %lf %lf\n", ray->o.x, ray->o.y, ray->o.z);
    //printf("dir: %lf %lf %lf\n\n", ray->v.x, ray->v.y, ray->v.z);
    Color res = {0, 0.5, 0.8};
    //Il faut garder une trace de ce qui a été touché, en gardant le plus proche intersecté
    double t = DBL_MAX;

    //Array des Spheres
    ModelsArray* sphereArray = &scene->spheres;
    //Pour chaque sphere
    for(unsigned int i = 0; i < sphereArray->nb; ++i)
    {
        double tTmp = intersectSphere(ray, &((Sphere*) sphereArray->tab)[i]);
        //printf("%lf\n", tTmp);
        if(tTmp != -1 && tTmp >= 0)
        {
            //printf("Colision!\n", tTmp);
            //Colision
            if(tTmp < t)
            {
                t = tTmp;
                res = ((Sphere*) sphereArray->tab)[i].color;
            }
        }
    }
    return res;
}

void addModel(Scene *scene, Sphere *sphere)
{
    ModelsArray* sphereArray = &scene->spheres;
    //Si la taille du tableau ne suffis plus, on l'agrandi de STEP
    if(sphereArray->nb == sphereArray->max)
    {
        sphereArray->max += STEP;
        sphereArray->tab = realloc(sphereArray->tab, sphereArray->max * sizeof(Sphere));
    }

    ((Sphere*) sphereArray->tab)[sphereArray->nb] = *sphere;
    sphereArray->nb++;
}