#include "models/tore.h"

Vector calculateToreNormal(Vector *point, Tore *tore)
{
    Vector directionHit = sub(point, &tore->center);

    double scal = dot(&tore->normal, &directionHit);
    Vector tmp = mul(&tore->normal, scal);
    //Vproj
    Vector proj = sub(point, &tmp);

    normalize(&proj);

    Vector centerExte = mul(&proj, tore->R);

    Vector normale = sub(point, &centerExte);

    return normale;
}