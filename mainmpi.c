#include <stdio.h>
#include <time.h>

#include "screen/screen.h"

#include "scene/scene.h"
#include "scene/camera.h"
#include "scene/light.h"

#include "renderer/renderer.h"

#include "models/material.h"

#include "animator/animator.h"

//MPI
#include <mpi.h>

#include "struct_types/struct_type_all.h"
#include "struct_init.h"
#include "reduce_utils.h"


int main(int argc, char const *argv[])
{
    if(argc == 1)
    {   
        printf("Veuillez renseigner nombre de sample max par images!\n");
        return 0;
    }

    //Process info
    int nbProcess, my_rank, nbTotalSample;;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nbProcess);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    srand(my_rank);

    nbTotalSample = atoi(argv[1]);
    unsigned nbSamplePerProcess =  nbTotalSample / nbProcess;

    if(my_rank == 0)
    {
        printf("Rendu de %d samples: %u samples pour %d process\n", nbTotalSample, nbSamplePerProcess, nbProcess);
    }
    
    Renderer renderer = {
        .l = 1920,
        .L = 1080,
        .maxSample = nbSamplePerProcess,
        .currentSample = 0,
        .maxBounces = 64
    };

    Renderer rendererGlobal = {
        .l = renderer.l,
        .L = renderer.L,
        .maxSample = nbTotalSample,
        .currentSample = nbTotalSample,
        .maxBounces = 64
    };

    Scene scene = {
        .sky = {0, 0.4, 0.7}}; // bleu clair {0, 0.5, 0.8}

    Camera camera = {
        .position = {2, 2, 2},
        .lookAt = {0, 0, 0},
        .up = {0, 1, 0},
        .distance = 1,
        .fov = 90};

    Animator animator;
    
    initMPIStruct();
    initReduceOp();

    //Chaque process initializera son ecran, et sa scènes, son renderer
    initScene(&scene);
    initRenderer(&renderer);
    initAnimator(&animator);

    if(my_rank == 0)
    {
        initRenderer(&rendererGlobal);
    }

    // On commence par initialiser les materiaux:
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
        .albedo = {0, 0, 1},
        .roughness = 0,
        .specular = {1, 1, 1},
        .shininess = 50
    };

    Material blanc = {
        .type = DIFFUSE | EMISSIVE,
        .albedo = {1, 1, 1},
        .roughness = 1,
        .specular = {0, 0, 0},
        .shininess = 0.5,
        .emissionColor = {1, 1, 1},
        .emissionPower = 1
    };

    Material magenta = {
        .type = DIFFUSE,
        .albedo = {1, 0, 1},
        .roughness = 0.5,
        .specular = {0, 0, 0},
        .shininess = 1
    };

    Material matsoleil = {
        .type = DIFFUSE | EMISSIVE,
        .albedo = {0.8, 0.5, 0.2},
        .roughness = 1,
        .specular = {0, 0, 0},
        .shininess = 1,
        .emissionColor = {0.8, 0.5, 0.2},
        .emissionPower = 1
    };
    
    // On ajoute les matériaux à la scène
    unsigned int imatx = addMaterial(&scene, &matx);
    unsigned int imaty = addMaterial(&scene, &maty);
    unsigned int imatz = addMaterial(&scene, &matz);
    unsigned int imatblanc = addMaterial(&scene, &blanc);
    unsigned int imatmagenta = addMaterial(&scene, &magenta);
    unsigned int imatsoleil = addMaterial(&scene, &matsoleil);

    // On initialise les éléments de la scène
    Sphere x = {{1, 0.2, 0}, 0.2, imatx};
    Sphere y = {{0, 1.2, 0}, 0.2, imaty};
    Sphere z = {{0, 0.2, 1}, 0.2, imatz};
    Sphere centre = {{0, 0.2, 0}, 0.2, imatblanc};

    Sphere sol = {{0, -10, 0}, 10, imatmagenta};
    Sphere soleil = {{-20, 5, -20}, 20, imatsoleil};

    // On ajoute les éléments dans la scène
    addModel(&scene, &x);
    addModel(&scene, &y);
    addModel(&scene, &z);
    addModel(&scene, &centre);

    addModel(&scene, &sol);
    addModel(&scene, &soleil);

    while(renderer.currentSample < renderer.maxSample)
    {
        //Cette fonction additionne une couleur de ecran.accumulator pour chaque appels
        render(&renderer, &scene, &camera);
    }

    //On reduce, dans l'accumulator que le root a créé
    MPI_Reduce(renderer.accumulator, rendererGlobal.accumulator, renderer.L * renderer.l, MPI_COLOR_STRUCT, ACCUMULATOR_REDUCE_OP, 0, MPI_COMM_WORLD);
    
    //On rassemble le résultat de tous les process, et on sauvegarde
    if(my_rank == 0)
    {
        time_t t;
        struct tm *tm_info;

        time(&t);
        tm_info = localtime(&t);

        char date_str[50];
        strftime(date_str, sizeof(date_str), "%m-%d-%H%M%S", tm_info);

        char result_str[60];
        sprintf(result_str, "%s-%d.ppm", date_str, rendererGlobal.maxSample);

        //Le root initialise un écran
        Screen ecran = {
            .l = renderer.l,
            .L = renderer.L
        };

        initScreen(&ecran);

        //Il remplie son écran avec le résultat de l'accumulation de sample
        updateResult(&rendererGlobal, &ecran);

        //On sauvegarde
        savePPMP6(ecran.screen, ecran.l, ecran.L, result_str);

        //Root détruit son écran
        destroyScreen(&ecran);
        destroyRenderer(&rendererGlobal);

    }
    
    destroyScene(&scene);
    destroyRenderer(&renderer);

    deleteReduceOp();
    freeMPIStruct();
    MPI_Finalize();
    return 0;
}