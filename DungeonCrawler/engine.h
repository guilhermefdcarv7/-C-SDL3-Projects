#ifndef ENGINE_H
#define ENGINE_H

int initGraphics(App *app, int width, int height);
void closeGraphics(App *app);
int detectMouseInput(float mouse_x, float mouse_y, float box_x, float box_y, float box_w, float box_h);
int detectRectCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

#endif