#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
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

#endif // !VECTOR_H
