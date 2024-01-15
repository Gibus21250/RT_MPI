#include <stdio.h>
#include <time.h>

#include "screen/screen.h"

#include "scene/scene.h"
#include "scene/camera.h"
#include "scene/light.h"

#include "renderer/renderer.h"

#include "models/material.h"
#include "models/sphere.h"
#include "models/tore.h"
#include "models/plane.h"
#include "models/cube.h"

#include "animator/animator.h"

#include <math.h>

//MPI
#include <mpi.h>

#include "struct_types/struct_type_all.h"
#include "struct_init.h"
#include "reduce_utils.h"


int main(int argc, char const *argv[])
{
    if(argc < 6)
    {   
        printf("Veuillez renseigner:\nLongueur, largeur, Nombre de FPS, le nombre de seconde d'animation, ainsi que le nombre de sample par images!\n");
        return 0;
    }

// ! ||--------------------------------------------------------------------------------||
// ! ||                                    MPI INIT                                    ||
// ! ||--------------------------------------------------------------------------------||

    int nbProcess, my_rank;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nbProcess);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    srand(my_rank);

    int longueur = atoi(argv[1]);
    int larg = atoi(argv[2]);
    int nbFPS = atoi(argv[3]);
    int timeAnimation = atoi(argv[4]);
    int nbTotalSample = atoi(argv[5]);


    unsigned nbSamplePerProcess =  ceil(((double) nbTotalSample) / nbProcess);
    //Arrondir au dessus le nombre de sample, pour rester en multiple du nombre de processus
    nbTotalSample = nbSamplePerProcess * nbProcess;

    double dt = 1./nbFPS;
    int nbImagesTotal = nbFPS * timeAnimation;

    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    char date_str[20];
    strftime(date_str, sizeof(date_str), "rendu%m-%d-%H%M", tm_info);

    //Nom du dossier où seront les images rendu de l'animations
    char folder[20];
    sprintf(folder, "%s", date_str);

    if(my_rank == 0)
    {
        printf("Rendu de %d images à %d samples (%d samples/processus (total: %d processus))\n@%u fps pour %ds d'animation\n", nbImagesTotal, nbTotalSample, nbSamplePerProcess, nbProcess, nbFPS, timeAnimation);
    }

// ! ||--------------------------------------------------------------------------------||
// ! ||                      INITIALISATION DES STRUCTURES DU CORE                     ||
// ! ||--------------------------------------------------------------------------------||
    
    Renderer renderer = {
        .l = longueur,
        .L = larg,
        .maxSample = nbSamplePerProcess,
        .currentSample = 0,
        .maxBounces = 64
    };

    Renderer rendererGlobal = {
        .l = longueur,
        .L = larg,
        .maxSample = nbTotalSample,
        .currentSample = nbTotalSample,
        .maxBounces = 64
    };

    Scene scene = {
        .sky = {0, 0.4, 0.7}}; // bleu clair {0, 0.5, 0.8}

    Camera camera = {
        .position = {2, 1, 2},
        .lookAt = {-1, .5, -1},
        .up = {0, 1, 0},
        .distance = 1,
        .fov = 90};

    Animator animator;
    
    initMPIStruct();
    initReduceOp();

    //Chaque process initializera sa scènes, son renderer, et son animator
    initScene(&scene);
    initRenderer(&renderer);
    initAnimator(&animator);

    //Le root initialise un renderer global, utilisé pendant le reduce
    if(my_rank == 0)
    {
        initRenderer(&rendererGlobal);
    }

// ! ||--------------------------------------------------------------------------------||
// ! ||                          INITIALISATION DES MATERIAUX                          ||
// ! ||--------------------------------------------------------------------------------||
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

// ! ||--------------------------------------------------------------------------------||
// ! ||                     AJOUT DES MATERIAUX DANS LA SCENE                          ||
// ! ||--------------------------------------------------------------------------------||

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

// ! ||--------------------------------------------------------------------------------||
// ! ||                        INITIALISATION DES MODELES                             ||
// ! ||--------------------------------------------------------------------------------||
    // On initialise les éléments de la scène
    Sphere x = {{1, 0.2, 0}, 0.2, imatx};
    Sphere y = {{0, 1.2, 0}, 0.2, imaty};
    Sphere z = {{0, 0.2, 1}, 0.2, imatz};

    //Cube c = {{1, 0, 1},{1.2, .2, 1.2}, imatblanc};

    Sphere centre = {{0, 0.2, 0}, 0.2, imatblanc};

    Sphere sol = {{0, -10, 0}, 10, imatmagenta};
    Sphere soleil = {{-20, 5, -20}, 20, imatsoleil};

    Plane testplane = {
            {0, 0, 0},
            {0, 1, 0},
            imatmagenta
        };
// ! ||--------------------------------------------------------------------------------||
// ! ||                         AJOUT DES MODELES DANS LA SCENE                        ||
// ! ||--------------------------------------------------------------------------------||

    // On ajoute les éléments dans la scène
    addModel(&scene, &x, SPHERE);
    addModel(&scene, &y, SPHERE);
    addModel(&scene, &z, SPHERE);
    unsigned int centreId = addModel(&scene, &centre, SPHERE);

    //addModel(&scene, &sol, SPHERE);
    addModel(&scene, &soleil, SPHERE);
    addModel(&scene, &centre, SPHERE);
    //addModel(&scene, &c, CUBE);

    addModel(&scene, &testplane, PLANE);

// ! ||--------------------------------------------------------------------------------||
// ! ||                    AJOUT DES MODELES A ANIMER A L'ANIMATEUR                    ||
// ! ||--------------------------------------------------------------------------------||

    Vector directionc = {1, 0, 1};
    normalize(&directionc);
    //On récupère le pointeur en mémoire depuis la scène
    void *centerPointer = pointerFrom(&scene, SPHERE, centreId);

    addMovableElement(&animator, SPHERE, centerPointer, &directionc);
    

// ! ||--------------------------------------------------------------------------------||
// ! ||                      BOUCLE PRINCIPALE DU RENDU DES IMAGES                     ||
// ! ||--------------------------------------------------------------------------------||
    int nbFrame = 0;
    while (nbFrame < nbImagesTotal)
    {
        //Chaque node clear leurs accumulator
        clearAccumulator(&renderer);

        if(my_rank == 0)
        {
            clearAccumulator(&rendererGlobal);
            //Le render global sera utilisé que pour collecter toutes les accumulators
            rendererGlobal.currentSample = rendererGlobal.maxSample;
        }
            

        //Boucle de rendu
        while(renderer.currentSample < renderer.maxSample)
        {
            //Cette fonction calcul 1 sample
            render(&renderer, &scene, &camera);
        }

        //On reduce, dans l'accumulator du renderer global que le root a créé
        MPI_Reduce(renderer.accumulator, rendererGlobal.accumulator, renderer.L * renderer.l, MPI_COLOR_STRUCT, ACCUMULATOR_REDUCE_OP, 0, MPI_COMM_WORLD);
        
        //on sauvegarde l'image
        if(my_rank == 0)
        {
            char filename[20];
            sprintf(filename, "%d.ppm", nbFrame);
            //Le root initialise un écran
            Screen ecran = {
                .l = renderer.l,
                .L = renderer.L
            };

            initScreen(&ecran);

            //Il remplie son écran avec le résultat de l'accumulation de sample
            updateResult(&rendererGlobal, &ecran);

            //On sauvegarde
            savePPMP6(ecran.screen, ecran.l, ecran.L, folder, filename);

            //Root détruit son écran
            destroyScreen(&ecran);

        }
        nbFrame++;
        //Update des positions des objets dans la scène
        updatePosition(&animator, dt);
    }
    
    destroyScene(&scene);
    destroyRenderer(&renderer);

    //Le root delete le renderer global
    if(my_rank == 0)
    {
        destroyRenderer(&rendererGlobal);
    }

    deleteReduceOp();
    freeMPIStruct();
    MPI_Finalize();
    return 0;
}