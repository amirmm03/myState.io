#include<stdio.h>
#include<string.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL2_gfxPrimitives.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>


#include"initialize-destroy.h"
#include"loadgetmenu.h"
#include"creategame.h"
#include"saverandom.h"

#ifdef main
#undef main
#endif

/////////////////////////////////////////////////////////

const char* path_to_pic="./picandfont/";
const int fps=1000/60;
SDL_Window * my_window;
SDL_Renderer * my_renderer;
int  Width_of_scr ;
int  Height_of_scr ;
SDL_Event *sdl_events;
char myname[100]="";

int main(){
	
    initializesdl();

    first_loading();
   
    get_name();
    int state_start_menu;

    int quit_game=0;
    char* options[4]={"CREATE NEW GAME","LOAD SAVED GAME","LEADERBOARD","QUIT GAME"};
    while (!quit_game)
    {
        state_start_menu=start_menu(4,options);
        switch (state_start_menu)
        {
        case 3:
            //quit
            quit_game=1;
            break;
        case 2:
            show_leaderboard();
            break;
        case 1:
            //load map
            get_info_savedgame();
            break;
        case 0:
            //choose map
            kind_of_map_menu();
            break;
        default:
            break;
        }
        
    }

    destroysdl();
    return 0;
}
