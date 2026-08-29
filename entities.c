#include <stdlib.h>
#include <math.h>
#include "entities.h"

void initWorld(World *world){
    world->enemy_list = NULL;
    world->alie_list = NULL;
    world->projectile_list = NULL;
    world->money = 500;
    world->base_lives = 0;
    world->level_timer_frames = 3600;
    world->spawn_timer_frames = 2;
    world->spawn_interval_frames = 120;
}

void addEnemy(World *world, float x, float y){
    Enemy *new_enemy = malloc(sizeof(Enemy));

    new_enemy->x = x;
    new_enemy->y = y;
    new_enemy->range_radius = 50;
    new_enemy->speed = 1;
    new_enemy->hp = 100;
    new_enemy->next = NULL;

    if(world->enemy_list == NULL){
        world->enemy_list = new_enemy;
    } else{
        Enemy *current_enemy = world->enemy_list;
        while(current_enemy->next != NULL){
            current_enemy = current_enemy->next;
        }
        current_enemy->next = new_enemy;
    }
}

void addAlie(World *world, float x, float y){
    Alie *new_alie = malloc(sizeof(Alie));

    new_alie->x = x;
    new_alie->y = y;
    new_alie->range_radius = 50;
    new_alie->damage = 100;
    new_alie->cooldown_frames = 180;
    new_alie->next = NULL;

    if(world->alie_list == NULL){
        world->alie_list = new_alie;
    } else{
        Alie *current_alie = world->alie_list;
        while(current_alie->next != NULL){
            current_alie = current_alie->next;
        }
        current_alie->next = new_alie;
    }
}

void shootProjectile(World *world, float origin_x, float origin_y, float target_x, float target_y){
    Projectile *new_projectile = malloc(sizeof(Projectile));

    float delta_x = target_x - origin_x;
    float delta_y = target_y - origin_y;
    float dist = sqrtf((delta_x * delta_x) + (delta_y * delta_y));

    new_projectile->x = origin_x;
    new_projectile->y = origin_y;
    new_projectile->dir_x = delta_x / dist;
    new_projectile->dir_y = delta_y / dist;
    new_projectile->speed = 10;
    new_projectile->damage = 100;
    new_projectile->next = NULL;

    if(world->projectile_list == NULL){
        world->projectile_list = new_projectile;
    } else{
        Projectile *current_projectile = world->projectile_list;
        while(current_projectile->next != NULL){
            current_projectile = current_projectile->next;
        }
        current_projectile->next = new_projectile;
    }
}

void updatePhysics(World *world){
    if(world->level_timer_frames > 0){
        world->level_timer_frames--;

        world->spawn_timer_frames--;
        if(world->spawn_timer_frames == 0){
            addEnemy(world, 600, 200);
            world->spawn_timer_frames = world->spawn_interval_frames;
        }
    } else{
        if(world->enemy_list == NULL){
            world->current_state = STATE_GAMEOVER;
        }
    }

    Enemy *current_enemy = world->enemy_list;

    while(current_enemy != NULL){
        if(current_enemy->x == 600 && current_enemy->y >= 200 && current_enemy->y <= 325){
            current_enemy->x = current_enemy->x;
            current_enemy->y = current_enemy->y + current_enemy->speed;
        }
        if(current_enemy->x <= 600 && current_enemy->x >= 25 && current_enemy->y == 325){
            current_enemy->x = current_enemy->x - current_enemy->speed;
            current_enemy->y = current_enemy->y;
        }
        if(current_enemy->x == 25 && current_enemy->y >= 325 && current_enemy->y <= 700){
            current_enemy->x = current_enemy->x;
            current_enemy->y = current_enemy->y + current_enemy->speed;
        }
        if(current_enemy->x >= 25 && current_enemy->x <= 1150 && current_enemy->y == 700){
            current_enemy->x = current_enemy->x + current_enemy->speed;
            current_enemy->y = current_enemy->y;
        }
        if(current_enemy->x == 1150 && current_enemy->y >= 475 && current_enemy->y <= 700){
            current_enemy->x = current_enemy->x;
            current_enemy->y = current_enemy->y - current_enemy->speed;
        }
        if(current_enemy->x >= 550 && current_enemy->x <= 1150 && current_enemy->y == 475){
            current_enemy->x = current_enemy->x - current_enemy->speed;
            current_enemy->y = current_enemy->y;
        }
        current_enemy = current_enemy->next;
    }

    Projectile *current_projectile = world->projectile_list;

    while(current_projectile != NULL){
        current_projectile->x += current_projectile->dir_x * current_projectile->speed;
        current_projectile->y += current_projectile->dir_y * current_projectile->speed;
        current_projectile = current_projectile->next;
    }

    Alie *current_alie = world->alie_list;

    while(current_alie != NULL){
        current_alie->cooldown_frames--;

        if(current_alie->cooldown_frames <= 0){
            Enemy *current_enemy = world->enemy_list;

            while(current_enemy != NULL){
                if(detectCircleCollision(current_alie->x, current_alie->y, current_alie->range_radius, current_enemy->x, current_enemy->y, current_enemy->range_radius)){
                    shootProjectile(world, current_alie->x, current_alie->y, current_enemy->x, current_enemy->y);
                    current_alie ->cooldown_frames = 120;
                    break;
                }

                current_enemy = current_enemy->next;
            }
        }

        current_alie = current_alie->next;
    }

    Enemy *previous_enemy = NULL;
    Enemy *currentEnemy = world->enemy_list;

    while(currentEnemy != NULL){
        Projectile *currentProjectile = world->projectile_list;

        while(currentProjectile != NULL){
            if(detectCircleCollision(currentProjectile->x, currentProjectile->y, 5, currentEnemy->x, currentEnemy->y, currentEnemy->range_radius)){
                currentEnemy->hp -= currentProjectile->damage;
            }

            currentProjectile = currentProjectile->next;
        }

        if(currentEnemy->hp <= 0){
            Enemy *temp = currentEnemy->next;

            if(previous_enemy == NULL){
                world->enemy_list = temp;
            } else{
                previous_enemy->next = temp;
            }
            free(currentEnemy);
            currentEnemy = temp;
        } else{
            previous_enemy = currentEnemy;
            currentEnemy = currentEnemy->next;
        }
    }
}

int detectCircleCollision(float x1, float y1, float radius1, float x2, float y2, float radius2){
    float dist = sqrtf(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));

    if(dist < radius1 + radius2){
        return 1;
    }

    return 0;
}