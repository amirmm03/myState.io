#ifndef _score_potion_
#define _score_potion_

#include<stdio.h>
#include<string.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL2_gfxPrimitives.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>

#include"creategame.h"
#include<time.h>

extern  const char* path_to_pic;
extern  const int fps;
extern  SDL_Window * my_window;
extern  SDL_Renderer * my_renderer;
extern  int  Width_of_scr ;
extern  int  Height_of_scr ;
extern  SDL_Event *sdl_events;
extern  char myname[100];


int find_yourscore();
void create_score_texture();
void show_score(int number_of_players);
void update_score();
void rewrite_scores();
void show_leaderboard();
void quit_scoresposions();


struct potion{
    int x_loc;
    int y_loc;
    int potion_kind;
    struct potion * next_potion;///-1=null 0=your speed will double 1=enemy soldier stuck 2=no limit create soldier 3= create spldier double speed
};


void create_potion();
void init_potion();
void rendercopy_potion_update();
void save_load_potion(int save_);
#endif