#ifndef TORE_H
#define TORE_H

#include "utils/vector.h"

typedef struct Tore {
    Vector center;
    Vector normal;
    double r;   //Rayon du cylindre du tore
    double R;   //Rayon du tore
    unsigned int materialIndice;
} Tore;

Vector calculateToreNormal(Vector *point, Tore *tore);


#endif