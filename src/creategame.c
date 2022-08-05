#include"creategame.h"

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
} 
void itoa(int n, char s[],int nothing)
{
nothing=1;
    int i, sign;

    if ((sign = n) < 0)  
        n = -n;          
    i = 0;
    do {   
        s[i++] = n % 10 + '0'; 
    } while ((n /= 10) > 0);     
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
} 


int mypull_event();


FILE * info;
int number_of_territories;
struct pair barrack_locations[20];
struct territory territory_state[20];
struct pair attack_fromto;
TTF_Font * font;
SDL_Texture* departue_text_texture;
SDL_Rect loc_of_departue_text;
struct soldier * all_soldiers_head=NULL;
struct soldier * all_soldiers_tail=NULL;
int number_of_players;
SDL_Texture * texture_of_soldiers[10];
struct player player_info[10];
int soldier_movement_speed=5;
SDL_Texture ** territories_texture;
SDL_Texture* neutral_terrs;
void quit_game(){
    TTF_CloseFont(font);
    SDL_DestroyTexture(departue_text_texture);
    for (int i = 0; i < number_of_players; i++)
    {
        SDL_DestroyTexture(texture_of_soldiers[i]);
    }
    for (int i = 0; i < number_of_territories; i++)
    {
        SDL_DestroyTexture(territories_texture[i]);
    }
    free(territories_texture);
    all_soldiers_tail=all_soldiers_head;
    while (all_soldiers_head)
    {
        all_soldiers_head=all_soldiers_head->next_soldier;
        free(all_soldiers_tail);
        all_soldiers_tail=all_soldiers_head;
    }

    SDL_DestroyTexture(neutral_terrs);
}


void create_texture_soldier(){
    char path_to_s_img[50]="";
    strcat(path_to_s_img,path_to_pic);
    strcat(path_to_s_img,"/soldier.png");
    for (int i = 0; i < number_of_players; i++)
    {
        texture_of_soldiers[i]=IMG_LoadTexture(my_renderer,path_to_s_img);
        SDL_SetTextureColorMod(texture_of_soldiers[i],player_info[i].player_color.r/1.4,player_info[i].player_color.g/1.4,player_info[i].player_color.b/1.4);
    }
}


float distance(struct pair * a,struct pair * b){
    float ans=(a->x-b->x)*(a->x-b->x)+(a->y-b->y)*(a->y-b->y);
    ans=sqrt(ans);
    return ans;
}

void callculate_def(int order){
    int thisx=(barrack_locations[all_soldiers_tail->source].y-barrack_locations[all_soldiers_tail->destination].y);
    int thisy=(barrack_locations[all_soldiers_tail->source].x-barrack_locations[all_soldiers_tail->destination].x);
    float zarib=10.0/sqrt(thisx*thisx+thisy*thisy);
    all_soldiers_tail->x_diff=-thisx*zarib*(order-2);
    all_soldiers_tail->y_diff=thisy*zarib*(order-2);
}

void create_soldier(int terr_index,int order_of_soldier){
    if(all_soldiers_tail){
        all_soldiers_tail->next_soldier=malloc(sizeof(struct soldier));
        all_soldiers_tail=all_soldiers_tail->next_soldier;
    }else{
        all_soldiers_head=malloc(sizeof(struct soldier));
        all_soldiers_tail=all_soldiers_head;
    }
    all_soldiers_tail->owner=territory_state[terr_index].owner;
    all_soldiers_tail->source=terr_index;
    all_soldiers_tail->destination=territory_state[terr_index].destination_of_attack;
    callculate_def(order_of_soldier);
    all_soldiers_tail->now_location.x=(barrack_locations+terr_index)->x-5+all_soldiers_tail->x_diff;
    all_soldiers_tail->now_location.y=(barrack_locations+terr_index)->y-5+all_soldiers_tail->y_diff;
    all_soldiers_tail->timeleft=(int)distance(&(all_soldiers_tail->now_location),barrack_locations+all_soldiers_tail->destination)/soldier_movement_speed;
    all_soldiers_tail->whole_time=all_soldiers_tail->timeleft;
    all_soldiers_tail->next_soldier=NULL;   
}

void update_start_attack(int terr_index){
    
    if (territory_state[terr_index].destination_of_attack!=-1)
    {
        if(territory_state[terr_index].soldiers>4){
        territory_state[terr_index].soldiers-=5;
        create_soldier(terr_index,0);
        create_soldier(terr_index,1);
        create_soldier(terr_index,2);
        create_soldier(terr_index,3);
        create_soldier(terr_index,4);
        return;
        }
    
        for (int i = 0; i < territory_state[terr_index].soldiers; i++)
        {
            create_soldier(terr_index,i);
        }
        
        territory_state[terr_index].soldiers=0;
        territory_state[terr_index].destination_of_attack=-1;
        

    }
    return;
}

void delete_soldier(struct soldier ** this_soldier,struct soldier ** before_soldier){
    if (*before_soldier==NULL){
        if ((*this_soldier)->next_soldier==NULL)
        {
            all_soldiers_tail=NULL;
        }
        *before_soldier=*this_soldier;
        *this_soldier=(*this_soldier)->next_soldier;
        free(*before_soldier);
        *before_soldier=NULL;
        all_soldiers_head=*this_soldier;
        return;
    }
    if ((*this_soldier)->next_soldier==NULL)
    {
        all_soldiers_tail=*before_soldier;
    }
    (*before_soldier)->next_soldier=(*this_soldier)->next_soldier;    
    free(*this_soldier);
    (*this_soldier)=(*before_soldier)->next_soldier;
}


void departure_text(int fors){
    SDL_Color textcolor={16, 134, 212,255};
    SDL_Surface * surface_oftext;
    if (fors)
    {
        char num[4];
        itoa(attack_fromto.x+1,num,10);
        char txt[40]="move from ";
        strcat(txt,num);
        strcat(txt," to ?");
        surface_oftext = TTF_RenderText_Solid(font,txt,textcolor);        
    }else{
    surface_oftext = TTF_RenderText_Solid(font,"move from ? to ...",textcolor);}
    departue_text_texture = SDL_CreateTextureFromSurface(my_renderer, surface_oftext);
   
    loc_of_departue_text.x=1620;
    loc_of_departue_text.y=100;
    loc_of_departue_text.h=surface_oftext->h;
    loc_of_departue_text.w=surface_oftext->w;
    SDL_FreeSurface(surface_oftext);
    
}


void update_soldiers(){
    
    struct soldier * this_soldier=all_soldiers_head;
    struct soldier * before_soldier=NULL;
    SDL_Rect this_soldier_loc={1,1,15,15};
    
    int able_to_move=-1;/////potion 1 effect
    for (int i = 0; i < number_of_players; i++)
    {
        if (player_info[i].potion_num==1)
        {
            able_to_move=i;
            break;
        }
    }
    
    while(this_soldier)
    {
        this_soldier_loc.x=this_soldier->now_location.x;
        this_soldier_loc.y=this_soldier->now_location.y;
        SDL_RenderCopy(my_renderer,texture_of_soldiers[this_soldier->owner],NULL,&this_soldier_loc);
        if (this_soldier->timeleft==0)
        {
            if(territory_state[this_soldier->destination].owner==this_soldier->owner)
            {
                territory_state[this_soldier->destination].soldiers+=1;
            }else if(territory_state[this_soldier->destination].soldiers==0){
                if (territory_state[this_soldier->destination].owner==0&&attack_fromto.x==this_soldier->destination)
                {
                    attack_fromto.x=-1;
                    departure_text(0);
                }
                player_info[territory_state[this_soldier->destination].owner].score-=1;
                player_info[territory_state[this_soldier->destination].owner].number_of_terr--;
                territory_state[this_soldier->destination].owner=this_soldier->owner;
                territory_state[this_soldier->destination].soldiers=1;
                player_info[this_soldier->owner].number_of_terr+=1;
                player_info[this_soldier->owner].score+=2;
                SDL_SetTextureColorMod(*(territories_texture+this_soldier->destination),player_info[this_soldier->owner].player_color.r,player_info[this_soldier->owner].player_color.g,player_info[this_soldier->owner].player_color.b);
                update_score(number_of_players,player_info);

            }else{
                territory_state[this_soldier->destination].soldiers-=1;
            }

            delete_soldier(&this_soldier,&before_soldier);
            continue;
        }
        ///////////potion 1 effect
        if((able_to_move<0)|(this_soldier->owner==able_to_move)){
            ///////////potion 0 effect
            if ((player_info[this_soldier->owner].potion_num==0)&(this_soldier->timeleft>1))
            {
                this_soldier->timeleft-=1;
            }
            
            this_soldier->timeleft-=1;
            this_soldier->now_location.x=(((barrack_locations+this_soldier->source)->x-5+this_soldier->x_diff)*(this_soldier->timeleft) + (this_soldier->whole_time-this_soldier->timeleft) *((barrack_locations+this_soldier->destination)->x-5+this_soldier->x_diff)) /this_soldier->whole_time;
            this_soldier->now_location.y=(((barrack_locations+this_soldier->source)->y-5+this_soldier->y_diff)*(this_soldier->timeleft) + (this_soldier->whole_time-this_soldier->timeleft) *((barrack_locations+this_soldier->destination)->y-5+this_soldier->y_diff)) /this_soldier->whole_time;
        }

        before_soldier=this_soldier;
        this_soldier=this_soldier->next_soldier;
        
    }

}

void show_numberof_soldiers(struct territory this_territory,int xofcir,int yofcir){
    SDL_Color textcolor={255,255,255,255};
    char mytext[5];
    itoa(this_territory.soldiers,mytext,10);
    SDL_Surface * surface_oftext = TTF_RenderText_Solid(font,mytext,textcolor);
    SDL_Texture * texture_oftext = SDL_CreateTextureFromSurface(my_renderer, surface_oftext);
    SDL_Rect loc_of_num;
    loc_of_num.x=xofcir-10;
    loc_of_num.y=yofcir+13;
    loc_of_num.h=surface_oftext->h;
    loc_of_num.w=surface_oftext->w;
    SDL_FreeSurface(surface_oftext);
    SDL_RenderCopy(my_renderer, texture_oftext, NULL, &loc_of_num);
    SDL_DestroyTexture(texture_oftext);
    return;
}
int findnumberofterritories(char * path_to_map,struct pair* barrack_locations,int map_number,int is_loading){
    char fontpath[100];
    strcpy(fontpath,path_to_pic);
    strcat(fontpath,"fonts/JungleAdventurer.ttf");
    font = TTF_OpenFont(fontpath, 23);



    char path_to_map_info[130];
    strcpy(path_to_map_info,path_to_map);
    strcat(path_to_map_info,"info.txt");    
    info=fopen(path_to_map_info,"r");
    int ans;
    if(map_number!=999){
        fscanf(info," %d ",&ans);
        for (int i = 0; i < ans; i++)
        {
            fscanf(info," %hi %hi",&barrack_locations[i].x,&barrack_locations[i].y);
        }
        return ans;
    }else
    {
        return random_territories(info,barrack_locations,is_loading);
    }
    
    
}


SDL_Texture * create_territory_img( SDL_Rect* location,const char* name,char* pathtomap,SDL_Color *terr_color,int map_num,int index_terr){
    char imgpath[100];
    strcpy(imgpath,pathtomap);
    strcat(imgpath,name);
    strcat(imgpath,".png");
    SDL_Surface * terimage = IMG_Load(imgpath);
    SDL_Texture * texture_terr = SDL_CreateTextureFromSurface(my_renderer, terimage);
    location->h=terimage->h;
    location->w=terimage->w;
    if(map_num==999){
        random_location(location,index_terr,barrack_locations+index_terr);
    }else{
        location->x=Width_of_scr/2-location->w/2-100;
        location->y=Height_of_scr/2-location->h/2;
    }
    SDL_FreeSurface(terimage);
    SDL_SetTextureColorMod(texture_terr,terr_color->r,terr_color->g,terr_color->b);
    return texture_terr;
}


SDL_Texture** create_map(int map_number,SDL_Rect *map_location,struct pair* barrack_locations,int is_loading){
    char path_to_map[130]="";
    strcat(path_to_map,path_to_pic);
    strcat(path_to_map,"map");
    itoa(map_number,path_to_map+strlen(path_to_map),10);
    strcat(path_to_map,"/");
    number_of_territories=findnumberofterritories(path_to_map,barrack_locations,map_number,is_loading);
    SDL_Texture** territory_texture=malloc(sizeof(SDL_Texture*)*20);
    
    SDL_Color startcolor={167, 171, 175,255};
    int* random_indexes=get_random_indexes();
    for (int i = 0; i <number_of_territories ; i++)
    {
        char terrname[5]="";
        if(map_number==999) itoa(random_indexes[i]+1,terrname,10);
        else  itoa(i+1,terrname,10);
        territory_texture[i]=create_territory_img(map_location+i,terrname,path_to_map,&startcolor,map_number,i);
    }
    if (map_number!=999)
    {
        strcat(path_to_map,"neutral.png");
        SDL_Surface * terimage = IMG_Load(path_to_map);
        neutral_terrs = SDL_CreateTextureFromSurface(my_renderer, terimage);
        SDL_FreeSurface(terimage);
    }
    
    return territory_texture;
   
}
void get_info(int is_random){

    for (int i = 0; i < 20; i++)
    {
        territory_state[i].owner=9;
        territory_state[i].soldiers=15;
        territory_state[i].destination_of_attack=-1;
    }
    player_info[9].player_color.r=167; 
    player_info[9].player_color.g=171;
    player_info[9].player_color.b=175;


    fscanf(info,"%d",&number_of_players);
    if(is_random)number_of_players=get_number_player_rand();
    
    for (int i = 0; i < number_of_players; i++)
    {
        int tmp,r,g,b;

        
        fscanf(info,"%d %d %d %d %s",&tmp,&r,&g,&b,player_info[i].name);
        if(is_random)tmp=random_start_terr(territory_state);
        
        player_info[i].player_color.r=r;
        player_info[i].player_color.g=g;
        player_info[i].player_color.b=b;
    
        player_info[i].number_of_terr=1;
        player_info[i].potion_num =-1;
        player_info[i].player_color.a=255;
        territory_state[tmp-1].owner=i;
        territory_state[tmp-1].soldiers=50;
        
        SDL_SetTextureColorMod(*(territories_texture+tmp-1),r,g,b);
    }
    fclose(info);
    attack_fromto.x=-1;
    attack_fromto.y=-1;
    create_texture_soldier();
    return;
}

int choose_barrack(int mousex,int mousey){
    for (int i = 0; i < number_of_territories; i++)
    {
        if ((abs(barrack_locations[i].x-mousex)+abs(barrack_locations[i].y-mousey))<70)
        {
            return i;
        }
        
    }
    return -1;
}

int find_closest_enemy_no_team(int terr_index){
    int closest_index=-1;
    int closest_distance=250*250;
    for (int i = 0; i < number_of_territories; i++)
    {
        if (territory_state[i].owner==9&&distance(barrack_locations+terr_index,barrack_locations+ i)<closest_distance)
        {
            closest_distance=distance(barrack_locations+terr_index,barrack_locations+ i);
            closest_index=i;
        }
    }
    
    return closest_index;
}


int find_closest_enemy_with_team(int this_owner,int terr_index){
    int closest_index=-1;
    int closest_distance=250*250;
    for (int i = 0; i < number_of_territories; i++)
    {
        if (territory_state[i].owner!=this_owner&&distance(barrack_locations+terr_index,barrack_locations+ i)<closest_distance)
        {
            closest_distance=distance(barrack_locations+terr_index,barrack_locations+ i);
            closest_index=i;
        }
        
    }
    return closest_index; 
}


int find_weakest_enemy(int this_owner,int terr_index){
    int weakest_index=-1;
    for (int i = 0; i < number_of_territories; i++)
    {
        if ((territory_state[i].owner!=this_owner)&&(territory_state[i].destination_of_attack>=0)&&(territory_state[terr_index].soldiers>=40)&&(territory_state[i].soldiers<=50))/////////limit for attack
        {
            weakest_index=i;
        }
    }
    return weakest_index;
}

void enemy_play(){
    for (int i = 0; i < number_of_territories; i++)
    {
        if (territory_state[i].owner<9&&territory_state[i].owner>0){
            int closestwithteam=find_closest_enemy_with_team(territory_state[i].owner,i);
            int closestnoteam=find_closest_enemy_no_team(i);
            int weakestenemy=find_weakest_enemy(territory_state[i].owner,i);
            int destination_atk=-1;
            if (territory_state[i].soldiers>=100)
            {
                destination_atk=closestnoteam;
                if (destination_atk<0)
                {
                   destination_atk=weakestenemy;
                }
                if (destination_atk<0)
                {
                    destination_atk=closestwithteam;
                }
                territory_state[i].destination_of_attack=destination_atk;
            }else if(closestnoteam>=0&&territory_state[i].soldiers>30){
                territory_state[i].destination_of_attack=closestnoteam;
            }else if(weakestenemy>=0){

                territory_state[i].destination_of_attack=weakestenemy;
            }



        }
        
    }
    return;
}

int lost(){
    int flag=1;
    for (int i = 0; i < number_of_territories; i++)
    {
        if (territory_state[i].owner==0)
        {
            flag=0;
        }
        
    }
    return flag;
}

int win(){
    int flag=1;
    for (int i = 0; i < number_of_territories; i++)
    {
        if (territory_state[i].owner>0&&territory_state[i].owner<9)
        {
            flag=0;
        }
        
    }
    return flag;
}

int check_win_lose(){
    if(lost())return -1;
    if (win())return 1;
    return 0;
}


void check_collision(){
    struct soldier* now=all_soldiers_head;
    struct soldier* beforenow=NULL;
    struct soldier* then;
    struct soldier* beforethen;
    int flag=1;
    while (now)
    {
        
        flag=1;
        then=now->next_soldier;
        beforethen=now;
      
        while (then)
        {
          
            if (now->owner!=then->owner&&10>distance(&now->now_location,&then->now_location))
            {
                     
                if (beforethen==now)
                {
                    beforethen=beforenow;
                }
                
                delete_soldier(&now,&beforenow);
                delete_soldier(&then,&beforethen);
                flag=0;
                break;
            }
            
         
            beforethen=then;
            then=then->next_soldier;
           
        }
            
        if(flag){
        beforenow=now;
        now=now->next_soldier;
        }
  
    }
    
}



void play_map(int map_number,int savestate){
    int memcheck=0;
    if(map_number==4){
        map_number=1;
        memcheck=1;
    }
    SDL_Rect map_location[20];
    territories_texture=create_map(map_number,map_location, barrack_locations,savestate);
    for (int i = 0; i < number_of_territories; i++)
    {
        barrack_locations[i].x+=map_location[i].x;
        barrack_locations[i].y+=map_location[i].y;
    }
  
    get_info(map_number==999);
    if(savestate){
        load_game(territory_state,number_of_territories,&all_soldiers_head,&all_soldiers_tail,territories_texture,player_info);
        save_load_potion(0);
    } 
    
    int tanavob=0;
    int pause_state;
    SDL_SetRenderDrawColor( my_renderer, 180, 225, 255, 255);
    departure_text(0);

    if(memcheck){
        for (int i = 0; i < number_of_territories; i++)
        {
            territory_state[i].owner=0;
            SDL_SetTextureColorMod(*(territories_texture+i),player_info[0].player_color.r,player_info[0].player_color.g,player_info[0].player_color.b);
        }
        territory_state[0].owner=1;
        SDL_SetTextureColorMod(*(territories_texture),player_info[1].player_color.r,player_info[1].player_color.g,player_info[1].player_color.b);
        player_info[0].number_of_terr=number_of_territories-1;
    }
    
    strcpy(player_info->name,myname);
    if(savestate==0) create_score_texture(number_of_players,player_info,0);
    update_score(number_of_players,player_info);
    init_potion();
    
    
    while (1)
    {   

        SDL_RenderClear( my_renderer );
        if(map_number!=999)SDL_RenderCopy(my_renderer,neutral_terrs,NULL,map_location);
        check_collision();
        
        for (int i = 0; i < number_of_territories; i++)
        {
            SDL_RenderCopy(my_renderer,*(territories_texture+i),NULL,map_location+i);
            show_numberof_soldiers(territory_state[i],barrack_locations[i].x,barrack_locations[i].y);
        }
       
        SDL_RenderCopy(my_renderer, departue_text_texture, NULL, &loc_of_departue_text);
        
        rendercopy_potion_update(all_soldiers_head,player_info);
        update_soldiers();
    
        show_score(number_of_players);
        SDL_RenderPresent(my_renderer);

        if (tanavob%5==0){
       for (int i = 0; i < number_of_territories; i++)
       {
           update_start_attack(i);////potion 2 effect
           if((territory_state[i].owner!=9)&((territory_state[i].soldiers<100)|(player_info[territory_state[i].owner].potion_num==2)))
           territory_state[i].soldiers+=1;
           ////potion 3 effect
           if((player_info[territory_state[i].owner].potion_num==3)&(territory_state[i].soldiers<100))territory_state[i].soldiers+=1;
       }
       }

        if (tanavob%10==0)
        {
            enemy_play();
            
        }

        if (tanavob==0)
        {
            create_potion(all_soldiers_head,player_info);
            int check_state=check_win_lose();
            if (check_state==-1)
            {
                rewrite_scores(number_of_players,player_info);
                quit_game();
                quit_scoresposions();
                fin_graphic(0);
                break;
            }
            if (check_state==1)
            {
                rewrite_scores(number_of_players,player_info);
                quit_game();
                quit_scoresposions();
                fin_graphic(1);
                break;
            }
        }
        

       tanavob+=1;
       tanavob=tanavob%60;

        SDL_Delay(fps);
        switch (mypull_event())
        {
        case 1:
            break;
        case 0:
            pause_state=pausegame_menu();
            if (pause_state==0){

                SDL_SetRenderDrawColor( my_renderer, 163, 218, 255, 255 );
                SDL_RenderClear( my_renderer );
                break;

            }if (pause_state==1)
            {
                save_game(map_number,territory_state,number_of_territories,all_soldiers_head,number_of_players,player_info);
                save_load_potion(1);
                SDL_SetRenderDrawColor( my_renderer, 163, 218, 255, 255 );
                SDL_RenderClear( my_renderer );
                break;
            }
            quit_scoresposions();
            quit_game();
            return;
            
            break;
        default:
            break;
        }

    }

}

int mypull_event(){

        while (SDL_PollEvent( sdl_events) != 0){
            switch (sdl_events->type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if(sdl_events->button.button==SDL_BUTTON_LEFT){
                    int x_mouse = sdl_events->motion.x;
                    int y_mouse = sdl_events->motion.y;
                    int choosen_barrack=choose_barrack(x_mouse,y_mouse);
                    if(choosen_barrack>=0){
                        if (attack_fromto.x==-1)
                        {
                            if(territory_state[choosen_barrack].owner==0){
                            attack_fromto.x=choosen_barrack;
                            departure_text(1);
                            }
                            return 1;//////do n0thing
                        }else{
                            if (attack_fromto.x==choosen_barrack)
                            {
                                attack_fromto.x=-1;
                                departure_text(0);
                                return 1;//////do n0thing
                            }
                            attack_fromto.y=choosen_barrack;
                            departure_text(0);
                            territory_state[attack_fromto.x].destination_of_attack=attack_fromto.y;
                            attack_fromto.x=-1;
                            attack_fromto.y=-1;
                            return 1;
                        }
                        
                    }
                } else if(sdl_events->button.button==SDL_BUTTON_RIGHT){
                        attack_fromto.x=-1;
                        departure_text(0);
                        return 1;
                }
                break;
            case SDL_KEYDOWN:
                if (sdl_events->key.keysym.sym==SDLK_ESCAPE)
                {
                    return 0;////pause
                }
                break;
                
            default:
                break;
            }

        }
        return 1;////do nothing
}

void choose_predefined_map(){
    char *options[5]={"europe : WW2 (easy)","middle east (medium)","south america (hard)","memory check","back"};
    int state_of_map_menu;
    while (1)
    {
        state_of_map_menu=start_menu(5,options);
        switch (state_of_map_menu)
        {
        case 0:
            play_map(1,0);
            break;
        case 1:
            play_map(2,0);
            break;
        case 2:
            play_map(3,0);
            break;
        case 3:
            play_map(4,0);
            break;
        case 4:
            return;
        default:
            break;
        }
        
    }   

}
void kind_of_map_menu(){
    char *options[3]={"select predefined map","create random map","back"};
    int state_of_map_menu;
    while (1)
    {
        state_of_map_menu=start_menu(3,options);
        switch (state_of_map_menu)
        {
        case 0:
            choose_predefined_map();
            break;
        case 1:
            play_random();
            break;
        case 2:
            return;
        default:
            break;
        }
        
    }
    
}
void update_score_for_potion(){
    update_score(number_of_players,player_info);
}
