#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector {
	double x, y, z;
} Vector;

Vector randomRayHemisphere(Vector *normale);

Vector randomUnit();

Vector randomFrom(double deb, double fin);
/**
 * Renvoie le produit scalair de deux vecteurs.
 */
double dot(Vector *u, Vector *v);

/**
 * Normalise le vecteur en parametre.
 */
void normalize(Vector *u);

/**
 * Renvoie le vecteur resultant le produit vectoriel des deux vecteurs.
 */
Vector cross(Vector *u, Vector *v);

Vector sub(Vector *v1, Vector *v2);

Vector add(Vector *v1, Vector *v2);

Vector mul(Vector *v1, double scal);

Vector reflect(Vector *incidence, Vector *normal);

#endif // !VECTOR_H
