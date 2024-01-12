#ifndef RENDERER_H
#define RENDERER_H

#include "utils/color.h"
#include "screen/screen.h"

#include "scene/scene.h"
#include "scene/camera.h"

typedef struct Renderer
{
    unsigned int maxSample;
    unsigned int currentSample;
    unsigned int maxBounces;
    unsigned int l, L;
    Color *accumulator;
} Renderer;

void initRenderer(Renderer *renderer);
void destroyRenderer(Renderer *renderer);

/**
 * Lance un rendu (= 1 sample) de la scène, à la vue de la camera et stock dans l'accumulation
*/
void render(Renderer *renderer, Scene *scene, Camera *camera);

/**
 * Calcule la couleur à partir du rayon
*/
Color computeColor(Renderer *renderer, Scene *scene, Ray *ray);

Color computeSkyColor(Scene *scene, Ray *ray);

/**
 * Remplie l'ecran en parametre, avec le rendu accumulé
*/
void updateResult(Renderer *renderer, Screen *screen);

void clearAccumulator(Renderer *renderer);


#endif