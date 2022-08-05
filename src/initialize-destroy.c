#include"initialize-destroy.h"

Mix_Music *music;

void initializesdl(){

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER|SDL_INIT_AUDIO);//https://theimaginativeconservative.org/2015/11/music-war-remembrance-ten-great-classical-music-works-stephen-klugewicz.html
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_Init(MIX_INIT_MP3);

    SDL_DisplayMode stlDisplay_Mode;
    SDL_GetDesktopDisplayMode(0, &stlDisplay_Mode);

    Width_of_scr=stlDisplay_Mode.w;
    Height_of_scr=stlDisplay_Mode.h;
    
    my_window  =SDL_CreateWindow("terr.io",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,Width_of_scr,Height_of_scr, 0);
    my_renderer =SDL_CreateRenderer(my_window,-1,SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC );

    sdl_events=malloc(sizeof(SDL_Event));

    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
    char mp3path[100];
    strcpy(mp3path,path_to_pic);
    strcat(mp3path,"Dmitrij Schostakowitsch - Sinfonie Nr11.mp3");
    music = Mix_LoadMUS(mp3path);
    Mix_PlayMusic(music,-1);

    return;
}

void destroysdl(){
    Mix_FreeMusic(music);

    TTF_Quit();
    IMG_Quit();
    Mix_Quit();

    free(sdl_events);
    SDL_DestroyWindow(my_window);
    SDL_DestroyRenderer(my_renderer);


    SDL_Quit();
    return;
}