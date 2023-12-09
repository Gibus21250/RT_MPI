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
		u->x * v->z - u->z * v->y,
		u->z * v->x - u->x * v->z, //J'ai échangé pour éviter de faire un -
		u->x * v->y - u->y * v->x
	};

	return res;
}
