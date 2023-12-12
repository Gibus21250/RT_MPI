#include <stdio.h>
#include <stdlib.h>

#include "screen/screen.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "scene/light.h"

#define PAS 0.1

#include "utils/color.h"
#include "exporter/saver.h"

#include "SDL2/SDL.h"

int main(int argc, char const **argv)
{
    //64 36 -> 16/9
    Screen ecran = {NULL, 720, 480};
    Scene scene = {
        .ambiant = {0.05, 0.05, 0.05},
        .sky = {0.1, 0.1, 0.1}
    };

    Camera camera = {
        .position = {2, 2, 2},
        .lookAt = {0, 0, 0},
        .up = {0, 1, 0},
        .distance = 1,
        .fov = 90
    };

    PointLight light = {{1, 1, 1}, 1, {1, 1, 1}};
    PointLight light2 = {{0, 0, 0}, 0.2, {1, 0, 0}};
    PointLight light3 = {{0, -1, 0}, 0.2, {0, 1, 0}};

    initScreen(&ecran);
    initScene(&scene);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Ray Tracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ecran.l, ecran.L, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Sphere x = {{1, 0, 0}, 0.2, {1, 0, 0}};
    Sphere y = {{0, 1, 0}, 0.2, {0, 1, 0}};
    Sphere z = {{0, 0, 1}, 0.2, {0, 0, 1}};
    Sphere centre = {{1, 1, 1}, 0.2, {0, 0, 0}};
    Sphere test = {{1, 0, 0}, 10, {0, -10.5, 0}};

    addModel(&scene, &x);
    addModel(&scene, &y);
    addModel(&scene, &z);
    addModel(&scene, &centre);
    addModel(&scene, &test);

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
            // Événement de pression de touche
            switch (event.key.keysym.sym) {
                case SDLK_z:
                    // Déplacez la caméra vers l'avant (augmentez la position en Z)
                    camera.position.z += PAS;
                    break;
                case SDLK_s:
                    if (event.key.keysym.mod & KMOD_CTRL)
                    {
                        printf("Sauvegarde...\n");
                        savePPMP6(ecran.screen, ecran.l, ecran.L, "premier6.ppm");
                        printf("Sauvé!\n");
                    } else
                        camera.position.z -= PAS;
                    break;
                case SDLK_q:
                    // Déplacez la caméra vers la gauche (diminuez la position en X)
                    camera.position.x -= PAS;
                    break;
                case SDLK_d:
                    // Déplacez la caméra vers la droite (augmentez la position en X)
                    camera.position.x += PAS;
                    break;
                case SDLK_e:
                    // Déplacez la caméra vers la gauche (diminuez la position en X)
                    camera.position.y += PAS;
                    break;
                case SDLK_a:
                    // Déplacez la caméra vers la droite (augmentez la position en X)
                    camera.position.y -= PAS;
                    break;
                case SDLK_i:
                    // Déplacez la caméra vers l'avant (augmentez la position en Z)
                    camera.lookAt.z += PAS;
                    break;
                case SDLK_k:
                    // Déplacez la caméra vers l'arrière (diminuez la position en Z)
                    camera.lookAt.z -= PAS;
                    break;
                case SDLK_j:
                    // Déplacez la caméra vers la gauche (diminuez la position en X)
                    camera.lookAt.x -= PAS;
                    break;
                case SDLK_l:
                    // Déplacez la caméra vers la droite (augmentez la position en X)
                    camera.lookAt.x += PAS;
                    break;
                case SDLK_o:
                    camera.lookAt.y += PAS;
                    break;
                case SDLK_u:
                    camera.lookAt.y -= PAS;
                    break;
                case SDLK_SEMICOLON:
                    camera.fov += 1;
                    break;
                case SDLK_COMMA:
                    camera.fov -= 1;
                    break;
            }
        }
        //printf("pos: %lf %lf %lf, lookat:%lf %lf %lf\n", camera.position.x, camera.position.y, camera.position.z, camera.lookAt.x, camera.lookAt.y, camera.lookAt.z);
        }

        draw(&ecran, &camera, &scene);
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
