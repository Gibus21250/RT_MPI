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

    //printf("%lf %lf\n", biaisx, biaisy);
    //On va lancer un rayon depuis chaque pixel de l'écran
    //Toute les lignes

    //Si la caméra est orthogonlale/parallele:
    if(camera->type == PARALLEL)
    {
        for (unsigned int i = 0; i < screen->L; i++)
        {
            //Toutes les colonnes de chaque ligne
            for (unsigned int j = 0; j < screen->l; ++j)
            {
                // Calcul des coordonnées normalisées du pixel sur l'écran

                double xEcran = 2 * ((double) j / (double) screen->l) - 1;
                double yEcran = 1 - 2 * ((double) i / (double) screen->L);
                //TODO changer pour que l'écran soit construit en fonction de la direction regardé par la caméra
                Ray ray = {{xEcran + biaisx, yEcran - biaisy, 0}, camera->projection.parallel.direction};

                screen->screen[i * screen->l + j] = launchRay(scene, &ray);
            }
        }
    }
    else //Camera perspective
    {
        // Calcul de la base de la caméra
        Vector view = sub(&camera->projection.perspective.lookAt, &camera->projection.perspective.position);
        normalize(&view);

        Vector right = cross(&camera->projection.perspective.up, &view);
        normalize(&right);

        Vector up = cross(&view, &right);

        // Point de perspective (centre de l'écran)
        Vector perspective_point = add(&camera->projection.perspective.position, &view);

        // Distance entre la caméra et le plan de l'écran
        double distance_to_screen = camera->projection.perspective.distance;

        for (unsigned int i = 0; i < screen->L; i++)
        {
            //Toutes les colonnes de chaque ligne
            for (unsigned int j = 0; j < screen->l; ++j)
            {
                // Calcul des coordonnées normalisées du pixel sur l'écran
                double xEcran = 2 * ((double) j / (double) screen->l) - 1;
                double yEcran = 1 - 2 * ((double) i / (double) screen->L);

                // Calcul de la direction du rayon en fonction des coordonnées du pixel
                Vector rx = mul(&right, xEcran); //Composante droite de la direction du rayon
                Vector uy = mul(&up, yEcran);   //Composante haut de la direction du rayon
                Vector rxup = add(&rx, &uy);    //Direction du vecteur (depuis l'origine de l'écran)

                Vector vdist = mul(&view, distance_to_screen);

                //Direction du rayon, depuis le point de vue (position)
                Vector ray_direction = add(&rxup, &vdist);
                normalize(&ray_direction);

                Ray ray = {perspective_point, ray_direction};

                screen->screen[i * screen->l + j] = launchRay(scene, &ray);
            }
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
