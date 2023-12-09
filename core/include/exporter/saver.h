#ifndef SAVER_H
#define SAVER_H

#include "color.h"

/**
 * Sauvegarde dans un fichier name, une image de longueur l, et largeur L
*/
char save(Color *image, unsigned int l, unsigned int L, char *name);

#endif