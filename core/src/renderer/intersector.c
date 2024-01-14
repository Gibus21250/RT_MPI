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

double intersectTore(Ray *r, Tore *tore, Vector *listePoint)
{
    // Notre base
    Vector i = {1, 0, 0}, j = {0, 1, 0}, k = {0, 0, 1};

    double rmaj = 3 * tore->r;

    // Changement de base par rapport à l'orientaiton normale du tore
    Vector kt = tore->normal;

    Vector it = cross(&i, &kt);

    Vector jt = cross(&kt, &it);

    // Projections des valeurs dans la nouvelle base
    // Position du tore dans la nouvelle base
    Vector rt = {
        dot(&tore->center, &it),
        dot(&tore->center, &jt),
        dot(&tore->center, &kt)};

    // Position du rayon dans la nouvelle base
    Vector rot = {
        dot(&r->o, &it),
        dot(&r->o, &jt),
        dot(&r->o, &kt)};

    // Nouveau vecteur directeur du rayon, dans la nouvelle base
    Vector rvt = {
        dot(&r->v, &it),
        dot(&r->v, &jt),
        dot(&r->v, &kt)};

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

    // double A = delta1 * delta1; //A vaut 1 ici
    double B = 2.0 * delta1 * eta2;
    double C = 2.0 * delta1 * beta3 + eta2 * eta2 - 4 * rmaj * rmaj * gamma1;
    double D = 2 * eta2 * beta3 - 4 * rmaj * rmaj * mu2;
    double E = beta3 * beta3 - 4 * rmaj * rmaj * alpha3;

    double solutions[4];

    // solveQuartic(coeff, solutions);
    int nb_reel = SolveP4(solutions, B, C, D, E);

    if (nb_reel == 0)
        return -1;
    else
    {
        // On récupère les points dans la base transformée
        for (int i = 0; i < nb_reel; ++i)
        {
            Vector tmp = {
                rot.x + solutions[i] * rvt.x,
                rot.y + solutions[i] * rvt.y,
                rot.z + solutions[i] * rvt.z};

            listePoint[i] = tmp;
        }

        double baseMatrix[3][3] = {
            {it.x, jt.x, kt.x},
            {it.y, jt.y, kt.y},
            {it.z, jt.z, kt.z}};

        // On transformes les points dans la base originale
        for (int i = 0; i < nb_reel; ++i)
        {
            // Vecteur M' dans la base d'origine
            listePoint[i].x = baseMatrix[0][0] * listePoint[i].x + baseMatrix[0][1] * listePoint[i].y + baseMatrix[0][2] * listePoint[i].z;
            listePoint[i].y = baseMatrix[1][0] * listePoint[i].x + baseMatrix[1][1] * listePoint[i].y + baseMatrix[1][2] * listePoint[i].z;
            listePoint[i].z = baseMatrix[2][0] * listePoint[i].x + baseMatrix[2][1] * listePoint[i].y + baseMatrix[2][2] * listePoint[i].z;

            // Ajouter les coordonnées du centre du tore pour obtenir la position absolue
            listePoint[i].x += tore->center.x;
            listePoint[i].y += tore->center.y;
            listePoint[i].z += tore->center.z;
        }

        return nb_reel;
    }
}

double intersectPlane(Ray *r, Plane *plane)
{
    double d = plane->normal.x * plane->center.x + plane->normal.y * plane->center.y + plane->normal.z * plane->center.z;

    double denominator = plane->normal.x * r->v.x + plane->normal.y * r->v.y + plane->normal.z * r->v.z;

    double t = -(plane->normal.x * r->o.x + plane->normal.y * r->o.y + plane->normal.z * r->o.z + d) / denominator;

    return t;
}

double intersectRectangle(Ray *r, Rectangle *rec)
{
    double d = rec->normal.x * rec->center.x + rec->normal.y * rec->center.y + rec->normal.z * rec->center.z;

    double denominator = rec->normal.x * r->v.x + rec->normal.y * r->v.y + rec->normal.z * r->v.z;

    double t = -(rec->normal.x * r->o.x + rec->normal.y * r->o.y + rec->normal.z * r->o.z + d) / denominator;

    //Le rayon intersecte le plan du rectangle
    if(t > 0)
    {
        double halfL = rec->l / 2.0;
        double halfW = rec->L / 2.0;

        Vector hitp = move(r, t).o;
        Vector ch = sub(&hitp, &rec->center);
        double dist2 = dot(&ch, &ch);

        //Verifier ici que le point appartient au rectangle
        if(dist2 <= rec->l * rec->l)
        {
            return t;
        }
        else
            return -1;
    }
    return -1;
}