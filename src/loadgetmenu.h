#ifndef loadgetm_header
#define loadgetm_header

#include<string.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL2_gfxPrimitives.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>

#include"scorepotion.h"

extern  const char* path_to_pic;
extern  const int fps;
extern  SDL_Window * my_window;
extern  SDL_Renderer * my_renderer;
extern  int  Width_of_scr ;
extern  int  Height_of_scr ;
extern  SDL_Event *sdl_events;

extern char myname[100];

void first_loading();
void get_name();
int start_menu(int , char**);
int pausegame_menu();
void fin_graphic(int state);
SDL_Texture * create_loading_menu_img( SDL_Rect* location_sharif,const char* name);
SDL_Texture* create_loading_text(SDL_Color textcolor,const char* mytxt,SDL_Rect* location_text,const char* fontname,float farc_of8);
int min();
int max();
int choose_number_terrs();
#endif