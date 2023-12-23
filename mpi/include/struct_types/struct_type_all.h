#ifndef STRUCT_TYPE_ALL_H
#define STRUCT_TYPE_ALL_H

#include <mpi.h>

    //Utils
    extern MPI_Datatype MPI_COLOR_STRUCT;
    extern MPI_Datatype MPI_VECTOR_STRUCT;

    //Screen
    extern MPI_Datatype MPI_SCREEN_STRUCT;

    //Scene
    extern MPI_Datatype MPI_CAMERA_STRUCT;
    extern MPI_Datatype MPI_DYNARRAY_STRUCT;
    extern MPI_Datatype MPI_SCENE_STRUCT;

    //Models
    extern MPI_Datatype MPI_MATERIAL_STRUCT;
    extern MPI_Datatype MPI_SPHERE_STRUCT;

#endif