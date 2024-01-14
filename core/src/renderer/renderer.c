#include "renderer/renderer.h"

#include "utils/math.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

void initRenderer(Renderer *renderer)
{
    renderer->accumulator = (Color *)calloc(renderer->l * renderer->L, sizeof(Color));
}

void destroyRenderer(Renderer *renderer)
{
    free(renderer->accumulator);
}

void render(Renderer *renderer, Scene *scene, Camera *camera)
{
    double biaisx = 2 * (double)1 / renderer->l;
    double biaisy = 2 * (double)1 / renderer->L;

    double halfFOV = tan(camera->fov * 0.5 * M_PI / 180.0);
    double aspect = (double)renderer->L / renderer->l;

    Vector up = camera->up;
    Vector direction = sub(&camera->lookAt, &camera->position);
    normalize(&direction);
    Vector right = cross(&up, &direction);

    // On lance un rayon pour chaque pixel de l'écran
    for (unsigned int i = 0; i < renderer->L; i++)
    {
        // Dessiner la ligne
        for (unsigned int j = 0; j < renderer->l; ++j)
        {
            // On commence en haut à gauche, et on lance un rayon au hasard dans le pixel (Monte Carlo)
            double xEcran = 2 * ((double)j / renderer->l) - 1;
            xEcran += biaisx * ((double)rand()) / RAND_MAX;

            double yEcran = 1 - 2 * ((double)i / renderer->L);
            yEcran -= biaisy * ((double)rand()) / RAND_MAX;

            Vector rm = mul(&right, xEcran * halfFOV * aspect);
            Vector um = mul(&up, yEcran * halfFOV);
            Vector rayDir = add(&rm, &um);
            Vector rayDirection = add(&rayDir, &direction);
            normalize(&rayDirection);

            Ray ray = {camera->position, rayDirection};

            // On récupère le résultat du lancé du rayon à travers la scène
            Color pixelColor = computeColor(renderer, scene, &ray);
            // On ajoute le résultat du pixel dans l'accumulateur
            renderer->accumulator[i * renderer->l + j] = addColor(&renderer->accumulator[i * renderer->l + j], &pixelColor);
        }
    }
    // On incrémente le nombre de sample calculés
    renderer->currentSample++;
}

Color computeColor(Renderer *renderer, Scene *scene, Ray *ray)
{

    Color light = {0, 0, 0};
    Color rayColor = {1.0, 1.0, 1.0};

    // On boucle tant qu'on ne touche pas le ciel && nbRebond < maxRebond
    for (unsigned int i = 0; i < renderer->maxBounces; ++i)
    {
        HitInfo hit = launchRay(scene, ray);

        if (hit.type == NONE)
        {
            // si le rayon ne touche rien, on récupère la couleur du ciel
            // Color sky = {.5, .5, .5};
            Color sky = computeSkyColor(scene, ray);
            sky = multiplyColorc(&sky, &rayColor);
            light = addColor(&light, &sky);
            break;
        }
        // On récupère le material touché
        Material matHit = ((Material *)scene->materials.tab)[hit.materialIndice];

        // En fonction du type de materiaux
        if (matHit.type & EMISSIVE)
        {
            Color emission = multiplyColord(&matHit.emissionColor, matHit.emissionPower);
            light = addColor(&light, &emission);
        }

        // Absorbtion par la couleur deu material
        rayColor = multiplyColorc(&rayColor, &matHit.albedo);

        if (((double)rand() / RAND_MAX) > matHit.roughness)
        {
            // Oriente le rebonds spéculaire
            Vector random = randomFrom(-0.5, 0.5);
            // Avec facteur en fonction du roughness
            Vector devia = mul(&random, matHit.roughness);
            devia = add(&devia, &hit.hitNormal);
            normalize(&devia);

            ray->o = hit.hitPoint;
            // On recule le point par rapport à la normal du hit (pour limiter l'acne)
            Vector bias = mul(&hit.hitNormal, 0.0000000001);
            ray->o = add(&ray->o, &bias);
            ray->v = reflect(&ray->v, &devia);
        }
        else
        {
            ray->o = hit.hitPoint;
            Vector bias = mul(&hit.hitNormal, 0.0000000001);
            ray->o = add(&ray->o, &bias);

            ray->v = randomRayHemisphere(&hit.hitNormal);
        }
    }
    return light;
}

Color computeSkyColor(Scene *scene, Ray *ray)
{
    // Direction du rayon normalisée
    Vector normalizedDirection = ray->v;

    double t = 0.5 * (normalizedDirection.y + 1.0);
    Color horizon = {1, 0.8823529, 0.7701960};
    //{1, 1, 0.8}
    // mieux 1, 0.8823529, 0.7701960

    Color tmpDeb = multiplyColord(&horizon, (1 - t));
    Color tmpFin = multiplyColord(&scene->sky, t);

    Color skyColor = addColor(&tmpDeb, &tmpFin);
    // Color skyColor = {0.3, 0.3, 0.5}; //Debug

    return skyColor;
}

void updateResult(Renderer *renderer, Screen *screen)
{
    for (unsigned int i = 0; i < renderer->L; i++)
    {
        for (unsigned int j = 0; j < renderer->l; ++j)
        {
            // Actualiser l'affichage
            Color accumulatedColor = renderer->accumulator[i * renderer->l + j];
            accumulatedColor = multiplyColord(&accumulatedColor, 1. / renderer->currentSample);
            clampColor(&accumulatedColor, 0, 1);
            screen->screen[i * screen->l + j] = accumulatedColor;
        }
    }
}

void clearAccumulator(Renderer *renderer)
{
    renderer->currentSample = 0;
    memset(renderer->accumulator, 0, renderer->L * renderer->l * sizeof(Color));
}