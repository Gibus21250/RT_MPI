#ifndef SCREEN_H
#define SCREEN_H

#include "utils/color.h"
#include "utils/vector.h"

#include "scene/scene.h"
#include "scene/camera.h"

typedef struct Screen
{
    unsigned int l, L;
    unsigned int nbSample;
    unsigned int maxSample;
    Color *screen;
    Color *accumulator;
} Screen;

/**
 * Aloue la mémoire nécéssaire pour gérer l'écran
 */
void initScreen(Screen *screen);
void clearScreen(Screen *screen);
void destroyScreen(Screen *screen);

// Une itération d'un lancé de rayon sur tout l'écran, stocké dans l'accumulateur
void draw(Screen *screen, Camera *camera, Scene *scene);
void updateRendered(Screen *screen);

/**
 * Print les valeurs des pixels dans stdout !!! Debug de dernier recours
 */
void showResult(Screen *screen);

#endif