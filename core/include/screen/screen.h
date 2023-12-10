#ifndef SCREEN_H
#define SCREEN_H

#include "utils/color.h"
#include "utils/vector.h"

#include "scene/scene.h"
#include "scene/camera.h"

typedef struct Screen {
    Color *screen;
    unsigned int l, L;
} Screen;

/**
 * Aloue la mémoire nécéssaire pour gérer l'écran
*/
void initScreen(Screen *screen);
void clearScreen(Screen *screen);
void destroyScreen(Screen *screen);

//Remplie l'écran de couleur des rayons envoyé à travers la scène
void draw(Screen* screen, Camera *camera, Scene *scene);

void showResult(Screen *screen);

#endif