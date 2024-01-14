#ifndef INTERSECTOR_H
#define INTERSECTOR_H

#include "models/sphere.h"
#include "models/tore.h"
#include "models/plane.h"
#include "models/rectangle.h"

#include "models/ray.h"

double intersectSphere(Ray *r, Sphere *sphere);
double intersectTore(Ray *r, Tore *tore, Vector *listePoint);
double intersectPlane(Ray *r, Plane *plane);
double intersectRectangle(Ray *r, Rectangle *rec);

#endif