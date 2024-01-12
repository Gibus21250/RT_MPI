#include "screen/screen.h"

#include "utils/color.h"
#include "utils/vector.h"
#include "utils/math.h"

#include "renderer/renderer.h"
#include "scene/scene.h"
#include "models/ray.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

void showResult(Screen *screen)
{
    for (unsigned int i = 0; i < screen->l; ++i)
    {
        for (unsigned int j = 0; j < screen->L; ++j)
        {
            Color tmp = screen->screen[i * screen->L + j];

            printf("(%f %f %f) ", tmp.r, tmp.g, tmp.b);
        }
    }
}

void initScreen(Screen *screen)
{
    screen->screen = (Color *)malloc(screen->l * screen->L * sizeof(Color));
}

void clearScreen(Screen *screen)
{
    memset(screen->screen, 0, screen->l * screen->L * sizeof(Color));
}

void destroyScreen(Screen *screen)
{
    free(screen->screen);
}
