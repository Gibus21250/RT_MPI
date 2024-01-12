#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "scene/scene.h"
#include "utils/vector.h"

/**
 * Associe un élément animable de la scène (Cf scene.h)
*/
typedef struct AnimatedElement {
    ModelType type;
    void *p; //Pointeur direct vers l'élément en mémoire
    Vector vitesse;
} AnimatedElement;

typedef struct Animator {
    AnimatedElement *elements;
    unsigned int nb;
    unsigned int nbMax;
} Animator;


void initAnimator(Animator *anim);
void destroyAnimator(Animator *anim);

void addMovableElement(Animator *anim, ModelType type, void *p, Vector *v);

void updatePosition(Animator *anim, double dt);

#endif