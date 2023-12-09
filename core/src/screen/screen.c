#include "screen/screen.h"

#include "exporter/color.h"
#include "scene/scene.h"
#include "models/ray.h"
#include "math/vector.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void draw(Screen* screen, Camera *camera, Scene *scene)
{

    //On va lancer un rayon depuis chaque pixel de l'écran
    for (unsigned int i = 0; i < screen->l; i++)
    {
        for (unsigned int j = 0; j < screen->L; ++j)
        {
            
            // Calcul des coordonnées normalisées du pixel sur l'écran
            double x_normalized = (2.0 * j - screen->L) / screen->L;
            double y_normalized = (2.0 * i - screen->l) / screen->l;

            Ray ray = {{x_normalized, y_normalized, 0}, {0, 0, 1}};
            screen->screen[i * screen->L + j] = launchRay(scene, &ray);
        }
    }
    
}

void showResult(Screen *screen)
{
    for(unsigned int i = 0; i < screen->l; ++i)
    {
        for(unsigned int j = 0; j < screen->L; ++j)
        {
            Color tmp = screen->screen[i * screen->L + j];
            
            printf("(%f %f %f) ", tmp.r, tmp.g, tmp.b);
        }
    }
}

void initScreen(Screen *screen)
{
    screen->screen = (Color*) malloc(screen->l * screen->L * sizeof(Color));
}

void clearScreen(Screen *screen)
{
    memset(screen, 0, screen->l * screen->L * sizeof(Color));
}

void destroyScreen(Screen *screen)
{
    free(screen->screen);
}
