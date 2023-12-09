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

    //Calcule du biais (pour lancer le rayon au centre du pixel, au lieu d'un coin)
    double biaisx = (double) 1 / screen->L;
    double biaisy = (double) 1 / screen->l;

    printf("%lf %lf\n", biaisx, biaisy);
    //On va lancer un rayon depuis chaque pixel de l'écran
    //Toute les lignes
    for (unsigned int i = 0; i < screen->L; i++)
    {
        //Toutes les colonnes de chaque ligne
        for (unsigned int j = 0; j < screen->l; ++j)
        {
            // Calcul des coordonnées normalisées du pixel sur l'écran

            double xEcran = 2 * ((double) j / (double) screen->l) - 1;
            double yEcran = 1 - 2 * ((double) i / (double) screen->L);

            Ray ray = {{xEcran + biaisx, yEcran - biaisy, 0}, {0, 0, 1}};

            screen->screen[i * screen->l + j] = launchRay(scene, &ray);
        }
    }
    
}






/*


double x_normalized = pixelsx + (2.0 * j - screen->L) / screen->L;
            double y_normalized = pixelsy + (2.0 * i - screen->l) / screen->l;

*/

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
