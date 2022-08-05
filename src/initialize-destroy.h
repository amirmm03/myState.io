#ifndef initialize_header
#define initialize_header
//////////////////////////////////path->

#include<stdio.h>
#include<string.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL2_gfxPrimitives.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>


extern const char* path_to_pic;
extern  const int fps;
extern  SDL_Window * my_window;
extern  SDL_Renderer * my_renderer;
extern  int  Width_of_scr ;
extern  int  Height_of_scr ;
extern  SDL_Event *sdl_events;

void initializesdl();
void destroysdl();



#endif