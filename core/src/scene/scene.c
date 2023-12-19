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

    // On init le tableau des lumières
    scene->lights.tab = malloc(STEP * sizeof(PointLight));
    scene->lights.nb = 0;
    scene->lights.max = STEP;

    // On init le tableau des materiaux
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

    Color light = {0, 0, 0};
    Color rayColor = {1.0, 1.0, 1.0};

    // On boucle tant qu'on ne touche pas le ciel
    for (unsigned int i = 0; i < scene->maxBounces; ++i)
    {
        HitInfo hit = launchRay(scene, ray);

        if (hit.type == NONE)
        {
            // si le rayon ne touche rien, on récupère la couleur du ciel
            //Color sky = {.5, .5, .5};
            Color sky = computeSkyColor(scene, ray);
            sky = multiplyColorc(&sky, &rayColor);
            light = addColor(&light, &sky);
            break;
        }
        // On récupère le material touché
        Material matHit = ((Material *)scene->materials.tab)[hit.materialIndice];

        // En fonction du type de materiaux
        if(matHit.type & EMISSIVE)
        {
            Color emission = multiplyColord(&matHit.emissionColor, matHit.emissionPower);
            light = addColor(&light, &emission);
        }
        rayColor = multiplyColorc(&rayColor, &matHit.albedo);

        //Oriente le rebonds spéculaire
        Vector random = randomFrom(-0.5, 0.5);
        //Avec facteur en fonction du roughness
        Vector devia = mul(&random, matHit.roughness);
        devia = add(&devia, &hit.hitNormal);
        normalize(&devia);

        ray->o = hit.hitPoint;
        //On recule le point par rapport à la normal du hit (pour limiter l'acne)
        Vector bias = mul(&hit.hitNormal, 0.0000000001);
        ray->o = add(&ray->o, &bias);
        ray->v = reflect(&ray->v, &devia);
    }
    return light;
}

/**
 * Soon useless
*/
Color computeColor(Scene *scene, HitInfo *hit)
{
    // couleur de l'éclairage direct de base
    Material mat = ((Material *)scene->materials.tab)[hit->materialIndice];

    Color ambiant = multiplyColorc(&scene->ambiant, &mat.albedo);
    Color diffuse = {0, 0, 0};
    Color specular = {0, 0, 0};

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
        //toLightRay = move(&toLightRay, 0.0000001);

        HitInfo shadowHit = launchRay(scene, &toLightRay);

        // Si aucune collision, on calcule la couleur diffuse en fonction de la lumière testée
        if (shadowHit.type == NONE)
        {
            double cosTheta = dot(&hit->hitNormal, &toLight);

            // si l'angle > 0 && < PI (180 deg)
            if (cosTheta >= -1 && cosTheta <= M_PI_2)
            {
                
                double atenuation = 1. / distanceLight2;
                // On calcule la couleur diffuse
                double intensity = fmax(0.0, cosTheta) * pLight->intensity * atenuation;
                Color lightColor = multiplyColord(&pLight->color, intensity);
                // On accumule la couleur au point d'impact pour toutes les lampe
                diffuse = addColor(&diffuse, &lightColor);

                // On calcule la couleur spéculaire
                Vector R = reflect(&hit->originalRay.v, &hit->hitNormal);
                double coeff = dot(&R, &toLight);
                coeff = fmax(coeff, 0);
                coeff = pow(coeff, mat.shininess);
                Color icoeff = multiplyColord(&pLight->color, coeff);
                icoeff = multiplyColord(&icoeff, atenuation);
                specular = addColor(&specular, &icoeff);
            }
        }
    }

    diffuse = multiplyColorc(&mat.albedo, &diffuse);
    Color blanc = {1, 1, 1};
    specular = multiplyColorc(&blanc, &specular);

    // On a accumulé toutes les lumières au point d'impacte
    // clampColor(&directLight, 0, 1);
    Color res = addColor(&ambiant, &diffuse);
    res = addColor(&res, &specular);

    clampColor(&res, 0, 1);
    return res;
}

Color computeSkyColor(Scene *scene, Ray *ray)
{
    // Direction du rayon normalisée
    Vector normalizedDirection = ray->v;

    double t = 0.5 * (normalizedDirection.y + 1.0);
    Color horizon = {1, 0.8823529, 0.7701960};
    //{1, 1, 0.8}
    //mieux 1, 0.8823529, 0.7701960

    Color tmpDeb = multiplyColord(&horizon, (1 - t));
    Color tmpFin = multiplyColord(&scene->sky, t);

    Color skyColor = addColor(&tmpDeb, &tmpFin);
    //Color skyColor = {0.3, 0.3, 0.5}; //Debug

    return skyColor;
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

void addModel(Scene *scene, Sphere *sphere)
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
}

void addLight(Scene *scene, PointLight *pLight)
{
    DynamicArray *lightArray = &scene->lights;
    // Si la taille du tableau ne suffis plus, on l'agrandi de STEP
    if (lightArray->nb == lightArray->max)
    {
        lightArray->max += STEP;
        lightArray->tab = realloc(lightArray->tab, lightArray->max * sizeof(PointLight));
    }

    ((PointLight *)lightArray->tab)[lightArray->nb] = *pLight;
    lightArray->nb++;
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