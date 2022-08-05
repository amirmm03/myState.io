#include"loadgetmenu.h"




SDL_Texture * create_loading_menu_img( SDL_Rect* location_sharif,const char* name){
    char imgpath[100];
    strcpy(imgpath,path_to_pic);
    strcat(imgpath,name);
    
    SDL_Surface * sharif_image = IMG_Load(imgpath);
    if(!sharif_image) {
    printf("IMG_Load: %s\n", IMG_GetError());
    // handle error
}
    SDL_Texture * texture_sharif = SDL_CreateTextureFromSurface(my_renderer, sharif_image);
  
    location_sharif->h=sharif_image->h;
  
    location_sharif->w=sharif_image->w;
    location_sharif->x=Width_of_scr/2-location_sharif->w/2;
    location_sharif->y=Height_of_scr/2-location_sharif->h/2;
    SDL_FreeSurface(sharif_image);
    return texture_sharif;
    
}




SDL_Texture* create_loading_text(SDL_Color textcolor,const char* mytxt,SDL_Rect* location_text,const char* fontname,float farc_of8){
    char fontpath[100];
    strcpy(fontpath,path_to_pic);
    strcat(fontpath,"fonts/");
    strcat(fontpath,fontname);
    TTF_Font * font = TTF_OpenFont(fontpath, 100);
    SDL_Surface * surface_oftext = TTF_RenderText_Solid(font,mytxt,textcolor);
    SDL_Texture * texture_oftext = SDL_CreateTextureFromSurface(my_renderer, surface_oftext);
    int texW,texH;
    SDL_QueryTexture(texture_oftext, NULL, NULL, &texW, &texH);

    location_text->h=texH;
    location_text->w=texW;
    location_text->x=Width_of_scr/2-texW/2;
    location_text->y=Height_of_scr*farc_of8/8;

    TTF_CloseFont(font);
    SDL_FreeSurface(surface_oftext);

    return texture_oftext;
    
}

int min(int a,int b){
return a<b?a:b;
}
int max(int a,int b){
return a>b?a:b;
}
void first_loading(){

	
    SDL_Rect* location_sharif =  malloc(sizeof(SDL_Rect));
   
    SDL_Texture * texture_sharif = create_loading_menu_img(location_sharif,"sharif.png");


    SDL_Color text1color={22, 214, 150,255};
    SDL_Rect* location_text1=malloc(sizeof(SDL_Rect));
    SDL_Texture* texture_oftext1=create_loading_text(text1color,"Welcome to FOP project",location_text1,"FREESCPT.TTF",1);


    SDL_Color text2color={89, 22, 214,255};
    SDL_Rect* location_text2=malloc(sizeof(SDL_Rect));
    SDL_Texture* texture_oftext2=create_loading_text(text2color,"By Amirmahdi Meighani",location_text2,"FREESCPT.TTF",6);


    int alpha=0;
    int timeleft=4500;/////////////////////////////////////////////////////////////////////////////////////////////////change to 4500
    while (timeleft>0)
    {
    SDL_SetRenderDrawColor( my_renderer, 255, 255, 255, 255 );
    SDL_RenderClear( my_renderer );


    SDL_SetTextureAlphaMod(texture_sharif,alpha+5);
    SDL_RenderCopy(my_renderer, texture_sharif, NULL, location_sharif);
    SDL_SetTextureAlphaMod(texture_oftext1,alpha+5);
    SDL_RenderCopy(my_renderer, texture_oftext1, NULL,location_text1 );
    SDL_SetTextureAlphaMod(texture_oftext2,alpha+5);
    SDL_RenderCopy(my_renderer, texture_oftext2, NULL,location_text2 );


    SDL_RenderPresent(my_renderer);
    SDL_Delay(fps);
    timeleft-=fps;
    if (timeleft>=4500-2000)
        {
            alpha+=256/90;
        }
    
    }
    
    SDL_DestroyTexture(texture_sharif);
    SDL_DestroyTexture(texture_oftext2);
    SDL_DestroyTexture(texture_oftext1);
    free(location_sharif);
    free(location_text1);
    free(location_text2);
    return;
}

void get_name(){

    while (SDL_PollEvent( sdl_events) != 0){}
    
    SDL_Color text1color={255, 102, 0,255};
    SDL_Rect* location_text1=malloc(sizeof(SDL_Rect));
    SDL_Texture* texture_oftext1=create_loading_text(text1color,"Enter your name in English:",location_text1,"arial.ttf",1);

    SDL_Color text2color={61, 255, 71,255};
    SDL_Rect* location_text2=malloc(sizeof(SDL_Rect));
    SDL_Texture* texture_oftext2=create_loading_text(text2color,myname,location_text2,"comic.ttf",4);


    SDL_StartTextInput();
    SDL_SetRenderDrawColor( my_renderer,80, 80, 80, 255 );
    SDL_RenderClear( my_renderer );
    SDL_RenderCopy(my_renderer, texture_oftext1, NULL,location_text1 );
    SDL_RenderCopy(my_renderer,texture_oftext2,NULL,location_text2);
    SDL_RenderPresent(my_renderer);

    while (1){


    SDL_Delay(fps);

    
    int change=0;
    while (SDL_PollEvent( sdl_events) != 0){
        
    
    
        switch (sdl_events->type) {
                    case SDL_QUIT:
                        return;
                    case SDL_KEYDOWN:
                        if (sdl_events->key.keysym.scancode==SDL_SCANCODE_RETURN)
                        {
                            SDL_StopTextInput();
                            SDL_DestroyTexture(texture_oftext2);
                            SDL_DestroyTexture(texture_oftext1);
                            free(location_text1);
                            free(location_text2);
                            return;
                        }
                        if (sdl_events->key.keysym.scancode==SDL_SCANCODE_BACKSPACE && strlen(myname)>0)
                        {
                            myname[strlen(myname)-1] = '\0';
                            change=1;
                        }
                        break;
                        
                    case SDL_TEXTINPUT:

                        strcat(myname,sdl_events->text.text);
                        change=1;      
                        
                    default:
                        break;

        }

    }
    if (change)
    {

        texture_oftext2=create_loading_text(text2color,myname,location_text2,"comic.ttf",4);
        SDL_SetRenderDrawColor( my_renderer,80, 80, 80, 255 );
        SDL_RenderClear( my_renderer );
        SDL_RenderCopy(my_renderer, texture_oftext1, NULL,location_text1 );
        SDL_RenderCopy(my_renderer,texture_oftext2,NULL,location_text2);
        SDL_RenderPresent(my_renderer);
    }
    


    }


}



int start_menu(int numberofoptions,char** options){
    SDL_SetRenderDrawColor( my_renderer, 0,0,0, 255 );
    SDL_RenderClear( my_renderer );


    float diff=4-numberofoptions;
    diff/=2;
    SDL_Color notselectedcolor={0,240,0,255};
    SDL_Color selectedcolor={240,0,0,255};

    SDL_Rect locations[5];
    SDL_Texture * font_texture[5];
    for (int i = 0; i < numberofoptions; i++)
    {
        *(font_texture+i)=create_loading_text(notselectedcolor,*(options+i),locations+i,"JungleAdventurer.ttf",2+i+diff);
    }

    int state_of_menu_option[5]={0,0,0,0,0};
    
    int x_mouse,y_mouse;
    SDL_Rect imgrec;
    SDL_Texture * backimg=create_loading_menu_img(&imgrec,"startmenu.jpg");

    SDL_SetTextureBlendMode(backimg, SDL_BLENDMODE_BLEND);
   // SDL_SetTextureAlphaMod(backimg,150);
   int lowestx=999999 ,lowesty=locations->y,highestx=-1,highesty=locations[numberofoptions-1].y+locations[numberofoptions-1].h;
   for (int i = 0; i < numberofoptions; i++)
   {
       lowestx=min(lowestx,locations[i].x);
       highestx=max(highestx,locations[i].x+locations[i].w);
   }
   
   

    SDL_RenderCopy(my_renderer,backimg,NULL,&imgrec);
    roundedBoxRGBA(my_renderer,lowestx-20,lowesty-20,highestx+20,highesty+20,45,0,0,50,200); 
    while (1)
    {
       
        
        for (int i = 0; i < numberofoptions; i++)
        {
            SDL_RenderCopy(my_renderer,*(font_texture+i),NULL,(locations+i));
        }
    
        SDL_RenderPresent(my_renderer);
        SDL_Delay(fps);

        while (SDL_PollEvent( sdl_events) != 0){
            switch (sdl_events->type)
            {
            case  SDL_MOUSEMOTION:
                x_mouse = sdl_events->motion.x;
                y_mouse = sdl_events->motion.y;
                for (int i = 0; i < numberofoptions; i++)
                {
                    if(x_mouse>=locations[i].x && x_mouse<=locations[i].x+locations[i].w && y_mouse>=locations[i].y && y_mouse<=locations[i].y+locations[i].h)
                    {
                        if (state_of_menu_option[i]==0)
                        {
                            state_of_menu_option[i]=1;
                            *(font_texture+i)=create_loading_text(selectedcolor,*(options+i),locations+i,"JungleAdventurer.ttf",2+i+diff);
                        }
                        

                    }
                    else{
                        
                        if (state_of_menu_option[i])
                        {
                            
                            state_of_menu_option[i]=0;
                            *(font_texture+i)=create_loading_text(notselectedcolor,*(options+i),locations+i,"JungleAdventurer.ttf",2+i+diff);

                        }
                        
                    }
                }
                
                break;
            case SDL_MOUSEBUTTONDOWN:
            if(sdl_events->button.button==SDL_BUTTON_LEFT){
                for (int i = 0; i < numberofoptions; i++)
                {
                    if (state_of_menu_option[i])
                    {
                        for (int j = 0; j < numberofoptions; j++)
                        {
                            SDL_DestroyTexture(font_texture[j]);
                        }
                        SDL_DestroyTexture(backimg);
                        
                        return i;
                    }
                    
                }
            }
                break;
            default:
                break;
            }



        }

    }


}



int pausegame_menu(){
    int numberofoptions=3;
    char* options[3]={"continue","save game","exit"};
    SDL_Color notselectedcolor={0,240,0,255};
    SDL_Color selectedcolor={240,0,0,255};

    SDL_Rect locations[5];
    SDL_Texture * font_texture[5];
    for (int i = 0; i < numberofoptions; i++)
    {
        *(font_texture+i)=create_loading_text(notselectedcolor,*(options+i),locations+i,"JungleAdventurer.ttf",2.5+i);
    }

    int state_of_menu_option[5]={0,0,0,0,0};
    int x_mouse,y_mouse;

   int lowestx=999999 ,lowesty=locations->y,highestx=-1,highesty=locations[numberofoptions-1].y+locations[numberofoptions-1].h;
   for (int i = 0; i < numberofoptions; i++)
   {
       lowestx=min(lowestx,locations[i].x);
       highestx=max(highestx,locations[i].x+locations[i].w);
   }
   
   

    roundedBoxRGBA(my_renderer,lowestx-20,lowesty-20,highestx+20,highesty+20,45,0,0,50,190);        
    while (1)
    {    
        for (int i = 0; i < numberofoptions; i++)
        {
            SDL_RenderCopy(my_renderer,*(font_texture+i),NULL,(locations+i));
        }
    
        SDL_RenderPresent(my_renderer);
        SDL_Delay(fps);

        while (SDL_PollEvent( sdl_events) != 0){
            switch (sdl_events->type)
            {
            case  SDL_MOUSEMOTION:
                x_mouse = sdl_events->motion.x;
                y_mouse = sdl_events->motion.y;
                for (int i = 0; i < numberofoptions; i++)
                {
                    if(x_mouse>=locations[i].x && x_mouse<=locations[i].x+locations[i].w && y_mouse>=locations[i].y && y_mouse<=locations[i].y+locations[i].h)
                    {
                        if (state_of_menu_option[i]==0)
                        {
                            state_of_menu_option[i]=1;
                            *(font_texture+i)=create_loading_text(selectedcolor,*(options+i),locations+i,"JungleAdventurer.ttf",2.5+i);
                        }
                        

                    }
                    else{
                        
                        if (state_of_menu_option[i])
                        {
                            
                            state_of_menu_option[i]=0;
                            *(font_texture+i)=create_loading_text(notselectedcolor,*(options+i),locations+i,"JungleAdventurer.ttf",2.5+i);

                        }
                        
                    }
                }
                
                break;
            case SDL_MOUSEBUTTONDOWN:
            if(sdl_events->button.button==SDL_BUTTON_LEFT){
                for (int i = 0; i < numberofoptions; i++)
                {
                    if (state_of_menu_option[i])
                    {
                        for (int j = 0; j < numberofoptions; j++)
                        {
                            SDL_DestroyTexture(font_texture[j]);
                        }
                        
                        return i;
                    }
                    
                }
            }
                break;
            case SDL_KEYDOWN:
                if(sdl_events->key.keysym.scancode==SDL_SCANCODE_ESCAPE){
                    for (int j = 0; j < numberofoptions; j++)
                    {
                        SDL_DestroyTexture(font_texture[j]);
                    }
                    return 0;
                }
            default:
                break;
            }



        }

    }
}


void fin_graphic(int state){
    int timeleft = 1800;
    int alpha=5;
    if (state==1) {
        SDL_Color txtcolor={12,255,23,255};
        SDL_Rect txtrect,backg;
        SDL_Texture * mytxt=create_loading_text(txtcolor,"we won the battle!!! :) +15 points",&txtrect,"comic.ttf",3);
        backg.x=0;
        backg.y=0;
        backg.h=Height_of_scr;
        backg.w=Width_of_scr;
        SDL_SetRenderDrawBlendMode(my_renderer,SDL_BLENDMODE_BLEND);
        while (timeleft>0)
        {
            SDL_SetRenderDrawColor(my_renderer,255,255,255,alpha);
            SDL_RenderFillRect(my_renderer,&backg);
            SDL_RenderCopy(my_renderer,mytxt,NULL,&txtrect);
            SDL_RenderPresent(my_renderer);
            SDL_Delay(fps);
            timeleft-=fps;
        }
        timeleft=2000;
        alpha=35;
        while (timeleft>0)
        {
            SDL_SetRenderDrawColor(my_renderer,255,255,255,alpha);
            SDL_RenderFillRect(my_renderer,&backg);
            SDL_RenderCopy(my_renderer,mytxt,NULL,&txtrect);
            SDL_RenderPresent(my_renderer);
            SDL_Delay(fps);
            timeleft-=fps;
    
        }


    }else
    {    
        SDL_Color txtcolor={226, 40, 40,255};
        SDL_Rect txtrect,backg;
        SDL_Texture * mytxt=create_loading_text(txtcolor,"we lost the battle... :( -5 points",&txtrect,"comic.ttf",3);
        backg.x=0;
        backg.y=0;
        backg.h=Height_of_scr;
        backg.w=Width_of_scr;
        SDL_SetRenderDrawBlendMode(my_renderer,SDL_BLENDMODE_BLEND);
        while (timeleft>0)
        {
            SDL_SetRenderDrawColor(my_renderer,0,0,0,alpha);
            SDL_RenderFillRect(my_renderer,&backg);
            SDL_RenderCopy(my_renderer,mytxt,NULL,&txtrect);
            SDL_RenderPresent(my_renderer);
            SDL_Delay(fps);
            timeleft-=fps;
        }
        alpha=10;
        timeleft=2000;
        while (timeleft>0)
        {
            SDL_SetRenderDrawColor(my_renderer,0,0,0,alpha);
            SDL_RenderFillRect(my_renderer,&backg);
            SDL_RenderCopy(my_renderer,mytxt,NULL,&txtrect);
            SDL_RenderPresent(my_renderer);
            SDL_Delay(fps);
            timeleft-=fps;
            
        }

    }
    
}


int choose_number_terrs(){
    
    while (SDL_PollEvent( sdl_events) != 0){}
    
    char number_of_terr_str[40]="";

    SDL_Color text1color={0,240,0,255};
    SDL_Rect* location_text1=malloc(sizeof(SDL_Rect));
    SDL_Texture* texture_oftext1=create_loading_text(text1color,"Enter number of territories(6-18):",location_text1,"comic.ttf",1);

    SDL_Color text2color={240, 0, 0,255};
    SDL_Rect* location_text2=malloc(sizeof(SDL_Rect));
    SDL_Texture* texture_oftext2=create_loading_text(text2color,number_of_terr_str,location_text2,"comic.ttf",4);


    SDL_Rect imgrec;
    SDL_Texture * backimg=create_loading_menu_img(&imgrec,"startmenu.jpg");
    SDL_SetTextureBlendMode(backimg, SDL_BLENDMODE_BLEND);
    SDL_SetTextureColorMod(backimg,40,50,70);

    SDL_StartTextInput();
    SDL_RenderClear( my_renderer );
    SDL_RenderCopy(my_renderer,backimg,NULL,&imgrec);
    SDL_RenderCopy(my_renderer, texture_oftext1, NULL,location_text1 );
    SDL_RenderCopy(my_renderer,texture_oftext2,NULL,location_text2);
    SDL_RenderPresent(my_renderer);

    while (1){


    SDL_Delay(fps);

    
    int change=0;
    while (SDL_PollEvent( sdl_events) != 0){ 
    
        switch (sdl_events->type) {
                    case SDL_KEYDOWN:
                        if (sdl_events->key.keysym.scancode==SDL_SCANCODE_RETURN)
                        {
                            SDL_StopTextInput();
                            SDL_DestroyTexture(texture_oftext2);
                            SDL_DestroyTexture(texture_oftext1);
                            SDL_DestroyTexture(backimg);
                            free(location_text1);
                            free(location_text2);
                            return SDL_atoi(number_of_terr_str);
                        }
                        if (sdl_events->key.keysym.scancode==SDL_SCANCODE_BACKSPACE && strlen(number_of_terr_str)>0)
                        {
                            number_of_terr_str[strlen(number_of_terr_str)-1] = '\0';
                            change=1;
                        }
                        break;
                        
                    case SDL_TEXTINPUT:

                        strcat(number_of_terr_str,sdl_events->text.text);
                        change=1;      
                        
                    default:
                        break;

        }

    }
    if (change)
    {

        texture_oftext2=create_loading_text(text2color,number_of_terr_str,location_text2,"comic.ttf",4);
        SDL_RenderClear( my_renderer );
        SDL_RenderCopy(my_renderer,backimg,NULL,&imgrec);
        SDL_RenderCopy(my_renderer, texture_oftext1, NULL,location_text1 );
        SDL_RenderCopy(my_renderer,texture_oftext2,NULL,location_text2);
        SDL_RenderPresent(my_renderer);
    }
    


    }

}