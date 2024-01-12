#ifndef SCREEN_H
#define SCREEN_H

#include "utils/color.h"
#include "utils/vector.h"

#include "scene/scene.h"
#include "scene/camera.h"

typedef struct Screen
{
    unsigned int l, L;
    Color *screen;
} Screen;

/**
 * Aloue la mémoire nécéssaire pour gérer l'écran
 */
void initScreen(Screen *screen);
void clearScreen(Screen *screen);
void destroyScreen(Screen *screen);

/**
 * Print les valeurs des pixels dans stdout !!! Debug de dernier recours
 */
void showResult(Screen *screen);

#endif