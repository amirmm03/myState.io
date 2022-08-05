#include"saverandom.h"

FILE * save_info_start_potion;
////////////////////random??????????????????
int random_indexes[20];

void save_game(int mapnum,struct territory * territory_state,int number_of_territories,struct soldier * all_head,int numberofplayers,struct player * allplayers){
    char address[50];
    strcpy(address,path_to_pic);
    strcat(address,"/save.txt");
    FILE * save_info=fopen(address,"w");
    fprintf(save_info,"1\n%d\n",mapnum);
    for (int i = 0; i < number_of_territories; i++)
    {
        fprintf(save_info,"%d %d %d ",territory_state[i].owner,territory_state[i].soldiers,territory_state[i].destination_of_attack);
    }

    int number_of_sildiers=0;
    struct soldier * now_soldier=all_head;
    while (now_soldier)
    {
        number_of_sildiers++;
        now_soldier=now_soldier->next_soldier;
    }
    fprintf(save_info,"\n%d\n",number_of_sildiers);
    while (all_head)
    {
        fprintf(save_info,"%d %d %d ",all_head->now_location.x,all_head->now_location.y,all_head->owner);
        fprintf(save_info,"%d %d %d %d ",all_head->source,all_head->destination,all_head->timeleft,all_head->whole_time);
        fprintf(save_info,"%d %d ",all_head->x_diff,all_head->y_diff);
        all_head=all_head->next_soldier;
    }
    fprintf(save_info,"\n%d\n",numberofplayers);

    for (int i = 0; i < numberofplayers; i++)
    {
        fprintf(save_info,"%d %d ",allplayers[i].score,allplayers[i].number_of_terr);
        fprintf(save_info,"%d %d ",allplayers[i].potion_num,allplayers[i].remaining_time_potion);
    }
    if(mapnum==999){
        strcpy(address,path_to_pic);
        strcat(address,"/save_rand.txt");
        FILE * save_info_rand=fopen(address,"w");
        fprintf(save_info_rand,"%d %d\n",numberofplayers,number_of_territories);
        for (int i = 0; i < number_of_territories; i++)
        {
            fprintf(save_info_rand,"%d ",random_indexes[i]);
        }
        fclose(save_info_rand);
    }

    

    fclose(save_info);
    

}

void load_soldier(struct soldier **all_tail,struct soldier **all_head,FILE * save_info){
    if (*all_head)
    {
        (*all_tail)->next_soldier=malloc(sizeof(struct soldier));
        (*all_tail)=(*all_tail)->next_soldier;
    }else{
    *all_head=malloc(sizeof(struct soldier));
    *all_tail=*all_head;

    }
    (*all_tail)->next_soldier=NULL;
    
    fscanf(save_info," %hi %hi %hi",&(*all_tail)->now_location.x,&(*all_tail)->now_location.y,&(*all_tail)->owner);
    fscanf(save_info," %hi %hi %hi %hi",&(*all_tail)->source,&(*all_tail)->destination,&(*all_tail)->timeleft,&(*all_tail)->whole_time);
    fscanf(save_info," %hi %hi",&(*all_tail)->x_diff,&(*all_tail)->y_diff);
}

void load_game(struct territory * territory_state,int number_of_territories,struct soldier ** all_head,struct soldier ** all_tail,SDL_Texture** territories_texture,struct player * player_info){
    char address[50];
    strcpy(address,path_to_pic);
    strcat(address,"/save.txt");
    FILE * save_info=fopen(address,"r");
    fscanf(save_info," %*d %*d ");
    for (int i = 0; i < number_of_territories; i++)
    {
        fscanf(save_info," %d %d %d",&(territory_state[i].owner),&territory_state[i].soldiers,&territory_state[i].destination_of_attack);
        SDL_SetTextureColorMod(*(territories_texture+i),player_info[territory_state[i].owner].player_color.r,player_info[territory_state[i].owner].player_color.g,player_info[territory_state[i].owner].player_color.b);
    }
    int number_of_soldiers;
    
    fscanf(save_info,"%d",&number_of_soldiers);

    for (int i = 0; i < number_of_soldiers; i++)
    {
        load_soldier(all_tail,all_head,save_info);
    }
    int numberofplayers;
    fscanf(save_info," %d ",&numberofplayers);
    for (int i = 0; i < numberofplayers; i++)
    {
        fscanf(save_info," %d %d",&player_info[i].score,&player_info[i].number_of_terr);
        fscanf(save_info," %d %d",&player_info[i].potion_num,&player_info[i].remaining_time_potion);
    }


    
    save_info_start_potion=save_info;
}



void get_info_savedgame(){
    char address[50];
    strcpy(address,path_to_pic);
    strcat(address,"/save.txt");
    FILE * save_info=fopen(address,"r");
    int state=-1;
    fscanf(save_info,"%d",&state);
    if (state!=1)
    {
        return;
    }
    int map_number;
    fscanf(save_info,"%d",&map_number);
    fclose(save_info);
    play_map(map_number,1);    
}


void save_potion(struct potion * pot_head){
    char address[50];
    strcpy(address,path_to_pic);
    strcat(address,"/save.txt");
    FILE * save_info=fopen(address,"a");
    int number_of_pot=0;
    struct potion * now_pot=pot_head;
    while (now_pot)
    {
        number_of_pot++;
        now_pot=now_pot->next_potion;
    }
    fprintf(save_info,"\n%d\n",number_of_pot);
    while (pot_head)
    {
        fprintf(save_info,"%d %d %d ",pot_head->potion_kind,pot_head->x_loc,pot_head->y_loc);
        pot_head=pot_head->next_potion;
    }

    fclose(save_info);
}

 
void load_one_potion(struct potion **all_tail,struct potion **all_head,FILE * save_info){
    if (*all_head)
    {
        (*all_tail)->next_potion=malloc(sizeof(struct potion));
        (*all_tail)=(*all_tail)->next_potion;
    }else{
    *all_head=malloc(sizeof(struct potion));
    *all_tail=*all_head;

    }
    (*all_tail)->next_potion=NULL;
    
    fscanf(save_info," %d %d %d",&(*all_tail)->potion_kind,&(*all_tail)->x_loc,&(*all_tail)->y_loc);
}

void load_potion(struct potion ** pot_head,struct potion ** pot_tail){
    int number_of_pot;
    fscanf(save_info_start_potion," %d ",&number_of_pot);
    for (int i = 0; i < number_of_pot; i++)
    {
        load_one_potion(pot_tail,pot_head,save_info_start_potion);
    }
    fclose(save_info_start_potion);
}
 //////////////////////////////////random map/////////////////////////////////

int number_of_terr;
int number_of_player;


void choose_number_players(){
    number_of_player=3;
    char* options[4]={"3 players","4 players","5 players","back"};
    number_of_player+=start_menu(4,options);
}

void play_random(){

    choose_number_players();
    if(number_of_player==6)return;
    int ans=choose_number_terrs();
    while ((ans<6)|(ans>18))
    {
        ans=choose_number_terrs();
    }
    number_of_terr=ans;
    play_map(999,0);
}

void fill_random_indexes(int * random_indexes,int number_whole_terr){
    srand(time(0));
    int random_num;
    for (int i = 0; i < number_of_terr; i++)
    {
        int repeat=1;
        while(repeat){
            random_num=rand()%number_whole_terr;
            random_indexes[i]=random_num;
            repeat=0;
            for (int j = 0; j < i; j++)
            {
                if(random_indexes[j]==random_num){
                    repeat=1;
                    break;
                }
            }
        }
    }
    
}
int in_array(int * arr,int num){
    for (int i = 0; i < number_of_terr; i++)
    {
        if (arr[i]==num)
        {
            return 1;
        }
        
    }
    return 0;
}
int random_territories(FILE * info,struct pair * barrack_locations,int is_loading){
    int number_whole_terr;
    int ind=0;
    fscanf(info," %d ",&number_whole_terr);
    if(is_loading==0)fill_random_indexes(random_indexes,number_whole_terr);
    else{
        char address[50];
        strcpy(address,path_to_pic);
        strcat(address,"/save_rand.txt");
        FILE * save_info_rand=fopen(address,"r");
        fscanf(save_info_rand,"%d %d",&number_of_player,&number_of_terr);
        for (int i = 0; i < number_of_terr; i++)
        {
            fscanf(save_info_rand," %d",random_indexes+i);
        }
        fclose(save_info_rand);

    }
    for (int i = 0; i < number_whole_terr; i++)
    {
        if(in_array(random_indexes,i)){
            for (int j = 0; j <number_of_terr ; j++)
            {
                if (random_indexes[j]==i)ind=j;
            }
            
            fscanf(info," %hi %hi",&barrack_locations[ind].x,&barrack_locations[ind].y);
        }else
        {
            fscanf(info,(" %*d %*d"));
        }
    
    }
    
    return number_of_terr;
}

int * get_random_indexes(){
    return random_indexes;   
}

void random_location(SDL_Rect* location,int index_terr,struct pair * barrack_loc){
    int terr_per_row;
    if(number_of_terr<=9){terr_per_row=3;}
    else if(number_of_terr<=16){terr_per_row=4;}
    else {terr_per_row=5;}
    int x,y;
    
    int max_len=max(location->h,location->w);
    if(max_len>310){
        float zarib=310.0/max_len;
        barrack_loc->x*=zarib;
        barrack_loc->y*=zarib;
        location->h*=zarib;
        location->w*=zarib;
    }

    x=index_terr%terr_per_row;
    y=index_terr/terr_per_row;
    if(x==0&&y==0){
        location->x=(rand()%200);
        location->y=50;
    }
    else if (x==0){
        location->x=(rand()%200);
        location->y=(location-terr_per_row)->y+(location-terr_per_row)->h-(rand()%70);
    }
    else if(y==0){
        location->x=(location-1)->x+(location-1)->w+70;
        location->y=50-(rand()%50);
    }else
    {
        location->x=(location-terr_per_row)->x-(rand()%70);
        location->y=(location-terr_per_row)->y+(location-terr_per_row)->h-(rand()%70);
    }

    if(index_terr==16){
        (location-12)->x=(location-terr_per_row+2)->x-(rand()%70);
        (location-12)->y=(location-terr_per_row+2)->y+(location-terr_per_row+2)->h-(rand()%70);
    }
    
}

int get_number_player_rand(){
    return number_of_player;
}

int random_start_terr(struct territory * all_terrs){
    int ans=rand()%number_of_terr+1;
    int repeat=1;
    while (repeat)
    {
        repeat=0;
        for (int i = 0; i < number_of_terr; i++)
        {
            if(all_terrs[ans-1].owner!=9){
                repeat=1;
                ans=rand()%number_of_terr+1;
            }
        }
        
    }
    return ans;

}
