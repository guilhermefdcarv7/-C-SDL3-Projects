#include <stdio.h>
#include "engine.h"

int initGraphics(App *app, int width, int height){
    if(SDL_Init(SDL_INIT_VIDEO)){
        app->window = SDL_CreateWindow("Tower Defense", width, height, SDL_WINDOW_FULLSCREEN);
        app->renderer = SDL_CreateRenderer(app->window, NULL);

        app->texture_background_menu = IMG_LoadTexture(app->renderer, "assets/background_menu.jpg");
        app->texture_background_map_menu = IMG_LoadTexture(app->renderer, "assets/background_map_menu.jpg");
        app->texture_gameover_win = IMG_LoadTexture(app->renderer, "assets/gameover_win.png");
        app->texture_gameover_lost= IMG_LoadTexture(app->renderer, "assets/gameover_lost.png");
        app->texture_play_button = IMG_LoadTexture(app->renderer, "assets/play_button.png");
        app->texture_exit_button = IMG_LoadTexture(app->renderer, "assets/exit_button.png");
        app->texture_map_button = IMG_LoadTexture(app->renderer, "assets/maps_button.png");
        app->texture_quit_button = IMG_LoadTexture(app->renderer, "assets/quit_button.png");
        app->texture_projectile = IMG_LoadTexture(app->renderer, "assets/projectile.png");
        app->texture_map = IMG_LoadTexture(app->renderer, "assets/map.jpg");
        app->texture_alie = IMG_LoadTexture(app->renderer, "assets/alie.png");
        app->texture_enemy = IMG_LoadTexture(app->renderer, "assets/enemy.png");

        return 1;
    }

    printf("---> ERROR: %s", SDL_GetError());

    return 0;
}

void closeGraphics(App *app){
    SDL_DestroyWindow(app->window);
    SDL_DestroyRenderer(app->renderer);
    SDL_Quit();
}

int detectMouseInput(float mouse_x, float mouse_y, float box_x, float box_y, float box_w, float box_h){
    if(mouse_x > box_x && mouse_x < box_x + box_w && mouse_y > box_y && mouse_y < box_y + box_h){
        return 1;
    }

    return 0;
}