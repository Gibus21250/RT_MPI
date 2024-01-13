#include "models/ray.h"

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