#include <stdio.h>
#include <stdlib.h>

#include "screen/screen.h"
#include "scene/scene.h"
#include "scene/camera.h"

#include "exporter/color.h"
#include "exporter/saver.h"

int main(int argc, char const **argv)
{
    Screen ecran = {NULL, 1024, 1024};
    Scene scene;
    Camera camera = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};

    initScreen(&ecran);
    initScene(&scene);

    Sphere test = {{0.4, 0.5, 0.8}, 0.5, {0, 1, 1}};
    Sphere test2 = {{0.8, 0.1, 0.4}, 1, {0, 0, 3}};

    addModel(&scene, &test);
    addModel(&scene, &test2);

    draw(&ecran, &camera, &scene);
    //showResult(&ecran);
    
    savePPMP6(ecran.screen, ecran.l, ecran.L, "premier6.ppm");
    //savePPMP3(ecran.screen, ecran.l, ecran.L, "premier3.ppm");

    destroyScreen(&ecran);
    destroyScene(&scene);
    return 0;
}
