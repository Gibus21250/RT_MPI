#include "utils/color.h"

#include <math.h>

void clampColor(Color *c, double min, double max)
{
    c->r = fmin(max, fmax(min, c->r));
    c->g = fmin(max, fmax(min, c->g));
    c->b = fmin(max, fmax(min, c->b));
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