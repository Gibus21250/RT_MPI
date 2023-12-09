#include "math/vector.h"
#include <math.h>


double dot(Vector* u, Vector* v)
{
	return u->x * v->x + u->y * v->y + u->z * v->z;
}

void normalize(Vector* u)
{
	double d = sqrt(dot(u, u));
	u->x = u->x / d;
	u->y = u->y / d;
	u->z = u->z / d;
}

Vector cross(Vector *u, Vector *v)
{
	Vector res = {
		u->y * v->z - u->z * v->y,
		u->z * v->x - u->x * v->z, //J'ai échangé pour éviter de faire un -
		u->x * v->y - u->y * v->x
	};

	return res;
}

Vector sub(Vector *v1, Vector *v2)
{
	Vector res = {
		v1->x - v2->x,
		v1->y - v2->y,
		v1->z - v2->z
	};

	return res;
}

Vector add(Vector *v1, Vector *v2)
{
	Vector res = {
		v1->x + v2->x,
		v1->y + v2->y,
		v1->z + v2->z
	};

	return res;
}

Vector mul(Vector *v1, double scal)
{
	Vector res = {
		v1->x * scal,
		v1->y * scal,
		v1->z * scal
	};

	return res;
}