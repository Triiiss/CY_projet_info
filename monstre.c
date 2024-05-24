#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define SALLE 30
typedef struct{
    int damage;
    char icon;
    char* name;
}Weapon;

typedef struct{
    float exp;
    int heal;
    char* name;
    char icon;
}Object;

typedef struct 
{
    Weapon weapon_pouch[2];
    Object  use_pouch[4];
}Inventory;

typedef struct{
    int pv;
    int damage;
    int x;
    int y;
    float exp;
    Inventory bag;
}Player;

typedef struct{
    char* name;
    int damage;
    int pv;
    int x;
    int y;
    float exp;
    Weapon reward_wpn;
    Object reward_obj;

}Enemies;

typedef struct{
    int x;              //Position of the bottom right corner x
    int y;              //Position of the bottom right corner y
    int ndoor;          //Number of doors
    //Door* doors;    //List of the different doors
    int width;      //At least 3
    int lenght;     //At least 3
}Room;

void init_weapon(Weapon* wpn){
    /*initialize the Weapon variable with defaults values*/
    wpn->damage=0;
    wpn->icon=0;
    wpn->name=NULL;
}

void init_object(Object* obj){
    /*initialize the Object variable with defaults values*/
    obj->exp=0;
    obj->heal=0;
    obj->icon=0;
    obj->name=NULL;
}

void init_inventory(Inventory* bag){
    /*initialize the Inventory variable with defaults values*/
    for(int i=0;i<2;i++){
        init_object(bag->use_pouch+i);
        init_weapon(bag->weapon_pouch+i);
    }
        for(int i=0;i<2;i++){
        init_object(bag->use_pouch+i+2);
        }
}

void init_player(Player* j1){
    /*initialize the Player variable with defaults values*/
    j1->pv=100;
    j1->damage=10;
    j1->x=0;
    j1->y=0;
    j1->exp=0;
    init_inventory(&(j1->bag));
}

void init_enemies(Enemies* j2){
    /*initialize the Enemies variable with defaults values*/
    j2->pv=100;
    j2->damage=10;
    j2->x=0;
    j2->y=0;
    j2->exp=150;
    init_weapon(&(j2->reward_wpn));
    init_object(&(j2->reward_obj));
}

void take_damage(Player* j1,Enemies*j2){
    j1->pv-=j2->damage;
}

void give_damage(Player* j1,Enemies*j2){
    if(j2->pv-(j1->damage+(j1->bag).weapon_pouch[0].damage)<0){

    }
    else{
        j2->pv-=j1->damage+(j1->bag).weapon_pouch[0].damage;
    }
    
}

int add_obj_bag(Player* j1,Object* obj){
    /*A function that add the Object obj to the Player j1 inventory if successful return 1 else return 0*/
    for(int i=0;i<4;i++){
        if (j1->bag.use_pouch[i].name==NULL){
            j1->bag.use_pouch[i]=*obj;
            free(obj);
            return 1;
        }
    }
    return 0;
}

int add_wpn_bag(Player* j1,Weapon* wpn){
    /*A function that add the Weapon wpn to the Player j1 inventory if successful return 1 else return 0*/
    for(int i=0;i<2;i++){
        if (j1->bag.weapon_pouch[i].name==NULL){
            j1->bag.weapon_pouch[i]=*wpn;
            free(wpn);
            return 1;
        }
    }
    return 0;
}



int déplacement_enemies(Player*j1,Enemies*j2){
    
    
        
        //regarde les coordonnées de l'ennemie et du joueur pour savoir si il peut attaquer. Il faut que le joueur soit proche
        if(((j1->y)>(j2->y)) && (j1->y)<((j2->y)+3) && ((j1->x)==(j2->x))){      
            //appel fonction pour prendre des dégâts
            take_damage(&j1,&j2);                                           
        }
        else if(((j1->y)>(j2->y)) && ((j1->y)<((j2->y)+3)) && ((j1->x)==(j2->x))){       
            take_damage(&j1,&j2);
        }
        else if(((j1->y)>(j2->y)) && (j1->y)<((j2->y)+3) && ((j1->x)==(j2->x))){       
            take_damage(&j1,&j2);
        }
        else if(((j1->x)>(j2->x)) && (j1->x)<((j2->x)+3) && ((j1->y)==(j2->y))){
            take_damage(&j1,&j2);
        }
        //compare les coordonnées de l'enemies avec ce du joueur pour savoir où l'enemies doit se déplacer
        if((j1->y)<(j2->y)){    
            j2->y--;
        }
       
        else if((j1->y)>(j2->y)){
            j2->y++;
        }
        if((j1->x)<(j2->x)){
            j2->x--;
        }
        else if((j1->x)>(j2->x)){
            j2->x++;
        }
    }
}

int main(){
    Player plr;
    Enemies ens;
    Room rm;
    int x=time(NULL);
    int tab[SALLE];
    time_t time=time(NULL);
    time_t time1;
    while(1){
        time1=time(NULL);            //appel la fonction toute les secondes pour faire bouger l'enemie
        if(time1-time>=1){
            déplacement_enemies(plr,ens); 
        }
    }

    return 0;
}
