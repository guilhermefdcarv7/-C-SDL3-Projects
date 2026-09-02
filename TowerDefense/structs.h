#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

typedef enum{
    STATE_MENU,
    STATE_MAPS,
    STATE_GAME,
    STATE_GAMEOVER
} GameState;

typedef struct Enemy{
    float x, y;
    float range_radius;
    float speed;
    int hp;
    struct Enemy *next; 
} Enemy;

typedef struct Alie{
    float x, y;
    float range_radius;
    int damage;
    int cooldown_frames; 
    struct Alie *next;
} Alie;

typedef struct Projectile{
    float x, y;
    float dir_x, dir_y; 
    float speed;
    int damage;
    struct Projectile *next;
} Projectile;

typedef struct{
    Enemy *enemy_list; 
    Alie *alie_list;     
    Projectile *projectile_list; 
    GameState current_state;

    int money;
    int base_lives;
    int level_timer_frames;
    int spawn_timer_frames;
    int spawn_interval_frames;
} World;

typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture *texture_background_menu;
    SDL_Texture *texture_background_map_menu;
    SDL_Texture *texture_gameover_win;
    SDL_Texture *texture_gameover_lost;
    SDL_Texture *texture_quit_button;
    SDL_Texture *texture_exit_button;
    SDL_Texture *texture_play_button;
    SDL_Texture *texture_map_button;
    SDL_Texture *texture_map;
    SDL_Texture *texture_alie;
    SDL_Texture *texture_enemy;
    SDL_Texture *texture_projectile;

    World *world;
    GameState current_state;

    int window_width;
    int window_height;
} App;

#endif