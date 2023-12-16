#include "scene/camera.h"
#include "utils/vector.h"

void moveCamera(Camera *camera, double stepx, double stepy, double stepz)
{
    Vector dirLook = sub(&camera->lookAt, &camera->position);
    normalize(&dirLook);

    Vector localx = cross(&camera->up, &dirLook);
    localx = mul(&localx, stepx);

    Vector localy = camera->up;
    localy = mul(&localy, stepy);

    Vector localz = dirLook;
    localz = mul(&localz, stepz);


    Vector translation = add(&localx, &localy);
    translation = add(&translation, &localz);

    camera->position = add(&camera->position, &translation);
    camera->lookAt = add(&camera->lookAt, &translation);
}