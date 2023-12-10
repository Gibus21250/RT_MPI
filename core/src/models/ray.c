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
    else if(delta > 0)
    {
        double tp = (-b + sqrt(delta)) / (2.0 * a);
        double tm = (-b - sqrt(delta)) / (2.0 * a);
        if(tm > 0 && tp > 0)
        {
            return tm>tp?tp:tm;
        }
        else //Un des deux est négatif
        {
            double res = tm>tp?tm:tp;
            if(res > 0)
                return res;
            else
                return -1;
        }
    }
    else //delta == 0
    {
        double t = -b / (2.0 * a);
        if(t > 0)
            return t;
        else
            return -1;
    }
}

Ray move(Ray *r, double t)
{
    Ray res = {
        {
        r->o.x = r->o.x + t * r->v.x,
        r->o.y = r->o.y + t * r->v.y,
        r->o.z = r->o.z + t * r->v.z
        },
        {r->v.x, r->v.y, r->v.z}
    };
    return res;
}