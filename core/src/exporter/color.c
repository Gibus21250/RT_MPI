#include "exporter/color.h"

#include <math.h>

void clampColor(Color *c)
{
    c->r = fmin(1.0, fmax(0.0, c->r));
    c->g = fmin(1.0, fmax(0.0, c->g));
    c->b = fmin(1.0, fmax(0.0, c->b));
}

Color addColor(Color *c1, Color *c2)
{
    Color res = {
        c1->r + c2->r,
        c1->g + c2->g,
        c1->b + c2->b
    };

    return res;
}

Color multiplyColord(Color *c, double i)
{
    Color res = {
        c->r * i,
        c->g * i,
        c->b * i
    };

    return res;
}
Color multiplyColorc(Color *c1, Color *c2)
{
    Color res = {
        c1->r * c2->r,
        c1->g * c2->g,
        c1->b * c2->b
    };

    return res;
}