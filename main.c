#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "screen/screen.h"

#include "scene/scene.h"
#include "scene/camera.h"
#include "scene/light.h"

#include "renderer/renderer.h"


#include "models/material.h"
#include "models/sphere.h"
#include "models/tore.h"
#include "models/cube.h"
#include "models/rectangle.h"
#include "models/plane.h"

#include "animator/animator.h"

#include "utils/color.h"
#include "exporter/saver.h"

#define PAS 0.1

#include "SDL2/SDL.h"

int mouseX, mouseY;
int prevMouseX, prevMouseY;

int main(int argc, char const **argv)
{
    // 64 36 -> 16/9
    Screen ecran = {
        .l = 720,
        .L = 480};

    Scene scene = {
        .sky = {0, 0.4, 0.7}}; // bleu clair {0, 0.5, 0.8}

    Camera camera = {
        .position = {2, 1, 2},
        .lookAt = {-1, .5, -1},
        .up = {0, 1, 0},
        .distance = 1,
        .fov = 90};

    Renderer renderer = {
        .l = ecran.l,
        .L = ecran.L,
        .maxSample = INT32_MAX,
        .currentSample = 0,
        .maxBounces = 4};

    Animator animator;

    initScreen(&ecran);
    initScene(&scene);
    initRenderer(&renderer);
    initAnimator(&animator);

    const char title[100];
    sprintf(title, "Monte Carlo Path Tracing - %d bounces | samples: %d", renderer.maxBounces, renderer.currentSample);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *sdl_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ecran.l, ecran.L, SDL_WINDOW_SHOWN);
    SDL_Renderer *sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);

    // On commence par initialiser les materiaux:
    Material matx = {
        .type = DIFFUSE,
        .albedo = {1, 0, 0},
        .roughness = 1,
        .metalness = 0};

    Material maty = {
        .type = DIFFUSE,
        .albedo = {0, 1, 0},
        .roughness = 0.1,
        .metalness = 0};

    Material matz = {
        .type = DIFFUSE,
        .albedo = {0, 0, 1},
        .roughness = 0,
        .metalness = 0};

    Material blanc = {
        .type = DIFFUSE | EMISSIVE,
        .albedo = {1, 1, 1},
        .roughness = 1,
        .emissionColor = {1, 1, 1},
        .emissionPower = 1};

    Material magenta = {
        .type = DIFFUSE,
        .albedo = {1, 0, 1},
        .roughness = 1};

    Material matvertforet = {
        .type = DIFFUSE,
        .albedo = {34/255., 139/255., 34/255.},
        .roughness = .8};

    Material matsoleil = {
        .type = DIFFUSE | EMISSIVE,
        .albedo = {0.8, 0.5, 0.2},
        .roughness = 1,
        .emissionColor = {0.8, 0.5, 0.2},
        .emissionPower = 1};

    Material matTore = {
        .type = DIFFUSE,
        .albedo = {0.5, .8, .9},
        .roughness = 0
    };

    Material matblack = {
        .type = DIFFUSE,
        .albedo = {0, 0, 0},
        .roughness = 1
    };

    Material matmirror = {
        .type = DIFFUSE,
        .albedo = {1, 1, 1},
        .roughness = 0
    };

    // On ajoute les matériaux à la scène
    unsigned int imatx = addMaterial(&scene, &matx);
    unsigned int imaty = addMaterial(&scene, &maty);
    unsigned int imatz = addMaterial(&scene, &matz);
    unsigned int imatblanc = addMaterial(&scene, &blanc);
    unsigned int imatmagenta = addMaterial(&scene, &magenta);
    unsigned int imatsoleil = addMaterial(&scene, &matsoleil);
    unsigned int imatTore = addMaterial(&scene, &matTore);
    unsigned int imatvertforet = addMaterial(&scene, &matvertforet);
    unsigned int imatblack = addMaterial(&scene, &matblack);
    unsigned int imatmirror = addMaterial(&scene, &matmirror);


    // On initialise les éléments de la scène
    Sphere x = {{1, 0.2, 0}, 0.2, imatx};
    Sphere y = {{0, 1.2, 0}, 0.2, imaty};
    Sphere z = {{0, 0.2, 1}, 0.2, imatz};

    Cube c = {{1, 0, 1},{2, 1, 2},imatblanc};

    Sphere centre = {{0, 0.2, 0}, 0.2, imatblanc};

    Sphere sol = {{0, -10, 0}, 10, imatmagenta};
    Sphere soleil = {{-20, 5, -20}, 20, imatsoleil};

    // On ajoute les éléments dans la scène
    addModel(&scene, &x, SPHERE);
    addModel(&scene, &y, SPHERE);
    addModel(&scene, &z, SPHERE);
    unsigned int centreI = addModel(&scene, &centre, SPHERE);

    addModel(&scene, &sol, SPHERE);
    addModel(&scene, &soleil, SPHERE);
    addModel(&scene, &centre, SPHERE);

    Tore test = {
        {1, .3, 1},
        {0, 1, 0},
        0.05,
        0.2,
        imatblanc};

    //addModel(&scene, &test, TORE);

    // Ajouter les models à animer, à l'Animator
    // void *pCentre = pointerFrom(&scene, SPHERE, centreI);
    Vector vitesseCentre = {0, 1, 0};

    Plane testplane = {
        {0, 0, 0},
        {0, 1, 0},
        imatmagenta
    };

    addModel(&scene, &testplane, PLANE);


    Vector circlePos = {1, 1, 1};
    Vector circleLook = {0, 0, 0};
    circleLook = sub(&circleLook, &circlePos);
    normalize(&circleLook);

    Rectangle testRec = {
        circlePos,
        circleLook,
        .2, 1,
        imatsoleil
    };

    //addModel(&scene, &testRec, RECTANGLE);
    
    // addMovableElement(&animator, SPHERE, pCentre, &vitesseCentre);

    SDL_Event event;
    int running = 1;

    while (running)
    {

        /**
         * Init de la scène pour tous les autres processus
         */

        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                char ok = 0;
                double movex = 0, movey = 0, movez = 0;
                switch (event.key.keysym.sym)
                {
                case SDLK_z:
                    // Déplacez la caméra vers l'avant (augmentez la position en Z)
                    // camera.position.z += PAS;
                    movez += PAS;
                    ok = 1;
                    break;
                case SDLK_s:
                    if (event.key.keysym.mod & KMOD_CTRL)
                    {
                        printf("Sauvegarde...\n");
                        char titre[20];
                        sprintf(titre, "res%d.ppm", renderer.currentSample);
                        savePPMP6(ecran.screen, ecran.l, ecran.L, "rendusOnTheFlight", titre);
                        printf("Sauvé!\n");
                    }
                    else
                        movez -= PAS;
                    ok = 1;
                    break;
                case SDLK_q:
                    movex -= PAS;
                    ok = 1;
                    break;
                case SDLK_d:
                    movex += PAS;
                    ok = 1;
                    break;
                case SDLK_e:
                    movey += PAS;
                    ok = 1;
                    break;
                case SDLK_a:
                    movey -= PAS;
                    ok = 1;
                    break;
                case SDLK_i:
                    camera.lookAt.z += PAS;
                    ok = 1;
                    break;
                case SDLK_k:
                    camera.lookAt.z -= PAS;
                    ok = 1;
                    break;
                case SDLK_j:
                    camera.lookAt.x -= PAS;
                    ok = 1;
                    break;
                case SDLK_l:
                    camera.lookAt.x += PAS;
                    ok = 1;
                    break;
                case SDLK_o:
                    camera.lookAt.y += PAS;
                    ok = 1;
                    break;
                case SDLK_u:
                    camera.lookAt.y -= PAS;
                    ok = 1;
                    break;
                case SDLK_SEMICOLON:
                    camera.fov += 1;
                    ok = 1;
                    break;
                case SDLK_COMMA:
                    camera.fov -= 1;
                    ok = 1;
                    break;
                case SDLK_t:
                    updatePosition(&animator, 0.1);
                    clearAccumulator(&renderer);
                    break;
                case SDLK_r:
                    updatePosition(&animator, -0.1);
                    clearAccumulator(&renderer);
                    break;
                }

                if (ok)
                {
                    moveCamera(&camera, movex, movey, movez);
                    clearAccumulator(&renderer);
                }
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
            }
        }

        clock_t start, end;
        double cpu_time_used;
        start = clock();
        render(&renderer, &scene, &camera);
        end = clock();
        // Calculez le temps passé en secondes
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

        // Affichez le temps
        printf("Temps écoulé : %f secondes\n", cpu_time_used);

        updateResult(&renderer, &ecran);
        SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);

        for (unsigned int i = 0; i < ecran.L; i++)
        {
            for (unsigned int j = 0; j < ecran.l; j++)
            {
                Color pixelColor = ecran.screen[i * ecran.l + j];

                // Convertissez la couleur de votre matrice en valeurs RGBA pour SDL
                Uint8 r = (Uint8)(pixelColor.r * 255);
                Uint8 g = (Uint8)(pixelColor.g * 255);
                Uint8 b = (Uint8)(pixelColor.b * 255);
                Uint8 a = 255; // La transparence est à 255 (complètement opaque)

                // Dessinez le pixel avec la couleur convertie
                SDL_SetRenderDrawColor(sdl_renderer, r, g, b, a);
                SDL_RenderDrawPoint(sdl_renderer, j, i); // Notez que j et i sont inversés ici, assurez-vous que cela convient à votre matrice
            }
        }

        // Mettez à jour le rendu
        SDL_RenderPresent(sdl_renderer);

        sprintf(title, "Monte Carlo Path Tracing - %d bounces | samples: %d", renderer.maxBounces, renderer.currentSample);
        SDL_SetWindowTitle(sdl_window, title);

        // Ajoutez une petite pause pour limiter la fréquence d'affichage (facultatif)
        SDL_Delay(16);
    }

    printf("finit\n");

    destroyScreen(&ecran);
    destroyScene(&scene);
    destroyAnimator(&animator);
    destroyRenderer(&renderer);

    return 0;
}
