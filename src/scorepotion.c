#include "scorepotion.h"

SDL_Texture * player_score_texture[10];
SDL_Rect locations[10];
SDL_Texture * box=NULL;
SDL_Rect boxlocation;

int old_user(FILE* scorefile,int number_of_users){
    for (int i = 0; i < number_of_users; i++)
    {
        char thisname[50];
        fscanf(scorefile," %s",thisname);
        if(strcmp(myname,thisname)==0){
            return 1;
        }
        fscanf(scorefile," %*d");
    }
    return 0;
}

int find_yourscore(){
    char scorepath[50];
    int this_player_score;
    strcpy(scorepath,path_to_pic);
    strcat(scorepath,"/user_score.txt");
    FILE * scorefile=fopen(scorepath,"r");
    int number_of_users;
    fscanf(scorefile," %d",&number_of_users);
    if(old_user(scorefile,number_of_users)){
        fscanf(scorefile,"%d",&this_player_score);
        fclose(scorefile);
        return this_player_score;
    }else{
        fclose(scorefile);
        scorefile=fopen(scorepath,"r");
        fscanf(scorefile," %*d ");

        int scores[50];
        char names[50][50];
        for (int i = 0; i < number_of_users; i++)
        {
            fscanf(scorefile,"%s %d",names[i],scores+i);
        }
        
        fclose(scorefile);
        scorefile=fopen(scorepath,"w");
        fprintf(scorefile,"%d\n",number_of_users+1);
        for (int i = 0; i < number_of_users; i++)
        {
            fprintf(scorefile,"%s %d\n",names[i],scores[i]);
        }
        fprintf(scorefile,"%s %d",myname,10);
        fclose(scorefile);
        return 10;
    }
}


void rewrite_thisplayer_score(int his_score){
    char scorepath[50];
    strcpy(scorepath,path_to_pic);
    strcat(scorepath,"/user_score.txt");
    FILE * scorefile=fopen(scorepath,"r");
    int number_of_users;
    fscanf(scorefile," %d",&number_of_users);
    int scores[50];
    char names[50][50];
    for (int i = 0; i < number_of_users; i++)
    {
        fscanf(scorefile,"%s %d",names[i],scores+i);
    }
    fclose(scorefile);
    scorefile=fopen(scorepath,"w");
    fprintf(scorefile,"%d\n",number_of_users);
    for (int i = 0; i < number_of_users; i++)
    {
        if(strcmp(myname,names[i]))fprintf(scorefile,"%s %d\n",names[i],scores[i]);
        else fprintf(scorefile,"%s %d\n",names[i],his_score);
    }
    fclose(scorefile);
}

void get_scores(int number_of_players,struct player * player_info,int for_leaderboard){
    char scorepath[50];
    strcpy(scorepath,path_to_pic);
    strcat(scorepath,"/scores.txt");
    FILE * scorefile=fopen(scorepath,"r");
    fscanf(scorefile,"%*s %*d");
    player_info->score=find_yourscore();
    char thisname[20];
    int thisscore;
    if (for_leaderboard)
    {
        for (int i = 1; i < 5; i++)
        {
        fscanf(scorefile," %s %d",thisname,&thisscore);        
        (player_info+i)->score=thisscore;
        strcpy((player_info+i)->name,thisname);
        }
        return;
    }
    
  
    for (int i = 1; i < 5; i++)
    {
        fscanf(scorefile," %s %d",thisname,&thisscore);
            
        for (int j = 1; j < number_of_players; j++)
        {
            
            if (strcmp(thisname,(player_info+j)->name)==0)
            {
                (player_info+j)->score=thisscore;
            }
            
        }
        
    }

    fclose(scorefile);
}


SDL_Texture * create_one_texture(SDL_Color textcolor,const char* mytxt,SDL_Rect* location_text,const char* fontname,int order,int for_leaderboard){
    char fontpath[100];
    strcpy(fontpath,path_to_pic);
    strcat(fontpath,"fonts/");
    strcat(fontpath,fontname);
    TTF_Font * font;
    if(for_leaderboard) font = TTF_OpenFont(fontpath, 100);
    else  font = TTF_OpenFont(fontpath, 23);
    SDL_Surface * surface_oftext = TTF_RenderText_Solid(font,mytxt,textcolor);
    SDL_Texture * texture_oftext = SDL_CreateTextureFromSurface(my_renderer, surface_oftext);
    int texW,texH;
    SDL_QueryTexture(texture_oftext, NULL, NULL, &texW, &texH);
    if (for_leaderboard)
    {
    location_text->h=texH;
    location_text->w=texW;
    location_text->x=Width_of_scr/2-texW/2;;
    location_text->y=250+order*texH;
    }
    else{
    location_text->h=texH;
    location_text->w=texW;
    location_text->x=1480;
    location_text->y=150+order*texH;
    }

    TTF_CloseFont(font);
    SDL_FreeSurface(surface_oftext);

    return texture_oftext;
}

int scorecmp(const void * a,const void * b){
    struct player ** f=(struct player **)a;
    struct player ** s=(struct player **)b;
    int ans=(*f)->score-(*s)->score;
    return -ans;
}

struct player ** sortscore(int number_of_players,struct player * player_info){
    struct player **ans=malloc(sizeof(struct player *)*10) ;
    for (int i = 0; i < number_of_players; i++)
    {
        *(ans+i)=(player_info+i);
    }

    qsort(ans,number_of_players,sizeof( struct player *),scorecmp);
 
    return ans;
}
void createbox(int number_of_players){
    char path[50];
    strcpy(path,path_to_pic);
    strcat(path,"/box.png");
    SDL_Surface * thisimage = IMG_Load(path);
    
    box = SDL_CreateTextureFromSurface(my_renderer, thisimage);
    boxlocation.h=number_of_players* (locations[0].h) +40;
    boxlocation.w=450;
    boxlocation. x=1480-20;
    boxlocation. y=150-20;
    SDL_FreeSurface(thisimage);
    SDL_SetTextureColorMod(box,97, 28, 224);
    SDL_SetTextureAlphaMod(box,100);
}
void create_score_texture(int number_of_players,struct player * player_info,int for_leaderboard){    

    strcpy(player_info->name,myname);
    
    get_scores( number_of_players, player_info,for_leaderboard);

    struct player ** ordered_player_info=sortscore( number_of_players, player_info);
      
    SDL_Color textcolor={7, 110, 190,255};
    if (for_leaderboard)
    {
        textcolor.r=255;
        textcolor.g=102;
        textcolor.b=0;
        textcolor.a=255;
    }
    
    char thistext[50];
    for (int i = 0; i < number_of_players; i++)
    {
        strcpy(thistext,(*(ordered_player_info+i))->name);
        
        strcat(thistext," : ");
        char thisscore[10];
         
        itoa((*(ordered_player_info+i))->score,thisscore,10);
        
        strcat(thistext,thisscore);
    
        
        *(player_score_texture+i)=create_one_texture(textcolor,thistext,(locations+i),"JungleAdventurer.ttf",i,for_leaderboard);
    }
    free(ordered_player_info);

    if (box==NULL)
    {
        createbox(number_of_players);
    }
    return;
    
}


void show_score(int number_of_players){
    SDL_RenderCopy(my_renderer,box,NULL,&boxlocation);
    for (int i = 0; i < number_of_players; i++)
    {
        SDL_RenderCopy(my_renderer,*(player_score_texture+i),NULL,locations+i);
    }

}

void update_score(int number_of_players,struct player * player_info){

    struct player ** ordered_player_info=sortscore( number_of_players, player_info);
    
    SDL_Color textcolor={7, 110, 190,255};
    char thistext[50];
 
    for (int i = 0; i < number_of_players; i++)
    {
        strcpy(thistext,(*(ordered_player_info+i))->name);
        
        strcat(thistext,": ");
        char thisscore[10];
        
        itoa((*(ordered_player_info+i))->score,thisscore,10);
        strcat(thistext,thisscore);
        //strcat(thistext," potion: ");
        switch ((*(ordered_player_info+i))->potion_num)
        {
        case -1:
            strcat(thistext," no potion");
            break;
        case 0:
            strcat(thistext," faster movement");
            break;
        case 1:
            strcat(thistext," enemy stuck");
            break;
        case 2:
            strcat(thistext," create soldier no limit");
            break;
        case 3:
            strcat(thistext," create soldier faster");
            break;
        default:
            break;
        }
        
        *(player_score_texture+i)=create_one_texture(textcolor,thistext,(locations+i),"JungleAdventurer.ttf",i,0);
    }
    if (box==NULL)
    {
        createbox(number_of_players);
    }
    free(ordered_player_info);
}

void rewrite_scores(int number_of_players,struct player * player_info){
    char scorepath[50];
    strcpy(scorepath,path_to_pic);
    strcat(scorepath,"/scores.txt");
    FILE * scorefile=fopen(scorepath,"r");
    int numbers[10];

    numbers[0]=player_info->score;
    if((player_info)->number_of_terr==0)
    numbers[0]-=5;
    else numbers[0]+=15;


    fscanf(scorefile," %*s %*d");
    char thisname[20];
    int thisscore;

    for (int i = 1; i < 5; i++)
    {
        fscanf(scorefile," %s %d",thisname,&thisscore);
            
        for (int j = 1; j < number_of_players; j++)
        {
            
            if (strcmp(thisname,(player_info+j)->name)==0)
            {
                numbers[i]=(player_info+j)->score;
                if((player_info+j)->number_of_terr==0)
                numbers[i]-=5;
                else numbers[i]+=15;
                break;
            }
            numbers[i]=thisscore;
        }
        
    }
    fclose(scorefile);
    scorefile=fopen(scorepath,"w");
    fprintf(scorefile,"null %d\n james(red) %d\n alex(blue) %d\n emma(yellow) %d\n sara(pink) %d",0,numbers[1],numbers[2],numbers[3],numbers[4]);
    rewrite_thisplayer_score(numbers[0]);
    fclose(scorefile);
    
}



void show_leaderboard(){
    SDL_Rect back_loc,back_buttom_loc;
    SDL_Texture * backg= create_loading_menu_img(&back_loc,"startmenu.jpg");
    SDL_Color notselectedcolor={0,240,0,255};
    SDL_Color selectedcolor={240,0,0,255};
    
    SDL_RenderCopy(my_renderer,backg,NULL,&back_loc);
    SDL_Texture* backbuttom= create_loading_text( notselectedcolor,"back",&back_buttom_loc,"JungleAdventurer.ttf",5.6);
    struct player playerinfo[10];
    create_score_texture(5,playerinfo,1);
    int lowestx=999999 ,lowesty=locations->y,highestx=-1,highesty=back_buttom_loc.y+locations[4].h;
    for (int i = 0; i < 5; i++)
    {
       lowestx=min(lowestx,locations[i].x);
       highestx=max(highestx,locations[i].x+locations[i].w);
    }
    roundedBoxRGBA(my_renderer,lowestx-20,lowesty-20,highestx+20,highesty+20,45,0,0,50,200);

    int state_of_menu_option=0;
    while (1)
    {   
        for (int i = 0; i <5; i++)
        {
            SDL_RenderCopy(my_renderer,*(player_score_texture+i),NULL,locations+i);
        }
        SDL_RenderCopy(my_renderer,backbuttom,NULL,&back_buttom_loc);
        SDL_RenderPresent(my_renderer);
        SDL_Delay(fps);
        int x_mouse=0,y_mouse =0;
        
        while (SDL_PollEvent( sdl_events) != 0){
            switch (sdl_events->type)
            {
            case  SDL_MOUSEMOTION:
                x_mouse = sdl_events->motion.x;
                y_mouse = sdl_events->motion.y;
                    if(x_mouse>=back_buttom_loc.x && x_mouse<=back_buttom_loc.x+back_buttom_loc.w && y_mouse>=back_buttom_loc.y && y_mouse<=back_buttom_loc.y+back_buttom_loc.h)
                    {
                        if (state_of_menu_option==0)
                        {
                            state_of_menu_option=1;
                            backbuttom=create_loading_text(selectedcolor,"back",&back_buttom_loc,"JungleAdventurer.ttf",5.6);
                        }
                        
                    }
                    else{
                        
                        if (state_of_menu_option)
                        {
                            
                            state_of_menu_option=0;
                            backbuttom=create_loading_text(notselectedcolor,"back",&back_buttom_loc,"JungleAdventurer.ttf",5.6);

                        }
                        
                    }
                
                
                break;

            case SDL_MOUSEBUTTONDOWN:
            if(sdl_events->button.button==SDL_BUTTON_LEFT){
        
                    if (state_of_menu_option)
                    {
                        
                        SDL_DestroyTexture(backbuttom);
                        SDL_DestroyTexture(backg);
                        quit_scoresposions();
                        SDL_RenderClear(my_renderer);
                        return ;
                    }
            }
                break;
            default:
                break;
            }

        }   

    }
}


//////////////////////////////////    potion    //////////////////////////////////


struct potion  *all_potions_head=NULL;
struct potion  *all_potions_tail=NULL;
SDL_Texture * potion_texture[4];
SDL_Rect loc_potion_texture;
int reserved_x=-1,reserved_y=-1;

void init_potion(){
    srand(time(0));
    char imgpath[100];
    for(int i=0;i<4;i++){
        strcpy(imgpath,path_to_pic);
        strcat(imgpath,"potion");
        char num[3];
        itoa(i+1,num,10);
        strcat(imgpath,num);
        strcat(imgpath,".png");
        SDL_Surface * terimage = IMG_Load(imgpath);
        potion_texture[i] = SDL_CreateTextureFromSurface(my_renderer, terimage);
        loc_potion_texture.h=40;
        loc_potion_texture.w=40;
        SDL_FreeSurface(terimage);
    }
}


void create_potion(struct soldier * tail_sol,struct player * players){    
    for (int i = 0; i < 10; i++)
    {
        if (players[i].potion_num>=0)
        {
            if (players[i].remaining_time_potion==0)
            {
                players[i].potion_num=-1;
                update_score_for_potion();
                continue;
            }
            players[i].remaining_time_potion-=1;        
        }
        
    }
    
    int x=rand()%3;
    if (x!=1||tail_sol==NULL)return;
    if (reserved_x<0)
    {
        reserved_x=tail_sol->now_location.x;
        reserved_y=tail_sol->now_location.y;
        return;
    }
    
    if (all_potions_head==NULL)
    {
        all_potions_head=malloc(sizeof(struct potion));
        all_potions_tail=all_potions_head;
    }else
    {
        all_potions_tail->next_potion=malloc(sizeof(struct potion));
        all_potions_tail=all_potions_tail->next_potion;
    }
    all_potions_tail->x_loc=reserved_x;
    all_potions_tail->y_loc=reserved_y;
    all_potions_tail->potion_kind=rand()%4;
    all_potions_tail->next_potion=NULL;

    reserved_x=tail_sol->now_location.x;
    reserved_y=tail_sol->now_location.y;

}

float distance_sol_potion(struct soldier * sol,struct potion * pot){
    float ans=(sol->now_location.x-pot->x_loc)*(sol->now_location.x-pot->x_loc);
    ans+=(sol->now_location.y-pot->y_loc)*(sol->now_location.y-pot->y_loc);
    return sqrt(ans);
}

void delete_pot(struct potion ** this_pot,struct potion ** before_pot){
    if (*before_pot==NULL){
        if ((*this_pot)->next_potion==NULL)
        {
            all_potions_tail=NULL;
        }
        *before_pot=*this_pot;
        *this_pot=(*this_pot)->next_potion;
        free(*before_pot);
        *before_pot=NULL;
        all_potions_head=*this_pot;
        return;
    }
    if ((*this_pot)->next_potion==NULL)
    {
        all_potions_tail=*before_pot;
    }
    (*before_pot)->next_potion=(*this_pot)->next_potion;    
    free(*this_pot);
    (*this_pot)=(*before_pot)->next_potion;

}

void collect_potion(struct soldier * head_sol,struct player * all_players){
    struct potion* now_pot=all_potions_head;
    struct  potion* beforenow_pot=NULL;
    struct  soldier * this_sol=head_sol;
    while (now_pot)
    {
        this_sol=head_sol;
        int flag=1;
        while (this_sol)
        {

            if (15>distance_sol_potion( this_sol,now_pot))
            {
                if (all_players[this_sol->owner].potion_num==-1)
                {
                    all_players[this_sol->owner].potion_num = now_pot->potion_kind;
                    switch (now_pot->potion_kind)
                    {
                    case 0:
                        all_players[this_sol->owner].remaining_time_potion=4;
                        break;
                    case 1:
                        all_players[this_sol->owner].remaining_time_potion=2;
                        break;
                    case 2:
                        all_players[this_sol->owner].remaining_time_potion=5;
                        break;
                    case 3:
                        all_players[this_sol->owner].remaining_time_potion=3;
                        break;
                    default:
                        break;
                    }
                    
                }
                delete_pot(&now_pot,&beforenow_pot);
                update_score_for_potion();
                flag=0;
                break;
            }
            this_sol=this_sol->next_soldier;   
        }

        if(flag){
        beforenow_pot=now_pot;
        now_pot=now_pot->next_potion;
        }
    }

}

void rendercopy_potion_update(struct soldier * head_sol,struct player * all_players){
    struct potion * this_pot=all_potions_head;
    while (this_pot)
    {
        loc_potion_texture.x=this_pot->x_loc-20;
        loc_potion_texture.y=this_pot->y_loc-20;
        SDL_RenderCopy(my_renderer,potion_texture[this_pot->potion_kind],NULL,&loc_potion_texture);
        this_pot=this_pot->next_potion;
        
    }

    collect_potion(head_sol,all_players);
}


void quit_scoresposions(){
    for (int i = 0; i < 10; i++)
    {
        SDL_DestroyTexture(player_score_texture[i]);
    }
    SDL_DestroyTexture(box);
    box=NULL;
    for (int i = 0; i < 4; i++)
    {
        SDL_DestroyTexture(potion_texture[i]);
    }
    
    struct potion * thispo=all_potions_head;
    struct potion * befpo=NULL;
    while (thispo)
    {
        befpo=thispo;
        thispo=thispo->next_potion;
        free(befpo);
    }
    free(befpo);

    all_potions_head=NULL;
    all_potions_tail=NULL;
    reserved_x=-1;
    reserved_y=-1;
}

void save_load_potion(int save){
    if (save)
    {
        save_potion(all_potions_head);
        return;
    }
    load_potion(&all_potions_head,&all_potions_tail);
}
