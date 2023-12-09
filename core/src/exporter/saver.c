#include "exporter/saver.h"
#include <stdio.h>


char savePPMP6(Color *image, unsigned int l, unsigned int L, char *name)
{
    FILE *pfile;

    pfile = fopen(name, "wb");

    if (pfile)
    {
        fprintf(pfile, "P6\n%u %u\n255\n", l, L);

        //pour chaque colonne
        for (unsigned int i = 0; i < l; ++i)
        {
            //Pour chaque ligne
            for (unsigned int j = 0; j < L; ++j)
            {
                //On récupère en valuer [0;255] la couleur
                unsigned char r = (unsigned char)(image[i * l + j].r * 255.0f);
                unsigned char g = (unsigned char)(image[i * l + j].g * 255.0f);
                unsigned char b = (unsigned char)(image[i * l + j].b * 255.0f);

                //Ecrire en binaire les valeur des couleurs, pour gagner en place!
                fwrite(&r, sizeof(unsigned char), 1, pfile);
                fwrite(&g, sizeof(unsigned char), 1, pfile);
                fwrite(&b, sizeof(unsigned char), 1, pfile);
            }
        }

        fclose(pfile);
        return 1;
    }
    else
        return 0;
}

char savePPMP3(Color *image, unsigned int l, unsigned int L, char *name)
{
    FILE *pfile;

    pfile = fopen(name, "wb");

    if (pfile)
    {
        fprintf(pfile, "P3\n%u %u\n255\n", l, L);

        //pour chaque colonne
        for (unsigned int i = 0; i < l; ++i)
        {
            //pour chaque ligne
            for (unsigned int j = 0; j < L; ++j)
            {
                //On récupère en valuer [0;255] la couleur
                unsigned char r = (unsigned char)(image[i * l + j].r * 255.0f);
                unsigned char g = (unsigned char)(image[i * l + j].g * 255.0f);
                unsigned char b = (unsigned char)(image[i * l + j].b * 255.0f);

                fprintf(pfile, "%u %u %u ", (unsigned int)r, (unsigned int)g, (unsigned int)b);
            }

            fprintf(pfile, "\n");
        }

        fclose(pfile);
        return 1;
    }
    else
        return 0;
}