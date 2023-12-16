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
    //Les initialisations se font dans l'odre de l'ENUM ModelType, sans NONE

    //Initialisation de la structure de sphere
    scene->spheres.tab = malloc(STEP * sizeof(Sphere));
    scene->spheres.nb = 0;
    scene->spheres.max = STEP;


    //On init le tableau des lumières
    scene->lights.tab = malloc(STEP * sizeof(PointLight));
    scene->lights.nb = 0;
    scene->lights.max = STEP;

    //On init le tableau des materiaux
    scene->materials.tab = malloc(STEP * sizeof(Material));
    scene->materials.nb = 0;
    scene->materials.max = STEP;
}

void destroyScene(Scene *scene)
{
    free(scene->spheres.tab);
    free(scene->lights.tab);
    free(scene->materials.tab);
}

Color drawPixel(Scene *scene, Ray *ray)
{   

    Color res = {0, 0, 0};
    double energie = 1.0;

    for(unsigned int i = 0; i < scene->maxBounces; ++i)
    {
        HitInfo hit = launchRay(scene, ray);
        
        if(hit.type == NONE)
        {
            //si le rayon ne touche rien, on récupère la couleur du ciel
            Color sky =  computeSkyColor(scene, ray);
            sky = multiplyColord(&sky, energie);
            res = addColor(&res, &sky);
            break;
        }
        //On récupère le materiaux touche
        Material matHit = ((Material*) scene->materials.tab)[hit.materialIndice];

        //En fonction du type de materiaux
        Color direct = computeDirectLight(scene, &hit);
        
        direct = multiplyColord(&direct, energie);
        direct = multiplyColorc(&matHit.albedo, &direct);
        res = addColor(&res, &direct);

        energie *= 0.7;

        //On change l'origine du point et sa direction pour le prochain lancé
        ray->o = hit.hitPoint;
        Vector random = randomFrom(-0.5, 0.5);
        Vector devia = mul(&random, matHit.roughness);
        devia = add(&devia, &hit.hitNormal);
        normalize(&devia);
        //ray->v = randomRayHemisphere(&hit.hitNormal);
        ray->v = reflect(&ray->v, &devia);

    }
    clampColor(&res);
    return res;
}

Color computeDirectLight(Scene *scene, HitInfo *hit)
{
    // couleur de l'éclairage direct de base
    Material mat = ((Material *)scene->materials.tab)[hit->materialIndice];

    DynamicArray* lightArray = &scene->lights;

    DynamicArray *lightArray = &scene->lights;

    // Pour chaque lumière de la scène
    for (unsigned int i = 0; i < lightArray->nb; ++i)
    {
        PointLight *pLight = &((PointLight *)scene->lights.tab)[i];

        // Vecteur PL (hit -> lumière)
        Vector toLight = sub(&pLight->position, &hit->hitPoint);
        double distanceLight2 = dot(&toLight, &toLight);
        normalize(&toLight);

        Ray toLightRay = {hit->hitPoint, toLight};
        toLightRay = move(&toLightRay, 0.0000001);

        HitInfo shadowHit = launchRay(scene, &toLightRay);

        // Si aucune collision, on calcule la couleur diffuse en fonction de la lumière testée
        if (shadowHit.type == NONE)
        {
            double cosTheta = dot(&hit->hitNormal, &toLight);

            // si l'angle > 0 && < PI (180 deg)
            if (cosTheta >= -1 && cosTheta <= M_PI_2)
            {
                
                double atenuation = 1. / (0.1 + distanceLight2);
                // On calcule la couleur diffuse
                double intensity = fmax(0.0, cosTheta) * pLight->intensity * atenuation;
                Color lightColor = multiplyColord(&pLight->color, intensity);
                // On accumule la couleur au point d'impact pour toutes les lampe
                diffuse = addColor(&diffuse, &lightColor);

                // On calcule la couleur spéculaire
                Vector R = reflect(&hit->ray.v, &hit->hitNormal);
                double coeff = dot(&R, &toLight);
                coeff = fmax(coeff, 0);
                coeff = pow(coeff, mat.shininess);
                Color icoeff = multiplyColord(&pLight->color, coeff);
                icoeff = multiplyColord(&icoeff, atenuation);
                specular = addColor(&specular, &icoeff);
            }
        }
    }

    //On a accumulé toutes les lumières au point d'impacte
    clampColor(&directLight);

    return directLight;
}

Color computeSkyColor(Scene *scene, Ray *ray)
{
    // Direction du rayon normalisée
    Vector normalizedDirection = ray->v;

    double t = 0.5 * (normalizedDirection.y + 1.0);
    Color horizon = {1, 1, 0.8};

    Color tmpDeb = multiplyColord(&horizon, (1-t));
    Color tmpFin = multiplyColord(&scene->sky, t);

    Color skyColor = addColor(&tmpDeb, &tmpFin);

    return skyColor;
}

HitInfo launchRay(Scene *scene, Ray *ray)
{
    
    //On s'occupe de récupérer le plus proche model hit par le rayon
    HitInfo closestHit = {NONE, -1, -1, DBL_MAX, {0,0,0}, {0,0,0}};

    double closestDist = DBL_MAX;

    //Array des Spheres
    DynamicArray* sphereArray = &scene->spheres;
    
    //Pour chaque sphere
    for(unsigned int i = 0; i < sphereArray->nb; ++i)
    {   
        Sphere *sphere = &((Sphere*) sphereArray->tab)[i];

        double tTmp = intersectSphere(ray, sphere);
        
        //printf("%lf\n", tTmp);
        if(tTmp != -1)
        {
            //On récupère le point à l'intersection
            Vector hitPoint = move(ray, tTmp).o;

            //Vecteur origine -> hitPoint
            Vector cHit = sub(&hitPoint, &ray->o);
            //dist carré
            double dist = dot(&cHit, &cHit);
            //printf("Colision!\n", tTmp);
            //Colision
            if(dist < closestDist)
            {
                closestDist = dist;
                closestHit.type = SPHERE;
                closestHit.modelIndice = i;
                closestHit.materialIndice = sphere->materialIndice;
                closestHit.distance = dist;
                closestHit.hitPoint = hitPoint;
                closestHit.hitNormal = sub(&hitPoint, &sphere->center);

                normalize(&closestHit.hitNormal);
            }
        }
    }
    return closestHit;
}

void addModel(Scene *scene, Sphere *sphere)
{
    DynamicArray* sphereArray = &scene->spheres;
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
    DynamicArray* lightArray = &scene->lights;
    //Si la taille du tableau ne suffis plus, on l'agrandi de STEP
    if(lightArray->nb == lightArray->max)
    {
        lightArray->max += STEP;
        lightArray->tab = realloc(lightArray->tab, lightArray->max * sizeof(PointLight));
    }

    ((PointLight*) lightArray->tab)[lightArray->nb] = *pLight;
    lightArray->nb++;
}

unsigned int addMaterial(Scene *scene, Material *pMat)
{
    DynamicArray* materialArray = &scene->materials;
    //Si la taille du tableau ne suffis plus, on l'agrandi de STEP
    if(materialArray->nb == materialArray->max)
    {
        materialArray->max += STEP;
        materialArray->tab = realloc(materialArray->tab, materialArray->max * sizeof(Material));
    }

    //On sauvegarde le matériaux dans le tableau
    ((Material*) materialArray->tab)[materialArray->nb] = *pMat;
    materialArray->nb++;

    return (materialArray->nb-1);
}