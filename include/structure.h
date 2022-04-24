#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED

typedef struct {
    SDL_Texture *sprite;
    SDL_Surface *sprite_sheet;
    SDL_Rect rect, dst;
    int vitesse;
    float vie;
    int mana, collisionA;
    char *path;
}personnages;

typedef struct {
    int **action;
    int ligne;
    int **point;
}actionPoint;


typedef struct Input
{
    SDL_bool key[SDL_NUM_SCANCODES];
    SDL_bool quit;
    int x, y, xrel, yrel;
    int xwheel, ywheel;
    SDL_bool mouse[6];
}Input;

typedef struct Direction
{
    SDL_bool right;
    SDL_bool left;

    SDL_bool down;

}Direction;

enum {REDUCE,ADD};
enum {NARUTO,SASUKE};

#endif // STRUCTURE_H_INCLUDED
