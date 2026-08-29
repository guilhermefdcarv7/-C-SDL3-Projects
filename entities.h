#ifndef ENTITIES_H
#define ENTITIES_H

#include "structs.h"

void initWorld(World *world);
void addEnemy(World *world, float x, float y);
void addAlie(World *world, float x, float y);
void shootProjectile(World *world, float origin_x, float origin_y, float target_x, float target_y);
void updatePhysics(World *world);
int detectCircleCollision(float x1, float y1, float radius1, float x2, float y2, float radius2);

#endif