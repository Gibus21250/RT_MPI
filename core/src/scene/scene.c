#include "scene/scene.h"

#include "utils/vector.h"
#include "utils/math.h"



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

Color drawPixel(Scene *scene, Ray *ray)
{   
    //printf("o: %lf %lf %lf\n", ray->o.x, ray->o.y, ray->o.z);
    //printf("dir: %lf %lf %lf\n\n", ray->v.x, ray->v.y, ray->v.z);
    //Color res = {0, 0.5, 0.8};

    HitInfo firstHit = launchRay(scene, ray);
    Color res = scene->sky;
    //On a notre t à l'intersection, on va calculer sa luminance en fonction des lumières
    if(firstHit.type != NONE)
    {
        switch (firstHit.type)
        {
        case SPHERE:
            res = ((Sphere*) scene->spheres.tab)[firstHit.indice].color;
            break;
        default:
            break;
        }
        
        //Gerer les lumière
        Color resLight = scene->ambiant; //== ombre
        ModelsArray* ligthArray = &scene->lights;

        for(unsigned int i = 0; i < ligthArray->nb; ++i)
        {

            PointLight *pLight = &((PointLight*) scene->lights.tab)[i];
            //Vecteur PL (point -> lumière)
            Vector toLight = sub(&pLight->position, &firstHit.hitPoint);
            normalize(&toLight);

            Ray toLightRay = {firstHit.hitPoint, toLight};
            toLightRay = move(&toLightRay, 0.00000001);

            /*HitInfo penombra = launchRay(scene, &toLightRay);

            if(penombra.type == NONE)
            {
            }
            */
            double cosTheta = dot(&firstHit.hitNormal, &toLight);

            if(cosTheta >= -1 && cosTheta <= M_PI_2)
            {

                //TODO Ajouter variation en fonction de la distance
                double intensity = fmax(0.0, cosTheta) * pLight->intensity;
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

HitInfo launchRay(Scene *scene, Ray *ray)
{
    
    //On s'occupe de récupérer le plus proche model hit par le rayon
    HitInfo closestHit = {NONE, -1, {0,0,0}, {0,0,0}, {0, 0, 0}};
    double t = DBL_MAX;

    //Array des Spheres
    ModelsArray* sphereArray = &scene->spheres;
    //Pour chaque sphere
    //#### comparer dist
    for(unsigned int i = 0; i < sphereArray->nb; ++i)
    {   
        Sphere *sphere = &((Sphere*) sphereArray->tab)[i];

        double tTmp = intersectSphere(ray, sphere);
        //printf("%lf\n", tTmp);
        if(tTmp != -1)
        {
            //printf("Colision!\n", tTmp);
            //Colision
            if(tTmp < t)
            {
                t = tTmp;

                closestHit.type = SPHERE;
                closestHit.indice = i;
                closestHit.hitPoint = move(ray, t).o;
                
                closestHit.hitNormal = sub(&closestHit.hitPoint, &sphere->center);
                normalize(&closestHit.hitNormal);
            }
        }
    }

    return closestHit;
   
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