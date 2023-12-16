#include "models/ray.h"
#include "utils/vector.h"

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

    if(delta < 0)
    {
        return -1.0;
    }
    else //Deux solutions, ou == 0 -> racine double
    {
        double tp = (-b + sqrt(delta)) / (2.0 * a);
        double tm = (-b - sqrt(delta)) / (2.0 * a);

        tp = fmax(tp, 0.0);
        tm = fmax(tm, 0.0);

        return fmin(tp, tm);
    }
}

Ray move(Ray *r, double t)
{
    Ray res = {
        {
            r->o.x + t * r->v.x,
            r->o.y + t * r->v.y,
            r->o.z + t * r->v.z
        },
        {r->v.x, r->v.y, r->v.z}
    };
    return res;
}