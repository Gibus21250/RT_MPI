#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector {
	double x, y, z;
} Vector;

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

#endif // !VECTOR_H
