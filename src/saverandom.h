#ifndef _save_random_
#define _save_random_

#include<stdio.h>
#include<string.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL2_gfxPrimitives.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>

#include"creategame.h"

extern  const char* path_to_pic;
extern  const int fps;
extern  SDL_Window * my_window;
extern  SDL_Renderer * my_renderer;
extern  int  Width_of_scr ;
extern  int  Height_of_scr ;
extern  SDL_Event *sdl_events;
extern  char myname[100];

void save_game();
void save_potion();
void load_potion();
void load_game();
void get_info_savedgame();

void play_random();
int random_territories();
int * get_random_indexes();
void random_location();
int get_number_player_rand();
int random_start_terr();
#endif