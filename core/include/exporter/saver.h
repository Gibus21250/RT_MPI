#ifndef SAVER_H
#define SAVER_H

#include "utils/color.h"

/**
 * Sauvegarde dans un fichier name, une image de longueur l, et largeur L
*/
char savePPMP6(Color *image, unsigned int l, unsigned int L, char *name);
char savePPMP3(Color *image, unsigned int l, unsigned int L, char *name);
#endif