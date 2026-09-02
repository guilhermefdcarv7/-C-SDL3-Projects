#include <stdlib.h>
#include "screen.h"
#include "engine.h"
#include "entities.h"

void drawMenu(App *app){
    SDL_RenderTexture(app->renderer, app->texture_background_menu, NULL, NULL);

    SDL_FRect play_button;
    play_button.x = 300;
    play_button.y = 200;
    play_button.w = 300;
    play_button.h = 200;

    SDL_FRect map_menu_button;
    map_menu_button.x = 300;
    map_menu_button.y = 400;
    map_menu_button.w = 300;
    map_menu_button.h = 200;

    SDL_FRect quit_button;
    quit_button.x = 300;
    quit_button.y = 600;
    quit_button.w = 300;
    quit_button.h = 200;

    SDL_RenderTexture(app->renderer, app->texture_play_button, NULL, &play_button);
    SDL_RenderTexture(app->renderer, app->texture_map_button, NULL, &map_menu_button);
    SDL_RenderTexture(app->renderer, app->texture_quit_button, NULL, &quit_button);
}

void drawMapMenu(App *app){
    SDL_RenderTexture(app->renderer, app->texture_background_map_menu, NULL, NULL);

    SDL_FRect map;
    map.x = 250;
    map.y = 450;
    map.w = 200;
    map.h = 100;

    SDL_FRect exit_button;
    exit_button.x = 1250;
    exit_button.y = 25;
    exit_button.w = 200;
    exit_button.h = 100;

    SDL_RenderTexture(app->renderer, app->texture_map, NULL, &map);
    SDL_RenderTexture(app->renderer, app->texture_exit_button, NULL, &exit_button);
}

void drawGame(App *app){
        SDL_RenderTexture(app->renderer, app->texture_map, NULL, NULL);

        Enemy *current_enemy = app->world->enemy_list;

        while(current_enemy != NULL){
            SDL_FRect enemy;
            enemy.x = current_enemy->x;
            enemy.y = current_enemy->y;
            enemy.w = 200;
            enemy.h = 200;

            SDL_RenderTexture(app->renderer, app->texture_enemy, NULL, &enemy);

            current_enemy = current_enemy->next;
        }

        Alie *current_alie  = app->world->alie_list;

        while(current_alie != NULL){
            SDL_FRect alie;
            alie.x = current_alie->x - 50;
            alie.y = current_alie->y - 50;
            alie.w = 100;
            alie.h = 100;
        
            SDL_RenderTexture(app->renderer, app->texture_alie, NULL, &alie);

            current_alie = current_alie->next;
        }

        Projectile *current_projectile = app->world->projectile_list;

        while(current_projectile != NULL){
            SDL_FRect projectile;
            projectile.x = current_projectile->x;
            projectile.y = current_projectile->y;
            projectile.w = 50;
            projectile.h = 50;

            SDL_RenderTexture(app->renderer, app->texture_projectile, NULL, &projectile);

            current_projectile = current_projectile->next;
        }

}

void drawGameOver(App *app){
    if(app->current_state == STATE_GAMEOVER){
        SDL_FRect gameover;
        gameover.x = 600;
        gameover.y = 300;
        gameover.w = 500;
        gameover.h = 500;

        SDL_RenderTexture(app->renderer, app->texture_gameover_lost, NULL, &gameover);
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
                    if(detectMouseInput(event.button.x, event.button.y, 300, 200, 300, 200)){
                        app->current_state = STATE_GAME;
                    }

                    if(detectMouseInput(event.button.x, event.button.y, 300, 400, 300, 200)){
                        app->current_state = STATE_MAPS;
                    }

                    if(detectMouseInput(event.button.x, event.button.y, 300, 600, 300, 200)){
                        running = 0;
                    }
                } else if(app->current_state == STATE_MAPS){
                    if(detectMouseInput(event.button.x, event.button.y, 1250, 25, 200, 100)){
                        app->current_state = STATE_MENU;
                    }
                } else if(app->current_state == STATE_GAME){
                    if(detectMouseInput(event.button.x, event.button.y, 0, 0, 1200, 1200)){
                        addAlie(app->world, event.button.x, event.button.y);
                    }
                }
            }
        }

        if(app->current_state == STATE_MENU){
            drawMenu(app);
        }
        
        if(app->current_state == STATE_MAPS){
            drawMapMenu(app);
        }

        if(app->current_state == STATE_GAME){
            updatePhysics(app->world);
            drawGame(app);
        }

        if(app->current_state == STATE_GAMEOVER){
            drawGameOver(app);
        }

        SDL_RenderPresent(app->renderer);
        SDL_Delay(16);
    }   

    closeGraphics(app);
}