#include "utils/vector.h"

#include <math.h>
#include <stdlib.h>

#ifdef __AVX2__
	#include <immintrin.h>
	#include <avx2intrin.h>
#endif

#include "utils/math.h"


Vector randomRayHemisphere(Vector *normale)
{
    // Génération de coordonnées sphériques aléatoires
    double theta = 2.0 * M_PI * ((double) rand() / RAND_MAX); // Angle azimutal
    double phi = acos(1.0 - 2.0 * ((double) rand() / RAND_MAX)); // Angle polaire

    // Conversion des coordonnées sphériques en coordonnées cartésiennes
    double x = sin(phi) * cos(theta);
    double y = sin(phi) * sin(theta);
    double z = cos(phi);

    // Générer une base orthogonale à partir de la normale
	Vector tangent = cross(&(Vector){0, 1, 0}, normale);
    normalize(&tangent);
    Vector bitangent = cross(normale, &tangent);
	normalize(&bitangent);

    // Calculer la direction du rayon dans la base locale
	Vector tx = mul(&tangent, x);
	Vector ty = mul(&bitangent, y);
	Vector nz = mul(normale, z);
	
    Vector randomDirection = add(&tx, &ty);
	randomDirection = add(&randomDirection, &nz);

    normalize(&randomDirection);

    return randomDirection;
}

Vector randomUnit()
{
	Vector res = {
		((double) rand() / RAND_MAX),
		((double) rand() / RAND_MAX),
		((double) rand() / RAND_MAX)
	};

	return res;
}

Vector randomFrom(double deb, double fin)
{
	double tx = ((double) rand() / RAND_MAX);
	double ty = ((double) rand() / RAND_MAX);
	double tz = ((double) rand() / RAND_MAX);

	Vector res = {
		(1 - tx) * deb + tx * fin,
		(1 - ty) * deb + ty * fin,
		(1 - tz) * deb + tz * fin
	};

	return res;
}

double dot(Vector* u, Vector* v)
{
	#ifdef __AVX2__
		
		__m256d u_vec = _mm256_loadu_pd((double*) &u->x);
        __m256d v_vec = _mm256_loadu_pd((double*) &v->x);

        // Multiplier les composants des vecteurs
        __m256d result = _mm256_mul_pd(u_vec, v_vec);

        // Addition horizontale des éléments
        result = _mm256_add_pd(result, _mm256_permute2f128_pd(result, result, 0x1));
        result = _mm256_hadd_pd(result, result);
        
        // Extraire le résultat
        double dot_product = _mm256_cvtsd_f64(result);

        return dot_product;

	#else
		return u->x * v->x + u->y * v->y + u->z * v->z;
	#endif
	
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

Vector reflect(Vector *incidence, Vector *normal)
{
	double d = dot(incidence, normal) * 2.0;
	Vector snormal = mul(normal, d);
	return sub(incidence, &snormal);
}