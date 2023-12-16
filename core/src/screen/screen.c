#include "screen/screen.h"

#include "utils/color.h"
#include "utils/vector.h"
#include "utils/math.h"

#include "scene/scene.h"
#include "models/ray.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

void draw(Screen *screen, Camera *camera, Scene *scene)
{
    screen->nbSample++;
    
    double biaisx = (double)1 / screen->L;
    double biaisy = (double)1 / screen->l;

    double halfFOV = tan(camera->fov * 0.5 * M_PI / 180.0);
    double aspect = (double)screen->L / screen->l;

    Vector up = camera->up;
    Vector direction = sub(&camera->lookAt, &camera->position);
    normalize(&direction);
    Vector right = cross(&up, &direction);

    //Accumuler les samples infos
    for (unsigned int i = 0; i < screen->L; i++)
    {
        // Dessiner la ligne
        for (unsigned int j = 0; j < screen->l; ++j)
        {

            double xEcran = 2 * ((double)j / screen->l) - 1;
            double yEcran = 1 - 2 * ((double)i / screen->L);

            Vector rm = mul(&right, xEcran * halfFOV * aspect);
            Vector um = mul(&up, yEcran * halfFOV);
            Vector rayDir = add(&rm, &um);
            Vector rayDirection = add(&rayDir, &direction);
            normalize(&rayDirection);

            Ray ray = {camera->position, rayDirection};
            Color pixelColor = drawPixel(scene, &ray);

            screen->accumulator[i * screen->l + j] = addColor(&screen->accumulator[i * screen->l + j], &pixelColor);

            //Actualiser l'affichage
            screen->screen[i * screen->l + j] = multiplyColord(&screen->accumulator[i * screen->l + j], (1.0 / screen->nbSample));
        }
    }
    //On incrémente le nb de sample
    
}

void showResult(Screen *screen)
{
    for (unsigned int i = 0; i < screen->l; ++i)
    {
        for (unsigned int j = 0; j < screen->L; ++j)
        {
            Color tmp = screen->screen[i * screen->L + j];

            printf("(%f %f %f) ", tmp.r, tmp.g, tmp.b);
        }
    }
}

void initScreen(Screen *screen)
{
    screen->screen = (Color *)malloc(screen->l * screen->L * sizeof(Color));
    screen->accumulator = (Color *)malloc(screen->l * screen->L * sizeof(Color));
}

void clearScreen(Screen *screen)
{
    screen->nbSample = 0;
    memset(screen->screen, 0, screen->l * screen->L * sizeof(Color));
    memset(screen->accumulator, 0, screen->l * screen->L * sizeof(Color));
}

void destroyScreen(Screen *screen)
{
    free(screen->screen);
    free(screen->accumulator);
}
