#ifndef ENGINE_H
#define ENGINE_H

#include "structs.h"

int initGraphics(App *app, int width, int height);
void closeGraphics(App *app);
int detectMouseInput(float mouse_x, float mouse_y, float box_x, float box_y, float box_w, float box_h);

#endif