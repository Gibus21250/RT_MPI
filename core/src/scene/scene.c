#include "scene/scene.h"

#include "utils/vector.h"
#include "utils/math.h"
#include "models/material.h"
#include "models/cube.h"
#include "models/tore.h"

#include "renderer/intersector.h"

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

    scene->cubes.tab = malloc(STEP * sizeof(Cube));
    scene->cubes.nb = 0;
    scene->cubes.max = STEP;

    scene->tores.tab = malloc(STEP * sizeof(Tore));
    scene->tores.nb = 0;
    scene->tores.max = STEP;

    scene->planes.tab = malloc(STEP * sizeof(Plane));
    scene->planes.nb = 0;
    scene->planes.max = STEP;

    scene->rectangles.tab = malloc(STEP * sizeof(Rectangle));
    scene->rectangles.nb = 0;
    scene->rectangles.max = STEP;

    // On init le tableau des materiaux
    scene->materials.tab = malloc(STEP * sizeof(Material));
    scene->materials.nb = 0;
    scene->materials.max = STEP;
}

void destroyScene(Scene *scene)
{
    free(scene->spheres.tab);
    free(scene->cubes.tab);
    free(scene->tores.tab);
    free(scene->planes.tab);
    free(scene->rectangles.tab);
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
    DynamicArray *array = &scene->spheres;

    // Pour chaque sphere
    for (unsigned int i = 0; i < array->nb; ++i)
    {
        Sphere *sphere = &((Sphere *)array->tab)[i];

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

    array = &scene->cubes;
    //Pour chaque Cube
    for (unsigned int i = 0; i < array->nb; ++i)
    {
        Cube *cube = &((Cube *)array->tab)[i];

        double tTmp = intersectCube(ray, cube);

        // printf("%lf\n", tTmp);
        if (tTmp > 0)
        {

            // On récupère le point à l'intersection
            Vector hitPoint = move(ray, tTmp).o;

            Vector normal = {1,1,1};/*
            Vector dir = add(&hitPoint,&cube->center);

            Vector mod = {dir.x,dir.y,dir.z};

            if(mod.x < 0.0){mod.x = -1.0*mod.x;}
            if(mod.y < 0.0){mod.y = -1.0*mod.y;}
            if(mod.z < 0.0){mod.z = -1.0*mod.z;}




            if (mod.x > mod.y && mod.x > mod.z)
            {
                printf("x :%lf \n",dir.x);
                if (dir.x > 0.0)
                {
                    Vector plan = {1, 0, 0};
                    normal = add(&normal,&plan);
                    normal = add(&hitPoint,&normal);
                }

                else
                {
                    Vector plan = {-1, 0, 0};
                    normal = add(&normal,&plan);
                    normal = add(&hitPoint,&normal);
                }
            }

            else if (mod.y > mod.x && mod.y > mod.z)
            {
                printf("y :%lf \n",dir.y);
                if (dir.y > 0.0)
                {
                    Vector plan = {0,1, 0};
                    normal = add(&normal,&plan);
                    normal = add(&hitPoint,&normal);
                }

                else
                {
    
                    Vector plan = {0, -1, 0};
                    Vector normal = add(&normal,&plan);
                    normal = add(&hitPoint,&normal);
                }

            }

            else if (mod.z > mod.x && mod.z > mod.y)
            {
                printf("z :%lf \n",dir.z);
                if (dir.z > 0.0)
                {

                    Vector plan = {0, 0, 1};
                    normal = add(&normal,&plan);
                    normal = add(&hitPoint,&normal);
                }

                else
                {
                    Vector plan = {0, 0,  -1};
                    normal = add(&normal,&plan);
                    normal = add(&hitPoint,&normal);
                }

            }*/

            // Vecteur origine -> hitPoint
            normalize(&normal);
            //printf("%lf %lf %lf \n",normal.x,normal.y,normal.z);
            // dist carré
            Vector cHit = sub(&hitPoint,&ray->o);
            double dist = dot(&cHit, &cHit);
            // printf("Colision!\n", tTmp);
            // Colision

            // Colision
            if (dist < closestHit.distance2)
            {
                closestHit.type = CUBE;
                closestHit.modelIndice = i;
                closestHit.materialIndice = cube->materialIndice;
                closestHit.distance2 = dist;
                closestHit.hitPoint = hitPoint;
                closestHit.hitNormal = normal; // Utiliser la normale calculée plutôt que de la normaliser à nouveau
            }
        }
    }

    array = &scene->tores;
    Vector hitpoints[4];
    //Pour chaque Tore
    for (unsigned int i = 0; i < array->nb; ++i)
    {
        Tore *tore = &((Tore *)array->tab)[i];

        int nbHit = intersectTore(ray, tore, &hitpoints);

        // printf("%lf\n", tTmp);
        if (nbHit > 0)
        {   
            int closestPoint = 0;
            //On prend par default le premier points
            double dist = dot(&hitpoints[0], &hitpoints[0]);

            //Pour tous les autres points, on regarde s'il seraient pas plus prochent
            for (int i = 1; i < nbHit; i++)
            {
                // Vecteur origine -> hitPoint
                Vector cHit = sub(&hitpoints[i], &ray->o);
                double tmp = dot(&cHit, &cHit);
                if(tmp < dist)
                {
                    dist = tmp;
                    closestPoint = i;
                }
            }

            // Colision
            if (dist < closestHit.distance2)
            {
                closestHit.type = TORE;
                closestHit.modelIndice = i;
                closestHit.materialIndice = tore->materialIndice;
                closestHit.distance2 = dist;
                closestHit.hitPoint = hitpoints[closestPoint];
                closestHit.hitNormal = calculateToreNormal(&hitpoints[closestPoint], tore);
                normalize(&closestHit.hitNormal);

            }
        }
    }

    
    array = &scene->planes;
    for (unsigned int i = 0; i < array->nb; ++i)
    {
        Plane *plane = &((Plane *)array->tab)[i];

        double tTmp = intersectPlane(ray, plane);

        if(tTmp > 0)
        {
            // On récupère le point à l'intersection
            Vector hitPoint = move(ray, tTmp).o;

            // Vecteur origine -> hitPoint
            Vector cHit = sub(&hitPoint, &ray->o);

            double dist = dot(&cHit, &cHit);

            //Si collision plus proche
            if (dist < closestHit.distance2)
            {
                closestHit.type = PLANE;
                closestHit.modelIndice = i;
                closestHit.materialIndice = plane->materialIndice;
                closestHit.distance2 = dist;
                closestHit.hitPoint = hitPoint;
                double angle = dot(&plane->normal, &ray->v);
                if(angle > 0)
                    closestHit.hitNormal = mul(&plane->normal, -1);
                else
                    closestHit.hitNormal = plane->normal;
            }
        }
    }
    
    array = &scene->rectangles;
    for (unsigned int i = 0; i < array->nb; ++i)
    {
        Rectangle *rec = &((Rectangle *)array->tab)[i];

        double tTmp = intersectRectangle(ray, rec);

        if(tTmp > 0)
        {
            // On récupère le point à l'intersection
            Vector hitPoint = move(ray, tTmp).o;

            // Vecteur origine -> hitPoint
            Vector cHit = sub(&hitPoint, &ray->o);

            double dist = dot(&cHit, &cHit);

            //Si collision plus proche
            if (dist < closestHit.distance2)
            {
                closestHit.type = PLANE;
                closestHit.modelIndice = i;
                closestHit.materialIndice = rec->materialIndice;
                closestHit.distance2 = dist;
                closestHit.hitPoint = hitPoint;
                double angle = dot(&rec->normal, &ray->v);
                if(angle > 0)
                    closestHit.hitNormal = mul(&rec->normal, -1);
                else
                    closestHit.hitNormal = rec->normal;
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

unsigned int addModel(Scene *scene, void *element, ModelType type)
{
    DynamicArray *array = 0;
    
    switch (type)
    {
    case SPHERE:
        array = &scene->spheres;
        // Si la taille du tableau ne suffis plus, on l'agrandi de STEP
        if (array->nb == array->max)
        {
            array->max += STEP;
            array->tab = realloc(array->tab, array->max * sizeof(Sphere));
        }
        ((Sphere *)array->tab)[array->nb] = *((Sphere*) element);
        break;

    case CUBE:
        array = &scene->cubes;
        // Si la taille du tableau ne suffis plus, on l'agrandi de STEP
        if (array->nb == array->max)
        {
            array->max += STEP;
            array->tab = realloc(array->tab, array->max * sizeof(Cube));
        }
        ((Cube *)array->tab)[array->nb] = *((Cube*) element);
        break;

    case TORE:
        array = &scene->tores;
        // Si la taille du tableau ne suffis plus, on l'agrandi de STEP
        if (array->nb == array->max)
        {
            array->max += STEP;
            array->tab = realloc(array->tab, array->max * sizeof(Tore));
        }
        ((Tore *)array->tab)[array->nb] = *((Tore*) element);
        break;
    case PLANE:
        array = &scene->planes;
        // Si la taille du tableau ne suffis plus, on l'agrandi de STEP
        if (array->nb == array->max)
        {
            array->max += STEP;
            array->tab = realloc(array->tab, array->max * sizeof(Plane));
        }
        ((Plane *)array->tab)[array->nb] = *((Plane*) element);
        break;
    case RECTANGLE:
        array = &scene->rectangles;
        // Si la taille du tableau ne suffis plus, on l'agrandi de STEP
        if (array->nb == array->max)
        {
            array->max += STEP;
            array->tab = realloc(array->tab, array->max * sizeof(Rectangle));
        }
        ((Rectangle *)array->tab)[array->nb] = *((Rectangle*) element);
        break;

    case NONE:
        break;

    }

    array->nb++;
    return array->nb-1;
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