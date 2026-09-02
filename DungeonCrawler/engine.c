#include "structs.h"
#include "engine.h"

int initGraphics(App *app, int width, int height){
    if(SDL_Init(SDL_INIT_VIDEO)){
        app->window = SDL_CreateWindow("DUNGEON CRAWLER - SDL3", width, height, SDL_WINDOW_FULLSCREEN);
        app->renderer = SDL_CreateRenderer(app->window, NULL);
        app->texture_background_menu = IMG_LoadTexture(app->renderer, "assets/background_menu.jpg");
        app->texture_map = IMG_LoadTexture(app->renderer, "assets/map.jpg");
        app->texture_gameover_win = IMG_LoadTexture(app->renderer, "assets/gameover_win.png");
        app->texture_gameover_lost = IMG_LoadTexture(app->renderer, "assets/gameover_lost.png");
        app->texture_play_button = IMG_LoadTexture(app->renderer, "assets/play_button.png");
        app->texture_quit_button = IMG_LoadTexture(app->renderer, "assets/quit_button.png");
        app->texture_hero = IMG_LoadTexture(app->renderer, "assets/hero.png");
        app->texture_boss = IMG_LoadTexture(app->renderer, "assets/boss.png");
        app->texture_enemy = IMG_LoadTexture(app->renderer, "assets/enemy.png");
        app->texture_projectile = IMG_LoadTexture(app->renderer, "assets/projectile.png");
        app->texture_boss_projectile = IMG_LoadTexture(app->renderer, "assets/boss_projectile.png");

        return 1;
    } else{
        printf("---> ERROR: %s", SDL_GetError());

        return 0;
    }
}

void closeGraphics(App *app){
    SDL_DestroyWindow(app->window);
    SDL_DestroyRenderer(app->renderer);
    SDL_Quit();
}


int detectMouseInput(float mouse_x, float mouse_y, float box_x, float box_y, float box_w, float box_h){
    return(mouse_x > box_x && mouse_x < box_x + box_w && mouse_y > box_y && mouse_y < box_y + box_h);
}

int detectRectCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2){
    return(x1 < x2 +w2 && x2 < x1 + w1 && y1 < y2 + h2 && y2 < y1+ h1);
}

