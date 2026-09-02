#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "entities.h"
#include "engine.h"

void spawnEnemy(World *world, float x, float y){
    Enemy *new_enemy = malloc(sizeof(Enemy));

    new_enemy->x = x;
    new_enemy->y = y;
    new_enemy->speed = 1;
    new_enemy->hp = 100;
    new_enemy->height = 100;
    new_enemy->width = 100;
    new_enemy->cooldown_frames = 120;
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

    world->enemies_remaining++;
}

void spawnBoss(World *world, float x, float y){
    world->boss.x = x;
    world->boss.y = y;
    world->boss.speed = 1;
    world->boss.hp = 1000;
    world->boss.height = 500;
    world->boss.width = 500;
    world->boss.cooldown_frames = 60;
    world->boss.is_active = 1;
}

void initWorld(World *world){
    world->enemy_list = NULL;
    world->projectile_list = NULL;
    world->current_level = 0;
    world->current_state = 1;
    world->enemies_remaining = 0;
    world->hero.x = 600;
    world->hero.y = 50;
    world->hero.damage = 50;
    world->hero.hp = 200;
    world->hero.max_hp = 100;
    world->hero.speed = 3;
    world->hero.cooldown_frames = 120;
    world->hero.height = 100;
    world->hero.width = 100;
    world->boss.is_active = 0;

    spawnEnemy(world, 200, 200);
    spawnEnemy(world, 800, 200);
    spawnEnemy(world, 500, 700);
}

void shootProjectile(World *world, float origin_x, float origin_y, float target_x, float target_y, int speed, int damage, int is_enemy_bullet, int is_boss_bullet){
    float delta_x = target_x - origin_x;
    float delta_y = target_y - origin_y;
    float dist = sqrt((delta_x * delta_x) + (delta_y * delta_y));

    Projectile *new_projectile = malloc(sizeof(Projectile));

    new_projectile->damage = 50;
    new_projectile->width = 50;
    new_projectile->height = 50;
    new_projectile->x = origin_x;
    new_projectile->y = origin_y;
    new_projectile->dir_x = delta_x / dist;
    new_projectile->dir_y = delta_y / dist;
    new_projectile->speed = 5;
    new_projectile->next = NULL;

    if(is_boss_bullet){
        new_projectile->is_boss_bullet = 1;
    } else{
        new_projectile->is_boss_bullet = 0;
    }
    
    if(origin_x != world->hero.x && origin_y != world->hero.y){
        new_projectile->is_enemy_bullet = 1;
    } else{
        new_projectile->is_enemy_bullet = 0;
    }

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

void updateHeroMovement(World *world, const bool *keyboard_state){
    if(keyboard_state == NULL){
        world->hero.x = world->hero.x;
        world->hero.y = world->hero.y;
    }
    if(keyboard_state[SDL_SCANCODE_W]){
        world->hero.y -= world->hero.speed;
    }
    if(keyboard_state[SDL_SCANCODE_S]){
        world->hero.y += world->hero.speed;
    }
    if(keyboard_state[SDL_SCANCODE_D]){
        world->hero.x += world->hero.speed;
    }
    if(keyboard_state[SDL_SCANCODE_A]){
        world->hero.x -= world->hero.speed;
    }
    if(world->hero.x + 100 >= 3024 || world->hero.x + 100 <= 0 || world->hero.y + 100 >= 1964 || world->hero.x + 100 <= 0){
        world->hero.x = 2924;
    }
}

void updateEnemies(World *world){
    Enemy *current_enemy = world->enemy_list;
    while(current_enemy != NULL){
        if(!detectRectCollision(world->hero.x, world->hero.y, world->hero.width, world->hero.height, current_enemy->x, current_enemy->y, current_enemy->width, current_enemy->height)){
            float delta_x = world->hero.x - current_enemy->x;
            float delta_y = world->hero.y - current_enemy->y;
            float dist = sqrt((delta_x * delta_x) + (delta_y * delta_y));

            current_enemy->x += (delta_x/ dist) * current_enemy->speed;
            current_enemy->y += (delta_y/ dist) * current_enemy->speed;

            current_enemy->cooldown_frames--;

            if(dist <= 200 && current_enemy->cooldown_frames <= 0){
                shootProjectile(world, current_enemy->x, current_enemy->y, world->hero.x, world->hero.y, 1, 50, 1, 0);
                current_enemy->cooldown_frames = 120;
            }
            
        }

        current_enemy = current_enemy->next;
    }

    if(world->boss.is_active == 1){
        float delta_x = world->hero.x - world->boss.x;
        float delta_y = world->hero.y - world->boss.y;
        float dist = sqrt((delta_x * delta_x) + (delta_y * delta_y));

        world->boss.x += (delta_x/ dist) * world->boss.speed;
        world->boss.y += (delta_y/ dist) * world->boss.speed;

        world->boss.cooldown_frames--;

        if(world->boss.cooldown_frames <= 0){
            shootProjectile(world, world->boss.x + 250, world->boss.y + 250, world->hero.x, world->hero.y, 1, 75, 0, 1);
            world->boss.cooldown_frames = 60;
        }
    }       
}
                            

void updateProjectiles(World *world){
    Projectile *current_projectile = world->projectile_list;

    while(current_projectile != NULL){
        current_projectile->x += current_projectile->dir_x * current_projectile->speed;
        current_projectile->y += current_projectile->dir_y * current_projectile->speed;

        current_projectile = current_projectile->next;
    }
}                         

void checkCollisions(World *world){
    Projectile *current_projectile = world->projectile_list;

    while(current_projectile != NULL){
        Enemy *current_enemy = world->enemy_list;

        while(current_enemy != NULL){
            if(detectRectCollision(current_projectile->x, current_projectile->y, current_projectile->width, current_projectile->height, current_enemy->x, current_enemy->y, current_enemy->width, current_enemy->height)){
                if(!current_projectile->is_enemy_bullet){
                    current_enemy->hp -= current_projectile->damage;
                    current_projectile->x = 5000;
                }
            }

            current_enemy = current_enemy->next;
        }

        if(detectRectCollision(current_projectile->x, current_projectile->y, current_projectile->width, current_projectile->height, world->hero.x, world->hero.y, world->hero.width, world->hero.height)){
            if(current_projectile->is_enemy_bullet){
                world->hero.hp -= current_projectile->damage;
                current_projectile->x = 5000;
            }
        }

        if((detectRectCollision(current_projectile->x, current_projectile->y, current_projectile->width, current_projectile->height, world->boss.x + 210, world->boss.y + 100, world->boss.width - 420, world->boss.height - 150) || detectRectCollision(current_projectile->x, current_projectile->y, current_projectile->width, current_projectile->height, world->boss.x + 15, world->boss.y + 175, world->boss.width - 50, world->boss.height - 400)) && world->boss.is_active){
            if(!current_projectile->is_enemy_bullet){    
                world->boss.hp -= current_projectile->damage;
                current_projectile->x = 5000;
            }
        }

        current_projectile = current_projectile->next;
    }
}

void cleanDeadEntities(World *world){
    Enemy *previous_enemy = NULL;
    Enemy *current_enemy = world->enemy_list;

    while(current_enemy != NULL){
        if(current_enemy->hp <= 0){
            Enemy *temp = current_enemy->next;
            
            if(previous_enemy == NULL){
                world->enemy_list = temp;
            } else{
                previous_enemy->next = temp;
            }

            free(current_enemy);

            world->enemies_remaining--;
            current_enemy = temp;
        } else{
            previous_enemy = current_enemy;
            current_enemy = current_enemy->next;
        }
    }

    Projectile *previous_projectile = NULL;
    Projectile *current_projectile = world->projectile_list;

    while(current_projectile != NULL){
        if(current_projectile->x >= 3024 || current_projectile->x <= -100 || current_projectile->y >= 1964 || current_projectile->y <= -100){
            Projectile *temp = current_projectile->next;
            
            if(previous_projectile == NULL){
                world->projectile_list = temp;
            } else{
                previous_projectile->next = temp;
            }

            free(current_projectile);
            current_projectile = temp;
        } else{
            previous_projectile = current_projectile;
            current_projectile = current_projectile->next;
        }
    }

    if(world->boss.hp <= 0 && world->boss.is_active){
        world->boss.is_active = 0;
        world->boss.x = 5000;
        world->current_state = STATE_VICTORY;
    }

    if(world->hero.hp <= 0 && world->boss.is_active){
        while(world->enemy_list != NULL){
            Enemy *temp = world->enemy_list->next;

            free(world->enemy_list);
            world->enemy_list = temp;
        }

        world->hero.x = 5000;
        world->current_state = STATE_GAMEOVER;
    }
}    

void updatePhysics(World *world, const bool *keyboard_state){
    updateHeroMovement(world, keyboard_state);
    updateEnemies(world);
    updateProjectiles(world);
    checkCollisions(world);
    cleanDeadEntities(world);
    if(world->enemies_remaining <= 0 && world->boss.is_active == 0){
        spawnBoss(world, 500, 200);
    }
}