#ifndef INCLUDE_H_INCLUDED
#define INCLUDE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../src/functions.c"

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);
SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);
int setWindowColor(SDL_Renderer *renderer, SDL_Color color);
int animation(SDL_Renderer *renderer, personnages personnage);
int init_personnage(SDL_Renderer *renderer, personnages *personnage, char path[], int x, int y, int w, int h, int vitesse, int dstX);
void updateEvent(struct Input *input);
void SetKeyToFalse(Input* in);
void animationP(personnages* personnage,int x , int y, int nombreAnimation,int reset_x,Input in);
void BougerPerso(personnages *personnage, Direction *dir, Input *in);
Direction narutoMove(Input in, personnages *personnage, Direction dir, SDL_Window *window, actionPoint actionPoint, SDL_Renderer *renderer, SDL_Texture *texture);
int SetSpeed(Input *in,personnages *personnage);
int fillActionPoint(const char *path, actionPoint *actionPoint, SDL_Renderer *renderer, int perso);
void upPoint(actionPoint *actionPoint, personnages *personnages, int perso);
void show_life(personnages *personnage, SDL_Renderer *renderer);
void life(SDL_Renderer *renderer, personnages *personnage);
void changeLife (personnages *personnage,int dmg, Input in);
void gameOver(personnages *personnage, Input *in,SDL_Renderer*);
int murInvisible(personnages *personnage, int narutoOrSasuke);
void changeMana (personnages *personnage,int quantity,int reduceOrAdd);

#endif // INCLUDE_H_INCLUDED
