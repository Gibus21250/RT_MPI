#include <stdio.h>
#include <time.h>

#include "screen/screen.h"

#include "scene/camera.h"
#include "scene/scene.h"

#include "models/material.h"
#include "models/sphere.h"

//MPI
#include <mpi.h>

#include "struct_types/struct_type_all.h"
#include "struct_init.h"


int main(int argc, char const *argv[])
{
    if(argc == 1)
    {   
        printf("Veuillez reseigner nombre de sample max par images!\n");
        return 0;
    }

    Screen ecran = {
        .l = 720,
        .L = 480,
        .nbSample = 0,
        .maxSample = atoi(argv[1])          //On définie le nombre de sample par image
    };

    Scene scene = {
        .ambiant = {0.05, 0.05, 0.05},
        .sky = {0, 0.4, 0.7},
        .maxBounces = 64
    };

    //bleu clairs {0, 0.5, 0.8}

    Camera camera = {
        .position = {2, 2, 2},
        .lookAt = {0, 0, 0},
        .up = {0, 1, 0},
        .distance = 1,
        .fov = 90
    };

    //Process info
    int nbProcess, my_rank;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nbProcess);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    initMPIStruct();

    //On diffuse du 0 vers tous les autres processus
    MPI_Bcast(&ecran, 1, MPI_SCREEN_STRUCT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&scene, 1, MPI_SCENE_STRUCT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&camera, 1, MPI_CAMERA_STRUCT, 0, MPI_COMM_WORLD);

    //Chaque process initializera son ecran, et sa scènes
    initScreen(&ecran);
    initScene(&scene);

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
        .roughness = 0.1,
        .specular = {0, 0, 0},
        .shininess = 1
        };

    MPI_Bcast(&blanc, 1, MPI_MATERIAL_STRUCT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&magenta, 1, MPI_MATERIAL_STRUCT, 0, MPI_COMM_WORLD);
    
    unsigned int imatblanc = addMaterial(&scene, &blanc);
    unsigned int imatmagenta = addMaterial(&scene, &magenta);

    // On initialise les éléments de la scène
    Sphere s1 = {{0, 1, 0}, 1, imatblanc};
    Sphere sol = {{0, -10.5, 0}, 10, imatmagenta};

    MPI_Bcast(&s1, 1, MPI_SPHERE_STRUCT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&sol, 1, MPI_SPHERE_STRUCT, 0, MPI_COMM_WORLD);

    addModel(&scene, &s1);
    addModel(&scene, &sol);

    while(ecran.nbSample < ecran.maxSample)
    {
        clock_t start, end;
        double cpu_time_used;
        start = clock();
        draw(&ecran, &camera, &scene);
        end = clock();
        // Calculez le temps passé en secondes
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        // Affichez le temps
        printf("Temps écoulé : %f secondes\n", cpu_time_used);
    }

    destroyScreen(&ecran);
    destroyScene(&scene);

    freeMPIStruct();
    MPI_Finalize();
    return 0;
}

/*
  

*/
