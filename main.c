#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "screen/screen.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "scene/light.h"
#include "models/material.h"

#define PAS 0.1

#include "utils/color.h"
#include "exporter/saver.h"

#include "SDL2/SDL.h"

int main(int argc, char const **argv)
{
    //64 36 -> 16/9
    Screen ecran = {
        .l = 720,
        .L = 480
    };

    Scene scene = {
        .ambiant = {0.05, 0.05, 0.05},
        .sky = {0, 0.4, 0.7},
        .maxBounces = 2
    };

    Camera camera = {
        .position = {1.5, 0, 1.5},
        .lookAt = {0, 0, 0},
        .up = {0, 1, 0},
        .distance = 1,
        .fov = 45
    };

    initScreen(&ecran);
    initScene(&scene);

    const char title[100];
    sprintf(title, "Ray Tracing (Pas encore MPI) - %d bounces | samples: %d", scene.maxBounces, ecran.nbSample);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ecran.l, ecran.L, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //On commence par initialiser les materiaux:
    Material matx = {
        .type = DIFFUSE,
        .albedo = {1, 0, 0},
        .roughness = 1,
        .specular = {1, 1, 1},
        .shininess = 1000
    };
    Material maty = {
        .type = DIFFUSE,
        .albedo = {0, 1, 0},
        .roughness = 0.1,
        .specular = {1, 1, 1},
        .shininess = 1
    };
    Material matz = {
        .type = DIFFUSE,
        .albedo = {0, 0, 0},
        .roughness = 0,
        .specular = {1, 1, 1},
        .shininess = 50
    };
    Material blanc = {
        .type = DIFFUSE,
        .albedo = {1, 1, 1},
        .roughness = 1,
        .specular = {0, 0, 0},
        .shininess = 0.5
    };
    Material magenta = {
        .type = DIFFUSE,
        .albedo = {1, 0, 1},
        .roughness = 1,
        .specular = {0, 0, 0},
        .shininess = 1
    };

    //On ajoute les matériaux à la scène
    unsigned int imatx = addMaterial(&scene, &matx);
    unsigned int imaty = addMaterial(&scene, &maty);
    unsigned int imatz = addMaterial(&scene, &matz);
    unsigned int imatblanc = addMaterial(&scene, &blanc);
    unsigned int imatmagenta = addMaterial(&scene, &magenta);

    //On initialise les éléments de la scène
    Sphere x = {{1, 0.2, 0}, 0.2, imatx};
    Sphere y = {{0, 1.2, 0}, 0.2, imaty};
    Sphere z = {{0, 0.2, 1}, 0.2, imatz};
    Sphere centre = {{0, 0.2, 0}, 0.2, imatmagenta};
    Sphere test = {{0, -10, 0}, 10, imatblanc};

    //On ajoute les éléments dans la scène
    addModel(&scene, &x);
    addModel(&scene, &y);
    addModel(&scene, &z);
    addModel(&scene, &centre);
    addModel(&scene, &test);

    PointLight light = {{-3, 3, 0}, 5, {1, 1, 1}}; //Soleil 0.972, .788, .411
    PointLight light2 = {{1, 1, 1}, 1, {1, 0, 0}};
    PointLight light3 = {{1, 0.5, 1}, 1, {0, 1, 0}};

    addLight(&scene, &light);
    //addLight(&scene, &light2);
    //addLight(&scene, &light3);

    SDL_Event event;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                char ok = 0;
                double movex = 0, movey = 0, movez = 0;
                switch (event.key.keysym.sym) {
                    case SDLK_z:
                        // Déplacez la caméra vers l'avant (augmentez la position en Z)
                        //camera.position.z += PAS;
                        movez += PAS;
                        ok = 1;
                        break;
                    case SDLK_s:
                        if (event.key.keysym.mod & KMOD_CTRL)
                        {
                            printf("Sauvegarde...\n");
                            savePPMP6(ecran.screen, ecran.l, ecran.L, "premier6.ppm");
                            printf("Sauvé!\n");
                        } else
                            movez -= PAS;
                        ok = 1;
                        break;
                    case SDLK_q:
                        // Déplacez la caméra vers la gauche (diminuez la position en X)
                        movex -= PAS;
                        //camera.position.x -= PAS;
                        ok = 1;
                        break;
                    case SDLK_d:
                        // Déplacez la caméra vers la droite (augmentez la position en X)
                        //camera.position.x += PAS;
                        movex += PAS;
                        ok = 1;
                        break;
                    case SDLK_e:
                        // Déplacez la caméra vers la gauche (diminuez la position en X)
                        //camera.position.y += PAS;
                        movey += PAS;
                        ok = 1;
                        break;
                    case SDLK_a:
                        // Déplacez la caméra vers la droite (augmentez la position en X)
                        //camera.position.y -= PAS;
                        movey -= PAS;
                        ok = 1;
                        break;
                    case SDLK_i:
                        // Déplacez la caméra vers l'avant (augmentez la position en Z)
                        camera.lookAt.z += PAS;
                        ok = 1;
                        break;
                    case SDLK_k:
                        // Déplacez la caméra vers l'arrière (diminuez la position en Z)
                        camera.lookAt.z -= PAS;
                        ok = 1;
                        break;
                    case SDLK_j:
                        // Déplacez la caméra vers la gauche (diminuez la position en X)
                        camera.lookAt.x -= PAS;
                        ok = 1;
                        break;
                    case SDLK_l:
                        // Déplacez la caméra vers la droite (augmentez la position en X)
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
                }

                if(ok)
                {
                    moveCamera(&camera, movex, movey, movez);
                    clearScreen(&ecran);
                }
                    
            } else if(event.type == SDL_MOUSEMOTION)
            {
                
            }
    
        }

        clock_t start, end;
        double cpu_time_used;
        start = clock();
        draw(&ecran, &camera, &scene);
        end = clock();
        // Calculez le temps passé en secondes
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        // Affichez le temps
        printf("Temps écoulé : %f secondes\n", cpu_time_used);
        //clear
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        for (unsigned int i = 0; i < ecran.L; i++) {
            for (unsigned int j = 0; j < ecran.l; j++) {
                Color pixelColor = ecran.screen[i * ecran.l + j];

                // Convertissez la couleur de votre matrice en valeurs RGBA pour SDL
                Uint8 r = (Uint8)(pixelColor.r * 255);
                Uint8 g = (Uint8)(pixelColor.g * 255);
                Uint8 b = (Uint8)(pixelColor.b * 255);
                Uint8 a = 255;  // La transparence est à 255 (complètement opaque)

                // Dessinez le pixel avec la couleur convertie
                SDL_SetRenderDrawColor(renderer, r, g, b, a);
                SDL_RenderDrawPoint(renderer, j, i);  // Notez que j et i sont inversés ici, assurez-vous que cela convient à votre matrice
            }
        }

        // Mettez à jour le rendu
        SDL_RenderPresent(renderer);

        sprintf(title, "Ray Tracing (Pas encore MPI) - %d bounces | samples: %d", scene.maxBounces, ecran.nbSample);
        SDL_SetWindowTitle(window, title);

        // Ajoutez une petite pause pour limiter la fréquence d'affichage (facultatif)
        SDL_Delay(16);
    }
    //showResult(&ecran);
    printf("finit\n");
    //savePPMP6(ecran.screen, ecran.l, ecran.L, "premier6.ppm");
    //savePPMP3(ecran.screen, ecran.l, ecran.L, "premier3.ppm");

    destroyScreen(&ecran);
    destroyScene(&scene);
    return 0;
}
