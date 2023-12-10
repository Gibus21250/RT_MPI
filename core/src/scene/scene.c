#include "scene/scene.h"
#include "math/vector.h"

#define M_PI 3.14159265358979323846

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#define STEP 10

void initScene(Scene *scene)
{   
    //Les initialisations se font dans l'odre de l'ENUM ModelType, sans NONE

    //Initialisation de la structure de sphere
    scene->spheres.tab = malloc(STEP * sizeof(Sphere));
    scene->spheres.nb = 0;
    scene->spheres.max = STEP;


    //On init le tableau des lumières
    scene->lights.tab = malloc(STEP * sizeof(PointLight));
    scene->lights.nb = 0;
    scene->lights.max = STEP;
}

void destroyScene(Scene *scene)
{
    free(scene->spheres.tab);
    free(scene->lights.tab);
}

Color launchRay(Scene *scene, Ray *ray)
{
    
    //printf("o: %lf %lf %lf\n", ray->o.x, ray->o.y, ray->o.z);
    //printf("dir: %lf %lf %lf\n\n", ray->v.x, ray->v.y, ray->v.z);
    //Color res = {0, 0.5, 0.8};
    Color res = {.1, .1, .1};
    //Il faut garder une trace de ce qui a été touché, en gardant le plus proche intersecté
    double t = DBL_MAX;

    Vector hitNormal = {0, 0, 0};
    Vector hitPoint = {0, 0, 0};

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
                Sphere *hit = &((Sphere*) sphereArray->tab)[i];
                //Optimiser, garder une trace de qui on a touché, au lieu de calculer pour chaque hit la normal et la pos
                res = hit->color;
                hitPoint = move(ray, t).o;
                hitNormal = sub(&hitPoint, &hit->center);
                normalize(&hitNormal);
            }
        }
    }

    //On a notre t à l'intersection, on va calculer sa luminance en fonction des lumières
    if(t > 0 && t != DBL_MAX)
    {

        Color resLight = scene->ambiant;
        ModelsArray* ligthArray = &scene->lights;

        for(unsigned int i = 0; i < ligthArray->nb; ++i)
        {

            PointLight *pLight = &((PointLight*) scene->lights.tab)[i];
            //Vecteur PL (point -> lumière)
            Vector toLight = sub(&pLight->position, &hitPoint);
            normalize(&toLight);

            double rad = acos(dot(&hitNormal, &toLight));

            if(rad >= 0 && rad <= M_PI)
            {
                //Ajouter variation en fonction de la distance
                double intensity = fmax(0.0, cos(rad)) * pLight->intensity;
                Color lightColor = multiplyColord(&pLight->color, intensity);
                //On accumule la couleur au point d'impact
                resLight = addColor(&resLight, &lightColor);
            }
        }

        //On a accumulé toutes les couleurs au point d'impacte
        clampColor(&resLight);

        res = multiplyColorc(&res, &resLight);
    }
    return res;
}

char isShadowed(Scene *scene, Ray *ray)
{
    return 0;
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

void addLight(Scene *scene, PointLight *pLight)
{
    ModelsArray* lightArray = &scene->lights;
    //Si la taille du tableau ne suffis plus, on l'agrandi de STEP
    if(lightArray->nb == lightArray->max)
    {
        lightArray->max += STEP;
        lightArray->tab = realloc(lightArray->tab, lightArray->max * sizeof(PointLight));
    }

    ((PointLight*) lightArray->tab)[lightArray->nb] = *pLight;
    lightArray->nb++;
}