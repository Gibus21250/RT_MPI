#ifndef COLOR_H
#define COLOR_H

typedef struct Color {
    float r, g, b;
} Color;

void clampColor(Color *color, double min, double max);
Color addColor(Color *c1, Color *c2);
Color multiplyColord(Color *c, double i);
Color multiplyColorc(Color *c1, Color *c2);

#endif