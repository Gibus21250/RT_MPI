#include "animator/animator.h"

#include "scene/camera.h"
#include "models/sphere.h"

#define STEP 5


void initAnimator(Animator *anim)
{
    anim->elements = (AnimatedElement*) malloc(STEP * sizeof(AnimatedElement));
    anim->nb = 0;
    anim->nbMax = STEP;
}

void destroyAnimator(Animator *anim)
{
    free(anim->elements);
}

void addMovableElement(Animator *anim, ModelType type, void *p, Vector *v)
{
    //Taille insuffisante, on agrandis le tableau
    if(anim->nb == anim->nbMax)
    {
        anim->nbMax += STEP;
        anim->elements = realloc(anim->elements, anim->nbMax * sizeof(AnimatedElement));
    }
    AnimatedElement toAdd = {
        type,
        p,
        *v
    };

    //On ajoute l'élément
    anim->elements[anim->nb] = toAdd;
    anim->nb++;
}

void updatePosition(Animator *anim, double dt)
{
    for (unsigned int i = 0; i < anim->nb; i++)
    {
        printf("type %d\n", anim->elements[i].type);
        switch (anim->elements[i].type)
        {
        case CAMERA:
            Camera *cam = (Camera*) &(anim->elements[i].p);
            Vector pc = mul(&anim->elements[i].vitesse, dt);
            cam->position = add(&cam->position, &pc);
            break;
        case SPHERE:
            //printf("Pointer animator: %p\n", anim->elements[i].p);
            Sphere *el = (Sphere*) (anim->elements[i].p);
            Vector ps = mul(&anim->elements[i].vitesse, dt);
            el->center = add(&el->center, &ps);
        default:
            break;
        }
    }
}