#include "struct_init.h"

#include "struct_types/struct_type_all.h"

#include <stddef.h>
#include <stdlib.h>

#include "utils/color.h"
#include "utils/vector.h"
#include "models/material.h"
#include "screen/screen.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "models/sphere.h"

//Utils
MPI_Datatype MPI_COLOR_STRUCT;
MPI_Datatype MPI_VECTOR_STRUCT;

//Screen
MPI_Datatype MPI_SCREEN_STRUCT;

//Scene
MPI_Datatype MPI_CAMERA_STRUCT;
MPI_Datatype MPI_DYNARRAY_STRUCT;
MPI_Datatype MPI_SCENE_STRUCT;

//Models
MPI_Datatype MPI_MATERIAL_STRUCT;
MPI_Datatype MPI_SPHERE_STRUCT;

void initMPIStruct()
{
    //Doc: //https://www.mpich.org/static/docs/v3.1/www3/MPI_Type_create_struct.html
    //COLOR
    MPI_Type_contiguous(3, MPI_FLOAT, &MPI_COLOR_STRUCT);
    MPI_Type_commit(&MPI_COLOR_STRUCT);

    //VECTOR
    MPI_Type_contiguous(3, MPI_DOUBLE, &MPI_VECTOR_STRUCT);
    MPI_Type_commit(&MPI_VECTOR_STRUCT);

    //SCREEN
    int screenblocklengths[3] = {1, 1, 1}; // number of elements in each block
    MPI_Datatype screentypes[3] = {MPI_UNSIGNED, MPI_UNSIGNED, MPI_AINT};
    MPI_Aint screenoffsets[3];

    screenoffsets[0] = offsetof(Screen, l);
    screenoffsets[1] = offsetof(Screen, L);
    screenoffsets[2] = offsetof(Screen, screen);

    MPI_Type_create_struct(3, screenblocklengths, screenoffsets, screentypes, &MPI_SCREEN_STRUCT);
    MPI_Type_commit(&MPI_SCREEN_STRUCT);
    


    //CAMERA
    int camerablocklengths[5] = {1, 1, 1, 1, 1}; // number of elements in each block
    MPI_Datatype cameratypes[5] = {MPI_VECTOR_STRUCT, MPI_VECTOR_STRUCT, MPI_VECTOR_STRUCT, MPI_DOUBLE, MPI_DOUBLE};
    MPI_Aint cameraoffsets[5];

    cameraoffsets[0] = offsetof(Camera, position);
    cameraoffsets[1] = offsetof(Camera, lookAt);
    cameraoffsets[2] = offsetof(Camera, up);
    cameraoffsets[3] = offsetof(Camera, distance);
    cameraoffsets[4] = offsetof(Camera, fov);

    MPI_Type_create_struct(5, camerablocklengths, cameraoffsets, cameratypes, &MPI_CAMERA_STRUCT);
    MPI_Type_commit(&MPI_CAMERA_STRUCT);

    //DYNAMICARRAY
    int dynarrayblocklengths[3] = {1, 1, 1};
    MPI_Datatype dynarraytypes[3] = {MPI_AINT, MPI_UNSIGNED, MPI_UNSIGNED};
    MPI_Aint dynarrayoffsets[3];

    dynarrayoffsets[0] = offsetof(DynamicArray, tab);
    dynarrayoffsets[1] = offsetof(DynamicArray, nb);
    dynarrayoffsets[2] = offsetof(DynamicArray, max);

    MPI_Type_create_struct(3, dynarrayblocklengths, dynarrayoffsets, dynarraytypes, &MPI_DYNARRAY_STRUCT);
    MPI_Type_commit(&MPI_DYNARRAY_STRUCT);

    //SCENE
    int sceneblocklengths[4] = {1, 1, 1, 1};
    MPI_Datatype scenetypes[4] = {MPI_DYNARRAY_STRUCT, MPI_DYNARRAY_STRUCT, MPI_DYNARRAY_STRUCT, MPI_COLOR_STRUCT, MPI_DOUBLE};
    MPI_Aint sceneoffsets[4];

    sceneoffsets[0] = offsetof(Scene, spheres);
    sceneoffsets[1] = offsetof(Scene, tores);
    sceneoffsets[2] = offsetof(Scene, materials);
    sceneoffsets[3] = offsetof(Scene, sky);

    MPI_Type_create_struct(4, sceneblocklengths, sceneoffsets, scenetypes, &MPI_SCENE_STRUCT);
    MPI_Type_commit(&MPI_SCENE_STRUCT);

    //MATERIAL
    int nbMembre = 7;
    int materialblocklengths[7] = {1, 1, 1, 1, 1, 1, 1, 1, 1}; // number of elements in each block
    MPI_Datatype materialtypes[7] = {MPI_INT, MPI_COLOR_STRUCT, MPI_COLOR_STRUCT, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_COLOR_STRUCT};
    MPI_Aint materialoffsets[7];

    materialoffsets[0] = offsetof(Material, type);
    materialoffsets[1] = offsetof(Material, albedo);
    materialoffsets[2] = offsetof(Material, roughness);
    materialoffsets[3] = offsetof(Material, metalness);
    materialoffsets[4] = offsetof(Material, refractionFactor);
    materialoffsets[5] = offsetof(Material, emissionPower);
    materialoffsets[6] = offsetof(Material, emissionColor);

    MPI_Type_create_struct(7, materialblocklengths, materialoffsets, materialtypes, &MPI_MATERIAL_STRUCT);
    MPI_Type_commit(&MPI_MATERIAL_STRUCT);

    //SPHERE
    int sphereblocklengths[3] = {1, 1, 1};
    MPI_Datatype spheretypes[3] = {MPI_VECTOR_STRUCT, MPI_FLOAT, MPI_UNSIGNED};
    MPI_Aint sphereoffsets[3];

    sphereoffsets[0] = offsetof(Sphere, center);
    sphereoffsets[1] = offsetof(Sphere, r);
    sphereoffsets[2] = offsetof(Sphere, materialIndice);

    MPI_Type_create_struct(3, sphereblocklengths, sphereoffsets, spheretypes, &MPI_SPHERE_STRUCT);
    MPI_Type_commit(&MPI_SPHERE_STRUCT);
}

void freeMPIStruct()
{
    MPI_Type_free(&MPI_COLOR_STRUCT);
    MPI_Type_free(&MPI_VECTOR_STRUCT);

    MPI_Type_free(&MPI_SCREEN_STRUCT);

    MPI_Type_free(&MPI_CAMERA_STRUCT);
    MPI_Type_free(&MPI_DYNARRAY_STRUCT);
    MPI_Type_free(&MPI_SCENE_STRUCT);

    MPI_Type_free(&MPI_MATERIAL_STRUCT);
    MPI_Type_free(&MPI_SPHERE_STRUCT);
}