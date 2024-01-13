#include "renderer/intersector.h"

#include "utils/solvers.h"

#include <float.h>

double intersectSphere(Ray *r, Sphere *sphere)
{
    Vector oc = {
        r->o.x - sphere->center.x,
        r->o.y - sphere->center.y,
        r->o.z - sphere->center.z};

    double a = dot(&r->v, &r->v);
    double b = 2.0 * dot(&oc, &r->v);
    double c = dot(&oc, &oc) - sphere->r * sphere->r;

    double delta = b * b - 4 * a * c;

    if (delta < 0)
    {
        return -1.0;
    }
    else // Deux solutions, ou == 0 -> racine double
    {
        double tp = (-b + sqrt(delta)) / (2.0 * a);
        double tm = (-b - sqrt(delta)) / (2.0 * a);

        tp = fmax(tp, 0.0);
        tm = fmax(tm, 0.0);

        return fmin(tp, tm);
    }
}

double intersectTore(Ray *r, Tore *tore, Vector *normal)
{
    //Notre base
    Vector i = {1, 0, 0}, j = {0, 1, 0}, k = {0, 0, 1};

    double rmaj = 3 * tore->r;

    //Changement de base par rapport à l'orientaiton normale du tore
    Vector kt = tore->normal;

    Vector it = cross(&i, &kt);

    Vector jt = cross(&kt, &it);

    //Projections des valeurs dans la nouvelle base
    //Position du tore dans la nouvelle base
    Vector rt = {
        dot(&tore->center, &it),
        dot(&tore->center, &jt),
        dot(&tore->center, &kt)
    };

    //Position du rayon dans la nouvelle base
    Vector rot = {
        dot(&r->o, &it),
        dot(&r->o, &jt),
        dot(&r->o, &kt)
    };

    //Nouveau vecteur directeur du rayon, dans la nouvelle base
    Vector rvt = {
        dot(&r->v, &it),
        dot(&r->v, &jt),
        dot(&r->v, &kt)
    };

    double alpha1 = rt.x * rt.x + rt.y * rt.y;
    double alpha2 = alpha1 + rot.x * rot.x + rot.y * rot.y;
    double alpha3 = alpha2 - 2.0 * (rot.x * rt.x + rot.y * rt.y);

    double beta1 = rmaj * rmaj - tore->r * tore->r + alpha1 + rt.z * rt.z;
    double beta2 = beta1 + dot(&rot, &rot);
    double beta3 = beta2 - 2.0 * dot(&rot, &rt);

    double gamma1 = rvt.x * rvt.x + rvt.y * rvt.y;

    double delta1 = 1.0;

    double mu1 = rot.x * rvt.x + rot.y * rvt.y;
    double mu2 = 2.0 * (mu1 - rvt.x * rt.x - rvt.y * rt.y);

    double eta1 = mu1 + rot.z * rvt.z;
    double eta2 = 2.0 * (eta1 - dot(&rvt, &rt));

    //double A = delta1 * delta1; //A vaut 1 ici
    double B = 2.0 * delta1 * eta2;
    double C = 2.0 * delta1 * beta3 + eta2 * eta2 - 4 * rmaj * rmaj * gamma1;
    double D = 2 * eta2 * beta3 - 4 * rmaj * rmaj * mu2;
    double E = beta3 * beta3 - 4 * rmaj * rmaj * alpha3;

    double solutions[4];

    //solveQuartic(coeff, solutions);
    int nb_reel = SolveP4(solutions, B, C, D, E);

    if(nb_reel == 0)
        return -1;
    else
    {
        double tMin = DBL_MAX;
        double mindist2 = DBL_MAX;

        //On récupère la plus petite racine
        for (int i = 0; i < nb_reel; ++i) {
            if(solutions[i] < tMin)
                tMin = solutions[i];
        }

        

        return tMin;
    }
}
