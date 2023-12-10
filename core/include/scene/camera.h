#ifndef CAMERA_H
#define CAMERA_H

#include "math/vector.h"

typedef struct {

    enum {PERSPECTIVE, PARALLEL} type;

    union 
    {
        struct 
        {
            Vector position;
            Vector lookAt;
            Vector up;
            double distance;
        } perspective;

        struct 
        {
            Vector direction;
            Vector up;
        } parallel;
        
    } projection;
    
} Camera;

#endif