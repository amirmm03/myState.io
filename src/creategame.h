#ifndef _create_newgame_
#define _create_newgame_

#include<string.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL2_gfxPrimitives.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include<math.h>

#include"loadgetmenu.h"
#include"saverandom.h"
#include"scorepotion.h"

extern  const char* path_to_pic;
extern  const int fps;
extern  SDL_Window * my_window;
extern  SDL_Renderer * my_renderer;
extern  int  Width_of_scr ;
extern  int  Height_of_scr ;
extern  SDL_Event *sdl_events;
extern  char myname[100];


#define itoa myitoa
void myitoa(int n, char s[],int nothing);

struct pair{
    int16_t x;
    int16_t y;
};

struct territory{
    int owner;
    int soldiers;
    int destination_of_attack;
};

struct player{
    int number_of_terr;
    SDL_Color player_color;
    char name[50];
    int score;
    int potion_num;///-1=null 0=your speed will double 1=enemy soldier stuck 2=no limit create soldier 3= create spldier double speed
    int remaining_time_potion;
};

struct soldier
{
    struct pair now_location;
    uint16_t owner;
    uint16_t source;
    uint16_t destination;
    struct soldier * next_soldier;
    uint16_t timeleft;
    uint16_t whole_time;
    int16_t x_diff;
    int16_t y_diff;
};

void kind_of_map_menu();
void play_map(int,int);
void update_score_for_potion();

#endif
