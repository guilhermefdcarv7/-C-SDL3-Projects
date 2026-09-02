#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#define STATE_MENU 0
#define STATE_GAME 1
#define STATE_GAMEOVER 2
#define STATE_VICTORY 3

typedef struct Projectile {
    float x, y;
    float width, height;
    float dir_x, dir_y;
    float speed;
    int damage;
    int is_enemy_bullet;
    int is_boss_bullet;
    struct Projectile *next;
} Projectile;

typedef struct Enemy {
    float x, y;
    float speed;
    int hp;
    int width, height;
    int cooldown_frames;
    struct Enemy *next;
} Enemy;

typedef struct Boss {
    float x, y;
    float speed;
    int hp;
    int max_hp;
    int width, height;
    int cooldown_frames;
    int is_active;
} Boss;

typedef struct Hero {
    float x, y;
    float speed;
    int damage;
    int hp;
    int max_hp;
    int width, height;
    int cooldown_frames;
} Hero;

typedef struct World {
    Hero hero;
    Boss boss;
    Enemy *enemy_list;
    Projectile *projectile_list;
    
    int current_level;
    int enemies_remaining; 
    int current_state;
} World;

typedef struct App {
    SDL_Window *window;
    SDL_Renderer *renderer;
    
    SDL_Texture *texture_background_menu;
    SDL_Texture *texture_map;
    SDL_Texture *texture_gameover_win;
    SDL_Texture *texture_gameover_lost;
    SDL_Texture *texture_play_button;
    SDL_Texture *texture_quit_button;
    SDL_Texture *texture_hero;
    SDL_Texture *texture_enemy;
    SDL_Texture *texture_boss;
    SDL_Texture *texture_projectile;
    SDL_Texture *texture_boss_projectile;
    
    int current_state;
    World *world;
} App;

#endif