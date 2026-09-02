#include <stdlib.h>
#include <stdbool.h>
#include "screen.h"
#include "engine.h"
#include "entities.h"

void drawMenu(App *app){
    SDL_RenderTexture(app->renderer, app->texture_background_menu, NULL, NULL);

    SDL_FRect play_button;
    play_button.x = 600;
    play_button.y = 400;
    play_button.w = 300;
    play_button.h = 200;

    SDL_FRect quit_button;
    quit_button.x = 600;
    quit_button.y = 500;
    quit_button.w = 300;
    quit_button.h = 200;

    SDL_RenderTexture(app->renderer, app->texture_play_button, NULL, &play_button);
    SDL_RenderTexture(app->renderer, app->texture_quit_button, NULL, &quit_button);
}

void drawGame(App *app){
    SDL_RenderTexture(app->renderer, app->texture_map, NULL, NULL);

    Enemy *current_enemy = app->world->enemy_list;

    while(current_enemy != NULL){
        SDL_FRect enemy;
        enemy.x = current_enemy->x;
        enemy.y = current_enemy->y;
        enemy.w = current_enemy->width;
        enemy.h = current_enemy->height;

        SDL_RenderTexture(app->renderer, app->texture_enemy, NULL, &enemy);

        current_enemy = current_enemy->next;
    }

    Projectile *current_projectile = app->world->projectile_list;

    while(current_projectile != NULL){
        if(!current_projectile->is_boss_bullet){
            SDL_FRect projectile;
            projectile.x = current_projectile->x;
            projectile.y = current_projectile->y;
            projectile.w = current_projectile->width;
            projectile.h = current_projectile->height;
        
            SDL_RenderTexture(app->renderer, app->texture_projectile, NULL, &projectile);
        } else{
            SDL_FRect boss_projectile;
            boss_projectile.x = current_projectile->x;
            boss_projectile.y = current_projectile->y;
            boss_projectile.w = current_projectile->width;
            boss_projectile.h = current_projectile->height;
        
            SDL_RenderTexture(app->renderer, app->texture_boss_projectile, NULL, &boss_projectile);
        }

        current_projectile = current_projectile->next;
    }

    Hero current_hero = app->world->hero;

    SDL_FRect hero;
    hero.x = current_hero.x;
    hero.y = current_hero.y;
    hero.w = current_hero.width;
    hero.h = current_hero.height;

    SDL_RenderTexture(app->renderer, app->texture_hero, NULL, &hero);


    Boss current_boss = app->world->boss;

    if(current_boss.is_active == 1){
        SDL_FRect boss;
        boss.x = current_boss.x;
        boss.y = current_boss.y;
        boss.w = current_boss.width;
        boss.h = current_boss.height;

        SDL_RenderTexture(app->renderer, app->texture_boss, NULL, &boss);
    }
}

void drawGameOver(App *app){
    if(app->current_state == STATE_GAMEOVER){
        SDL_FRect quit_button;
        quit_button.x = 600;
        quit_button.y = 600;
        quit_button.w = 300;
        quit_button.h = 200;

        SDL_RenderTexture(app->renderer, app->texture_gameover_lost, NULL, NULL);
        SDL_RenderTexture(app->renderer, app->texture_quit_button, NULL, &quit_button);
    }
}

void drawVictory(App *app){
    if(app->current_state == STATE_VICTORY){
        SDL_FRect quit_button;
        quit_button.x = 600;
        quit_button.y = 600;
        quit_button.w = 300;
        quit_button.h = 200;
        
        SDL_RenderTexture(app->renderer, app->texture_gameover_win, NULL, NULL);
        SDL_RenderTexture(app->renderer, app->texture_quit_button, NULL, &quit_button);
    }
}

void drawApp(App *app){
    app->current_state = STATE_MENU;

    int running = 1;

    SDL_Event event;

    app->world = malloc(sizeof(World));
    initWorld(app->world);
    initGraphics(app, 1200, 900);

    while(running){
        SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
        SDL_RenderClear(app->renderer);

        while(SDL_PollEvent(&event)){
            if(event.button.type == SDL_EVENT_QUIT){
                running = 0;
            }
            if(event.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                if(app->current_state == STATE_MENU){
                    if(detectMouseInput(event.button.x, event.button.y, 600, 400, 300, 200)){
                        app->current_state = STATE_GAME;
                    }
                    if(detectMouseInput(event.button.x, event.button.y, 600, 500, 300, 200)){
                        running = 0;
                    }
                } else if(app->current_state == STATE_GAME){
                    shootProjectile(app->world, app->world->hero.x, app->world->hero.y, event.button.x, event.button.y, app->world->hero.speed, app->world->hero.damage, 0, 0);
                } else if(app->current_state == STATE_GAMEOVER || app->current_state == STATE_VICTORY){
                    if(detectMouseInput(event.button.x, event.button.y, 600, 600, 300, 200)){
                        app->current_state = STATE_MENU;
                        initWorld(app->world);
                    }
                }
            }
        }

        if(app->current_state == STATE_MENU){
            drawMenu(app);
        }

        if(app->current_state == STATE_GAME){
            const bool *keyboard_state = SDL_GetKeyboardState(NULL);
            updatePhysics(app->world, keyboard_state);
            app->current_state = app->world->current_state;
            drawGame(app);
        }

        if(app->current_state == STATE_GAMEOVER){
            drawGameOver(app);
        }

        if(app->current_state == STATE_VICTORY){
            drawVictory(app);
        }

        SDL_RenderPresent(app->renderer);
        SDL_Delay(16);
    }

    closeGraphics(app);
}