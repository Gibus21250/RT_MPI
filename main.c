#include <stdio.h>
#include <stdlib.h>

#include "screen/screen.h"
#include "scene/scene.h"
#include "scene/camera.h"

#include "exporter/color.h"
#include "exporter/saver.h"

int main(int argc, char const **argv)
{
    Screen ecran = {NULL, 4096, 4096};
    Scene scene;

    Camera perspCam = {
        .type = PERSPECTIVE, 
        .projection = {
            .perspective = {
                .position = {5, 5, -5},
                .lookAt = {0, 0, 0},
                .up = {0, 1, 0}
            }
        }
    };

    Camera cameraPar = {
        .type = PARALLEL,
        .projection = {
            .parallel = {
                .direction = {0, 0, 1},
                .up = {0, 1, 0}
            }
        }
    };

    initScreen(&ecran);
    initScene(&scene);

    Sphere test = {{0.4, 0.5, 0.8}, 0.5, {0, 0.1, 0.9}}; //bleu
    Sphere test2 = {{0.8, 0.1, 0.4}, 0.2, {-0.5, 0.5, 1}}; //rose
    Sphere test3 = {{0.1, 0.1, 0.4}, 0.2, {0.5, 0.5, 1}}; //rose

    addModel(&scene, &test);
    addModel(&scene, &test2);
    addModel(&scene, &test3);

    draw(&ecran, &perspCam, &scene);
    //showResult(&ecran);
    
    savePPMP6(ecran.screen, ecran.l, ecran.L, "premier6.ppm");
    //savePPMP3(ecran.screen, ecran.l, ecran.L, "premier3.ppm");

    destroyScreen(&ecran);
    destroyScene(&scene);
    return 0;
}
