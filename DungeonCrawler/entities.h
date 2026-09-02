#ifndef ENTITIES_H
#define ENTITIES_H

#include <SDL3/SDL.h>
#include "structs.h"

void initWorld(World *world);
void spawnEnemy(World *world, float x, float y);
void spawnBoss(World *world, float x, float y);
void shootProjectile(World *world, float origin_x, float origin_y, float target_x, float target_y, int speed, int damage, int is_enemy_bullet, int is_boss_bullet);
void updateHeroMovement(World *world, const bool *keyboard_state);
void updateEnemies(World *world);                                
void updateProjectiles(World *world);                            
void checkCollisions(World *world);                              
void cleanDeadEntities(World *world);                   
void updatePhysics(World *world, const bool *keyboard_state);

#endif