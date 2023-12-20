#include "struct_init.h"

#include <stddef.h>
#include <stdlib.h>

#include "utils/color.h"
#include "utils/vector.h"
#include "scene/camera.h"

void initMPIStruct()
{

    //Doc: //https://www.mpich.org/static/docs/v3.1/www3/MPI_Type_create_struct.html

    //COLOR
    MPI_Datatype_contiguous(3, MPI_FLOAT, &MPI_COLOR_STRUCT)
    MPI_Type_commit(&MPI_COLOR_STRUCT);

    //VECTOR
    MPI_Datatype_contiguous(3, MPI_DOUBLE, &MPI_COLOR_STRUCT)
    MPI_Type_commit(&MPI_VECTOR_STRUCT);

    //CAMERA
    int camerablocklengths[5] = {1, 1, 1, 1, 1}; // number of elements in each block
    MPI_Datatype cameratypes[5] = {MPI_VECTOR_STRUCT, MPI_VECTOR_STRUCT, MPI_VECTOR_STRUCT, MPI_DOUBLE, MPI_DOUBLE};
    MPI_Aint cameraoffsets[5];

    cameraoffsets[0] = offsetof(Camera, position);
    cameraoffsets[1] = offsetof(Camera, lookAt);
    cameraoffsets[2] = offsetof(Camera, up);
    cameraoffsets[4] = offsetof(Camera, distance);
    cameraoffsets[5] = offsetof(Camera, fov);

    MPI_Type_create_struct(5, camerablocklengths, cameraoffsets, cameratypes, &MPI_CAMERA_STRUCT);
    MPI_Type_commit(&MPI_CAMERA_STRUCT);

    //SCREEN
    int camerablocklengths[6] = {1, 1, 1, 1, 1, 1}; // number of elements in each block
    MPI_Datatype cameratypes[6] = {MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED, MPI_AINT, MPI_AINT};
    MPI_Aint cameraoffsets[6];

    cameraoffsets[0] = offsetof(Camera, position);
    cameraoffsets[1] = offsetof(Camera, lookAt);
    cameraoffsets[2] = offsetof(Camera, up);
    cameraoffsets[4] = offsetof(Camera, distance);
    cameraoffsets[5] = offsetof(Camera, fov);

    MPI_Type_create_struct(5, camerablocklengths, cameraoffsets, cameratypes, &MPI_CAMERA_STRUCT);
    MPI_Type_commit(&MPI_CAMERA_STRUCT);
}

void commitAllMPIStruct()
{
    
}