#include <stdio.h>
#include <stdlib.h>

#include "screen/screen.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "scene/light.h"

#include "utils/color.h"
#include "exporter/saver.h"

int main(int argc, char const **argv)
{
    //64 36 -> 16/9
    Screen ecran = {NULL, 480, 270};
    Scene scene;
    Color ambiant = {.2, .2, .2};
    scene.ambiant = ambiant;

    Camera perspCam = {
        .type = PERSPECTIVE, 
        .projection = {
            .perspective = {
                .position = {0, 0, -1},
                .lookAt = {0, 0, 1},
                .up = {0, 1, 0},
                .distance = 1/(16/9.)
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

    PointLight light = {{0, 1, 0}, 0.5, {1, 1, 1}};
    PointLight light2 = {{0, 0, 0}, 0.2, {1, 0, 0}};
    PointLight light3 = {{0, -1, 0}, 0.2, {0, 1, 0}};

    initScreen(&ecran);
    initScene(&scene);

    Sphere test = {{1, 1, 1}, 0.5, {0, 0, 1}};
    Sphere test2 = {{1, 1, 1}, 0.2, {-0.5, 0.5, 1}};
    Sphere test3 = {{1, 1, 1}, 0.2, {0.5, 0.5, 1}};

    addModel(&scene, &test);
    addModel(&scene, &test2);
    addModel(&scene, &test3);

    addLight(&scene, &light);
    addLight(&scene, &light2);
    addLight(&scene, &light3);

    draw(&ecran, &perspCam, &scene);
    //showResult(&ecran);
    printf("finit\n");
    savePPMP6(ecran.screen, ecran.l, ecran.L, "premier6.ppm");
    //savePPMP3(ecran.screen, ecran.l, ecran.L, "premier3.ppm");

    destroyScreen(&ecran);
    destroyScene(&scene);
    return 0;
}
