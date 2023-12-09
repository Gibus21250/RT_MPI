#include "models/ray.h"
#include "math/vector.h"
#include <math.h>

/**
 * Renvoie le t de l'intersection, -1 sinon
*/
double intersectSphere(Ray *r, Sphere *sphere)
{
    Vector oc = {
        r->o.x - sphere->center.x,
        r->o.y - sphere->center.y,
        r->o.z - sphere->center.z
    };

    double a = dot(&r->v, &r->v);
    double b = 2.0 * dot(&oc, &r->v);
    double c = dot(&oc, &oc) - sphere->r * sphere->r;

    double delta = b*b - 4*a*c;

    if(delta >=0)
        return (-b - sqrt(delta) / 2.0 * a);
    else
        return -1.0;
}