#include <ncurses.h>  // library that allows to move with arrow keys (need to check docstring)
#include <curses.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

typedef struct{
    char icon;
    int damage;
    char* name;
}Weapon;

typedef struct{
    int exp;
    int heal;
    int x;
    int y;
    char icon;
    int pickup;
}Object;

typedef struct{
    Weapon weaponPouch[2];
    Object use_pouch[4];
}Invent;

typedef struct{
    Invent bag;           // Inventory, contains object
    int x;              // Character's position on the x axis
    int y;              // Character's position on the y axis
    int pv;             // Health, must be above 0
    int exp;            // Experience points
    int tasksDone;
    int timer;
    int damage;
    int hold;
}Character;

typedef struct{
    int x;
    int y;
    int wall;       // Which wall it is on 0:up ; 1:right ; 2:down ; 3:left
    int open;       // 1 if open, 0 if closed
}Door;

typedef struct{
    int x;
    int y;
    int done;
}Task;

typedef struct{
    int pv;
    int damage;
}Monsters;

typedef struct{
    Door* doors;    // Lists of all the doors
    Object object;     // Max 1 obj per room
    Task tasks;     // Max 1 task per room
    int x;          // Bottom left corner's position horizontal
    int y;          // Bottom left corner's position vertical
    int H;          // Height (vertical)
    int L;          // Lenght (horizontal)
    int n_doors;    // Number of doors
}Room;

typedef struct{
    Room* rooms;            // List of all the rooms
    int n_tasks;            // Number of tasks
    int n_tasks2create;
    int total_rooms;        // Total number of rooms
    int nr_created;         // Number of rooms created so far
    int nd_left;            // Number of rooms left to offer (How many doors left to be created)
}Dungeon;

int strequal(char* str1,char* str2){
    int size=strlen(str1);
    if(strlen(str2)!=size){
        return 0;
    }
    for(int i=0;i<size;i++){
        if(str1[i]!=str2[i]){
            return 0;
        }
    }
    
    return 1;
}

int NewSave(Dungeon spacestation, Character player,char* name,int seed,time_t timer){
    FILE* fp=NULL;
    
    fp=fopen("saves.txt","a");
    if (fp == NULL){
        return 2;
    }
    fseek(fp, 0, SEEK_END);

        fprintf(fp,"{%s\n",name);
        fprintf(fp,"%d\n",seed);
        fprintf(fp,"%d\n",player.x);
        fprintf(fp,"%d\n",player.y);
        fprintf(fp,"%d\n",player.pv);
        fprintf(fp,"%d\n",player.exp);
        fprintf(fp,"%d\n",player.tasksDone);

        fprintf(fp,"%ld\n",timer-time(NULL)+player.timer);

        fprintf(fp,"%d\n",spacestation.n_tasks);
        fprintf(fp,"%d\n",spacestation.total_rooms);
        fprintf(fp,"%d\n",spacestation.nr_created);
        fprintf(fp,"%d\n",spacestation.nd_left);

        for (int i=0;i<spacestation.nr_created;i++){
            fprintf(fp,"%d\n",spacestation.rooms[i].x);
            fprintf(fp,"%d\n",spacestation.rooms[i].y);
            fprintf(fp,"%d\n",spacestation.rooms[i].H);
            fprintf(fp,"%d\n",spacestation.rooms[i].L);
            fprintf(fp,"%d\n",spacestation.rooms[i].tasks.x);
            fprintf(fp,"%d\n",spacestation.rooms[i].tasks.y);
            fprintf(fp,"%d\n",spacestation.rooms[i].tasks.done);

            fprintf(fp,"%d\n",spacestation.rooms[i].n_doors);
            for (int j=0;j<spacestation.rooms[i].n_doors;j++){
                fprintf(fp,"%d\n",spacestation.rooms[i].doors[j].x);
                fprintf(fp,"%d\n",spacestation.rooms[i].doors[j].y);
                fprintf(fp,"%d\n",spacestation.rooms[i].doors[j].wall);
                fprintf(fp,"%d\n",spacestation.rooms[i].doors[j].open);
            }
        }
        fprintf(fp,"}\n");
        fclose(fp);
        return 0;
    fclose(fp);

    return -2;
    
}


int SearchSave(char* name){
    FILE* fp=NULL;
    int cursor=0;
    int size=0;
    size=strlen(name);
    char c=0;
    fp=fopen("saves.txt","r");
    if (fp == NULL){
        return 2;
    }

    fseek(fp,0,SEEK_SET);
    
    while(c!=EOF){
        if(c==name[cursor]){
            cursor++;
            if(cursor>=size || (cursor+1 == size && name[cursor] == '\n')){
                return ftell(fp);
            }
        }
        else{
            cursor=0;
        }
        c=fgetc(fp);
    }
    return -1;
}

/*
int GetSave(Dungeon* spacestation,Character* j,char* name,int* seed){
    FILE *fp;

    if(SearchSave(name)!=-1){
        fp=fopen("saves.txt","r+");
        fseek(fp,SearchSave(name),SEEK_SET);
        fscanf(fp,"%s",name);
        fscanf(fp,"%d",seed);
        fscanf(fp,"%d",&j->x);
        fscanf(fp,"%d",&j->y);
        fscanf(fp,"%d",&j->pv);
        fscanf(fp,"%d",&j->exp);
        fscanf(fp,"%d",&j->tasksDone);

        fscanf(fp,"%d",&j->timer);

        fscanf(fp,"%d",&spacestation->n_tasks);
        fscanf(fp,"%d",&spacestation->total_rooms);
        fscanf(fp,"%d",&spacestation->nr_created);
        fscanf(fp,"%d",&spacestation->nd_left);

        for (int i;i<spacestation->nr_created;i++){
            fscanf(fp,"%d",&spacestation->rooms[i].x);
            fscanf(fp,"%d",&spacestation->rooms[i].y);
            fscanf(fp,"%d",&spacestation->rooms[i].H);
            fscanf(fp,"%d",&spacestation->rooms[i].L);
            fscanf(fp,"%d",&spacestation->rooms[i].tasks.x);
            fscanf(fp,"%d",&spacestation->rooms[i].tasks.y);
            fscanf(fp,"%d",&spacestation->rooms[i].tasks.done);
            fscanf(fp,"%d",&spacestation->rooms[i].n_doors);
            for (int j=0;j<spacestation->rooms[i].n_doors;j++){
                fscanf(fp,"%d",&spacestation->rooms[i].doors[j].x);
                fscanf(fp,"%d",&spacestation->rooms[i].doors[j].y);
                fscanf(fp,"%d",&spacestation->rooms[i].doors[j].wall);
                fscanf(fp,"%d",&spacestation->rooms[i].doors[j].open);
            }
        }
        fclose(fp);
        return 1;
    }
    return -1;
}*/

int getname(char** tab){
    FILE* fp;
    char c;
    char* nom;
    int size=0;
    int indice;
    fp=fopen("saves.txt","r");
    c=fgetc(fp);
    while(c!=EOF){
        if(c=='{'){
            c=fgetc(fp);
            nom=malloc(21*sizeof(char));
            indice=0;
            do{
                nom[indice]=c;
                indice++;
                c=fgetc(fp);    
            }while(c!='\n');
            tab[size]=nom;
            size++;
        }

        c=fgetc(fp);
    }
    fclose(fp);
    return size;
}

int UpdateSave(Dungeon spacestation,Character j1,char* name,int seed,time_t start, time_t pause){
    /*save the parameter in the file save.txt if the name does not exist return -1*/
    char** tab=NULL;
    char vide[20];
    int nbsave=0,flag=0,j=0;

    tab=malloc(4*sizeof(char*));
    for(int i=0;i<4;i++){
        tab[i]=NULL;
    }
    FILE* fp=NULL;
    Character* playertab=NULL;
    int* seedtab=NULL;
    Dungeon* spacestationtab=NULL;

    fp=fopen("saves.txt","a");
    if(fp==NULL){
        return 2;
    }
    
    if(SearchSave(name)==-1){
        return -2;
    }
    
    fclose(fp);

    nbsave=getname(tab);
    spacestationtab=malloc((nbsave-1)*sizeof(Dungeon));
    seedtab=malloc((nbsave-1)*sizeof(int));
    playertab=malloc((nbsave-1)*sizeof(Character));
    
    if (spacestationtab == NULL || seedtab == NULL || playertab == NULL){
        return 2;
    }

    fp=fopen("saves.txt","r");

    int c=0;
    for(int i=0;i<nbsave;i++){
        if(strequal(tab[i],name)!=1){
            fseek(fp,SearchSave(tab[i])+1,SEEK_CUR);
            fscanf(fp,"%s",vide);
            fscanf(fp,"%d",&(seedtab[j]));
            fscanf(fp,"%d",&(playertab[j].x));
            fscanf(fp,"%d",&(playertab[j].y));
            fscanf(fp,"%d",&(playertab[j].pv));
            fscanf(fp,"%d",&(playertab[j].exp));
            fscanf(fp,"%d",&(playertab[j].tasksDone));
            fscanf(fp,"%d",&(playertab[j].timer));

            fscanf(fp,"%d",&(spacestationtab[j].n_tasks));
            fscanf(fp,"%d",&(spacestationtab[j].total_rooms));
            fscanf(fp,"%d",&(spacestationtab[j].nr_created));
            fscanf(fp,"%d",&(spacestationtab[j].nd_left));

            spacestationtab[j].rooms = NULL;
            spacestationtab[j].rooms = malloc(spacestationtab[j].nr_created*sizeof(Room));
            if(spacestationtab[j].rooms == NULL){
                return 2;
            }

            for (int k=0;k<spacestationtab[j].nr_created;k++){
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].x));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].y));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].H));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].L));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].tasks.x));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].tasks.y));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].tasks.done));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].n_doors));

                spacestationtab[j].rooms[k].doors = NULL;
                spacestationtab[j].rooms[k].doors = malloc(sizeof(Door)*(spacestationtab+j)->rooms[k].n_doors);
                if (spacestationtab[j].rooms[k].doors == NULL){
                    return 2;
                }

                for (int l=0;l<spacestationtab[j].rooms[k].n_doors;l++){
                    fscanf(fp,"%d",&(spacestationtab[j].rooms[k].doors[l].x));
                    fscanf(fp,"%d",&(spacestationtab[j].rooms[k].doors[l].y));
                    fscanf(fp,"%d",&(spacestationtab[j].rooms[k].doors[l].wall));
                    fscanf(fp,"%d",&(spacestationtab[j].rooms[k].doors[l].open));
                }
            }
        j++;
        }
    }

    fclose(fp);
    
    fp=fopen("saves.txt","w+");

    fprintf(fp,"{%s\n",name);
    fprintf(fp,"%d\n",seed);
    fprintf(fp,"%d\n",j1.x);
    fprintf(fp,"%d\n",j1.y);
    fprintf(fp,"%d\n",j1.pv);
    fprintf(fp,"%d\n",j1.exp);
    fprintf(fp,"%d\n",j1.tasksDone);
    fprintf(fp,"%ld\n",start-pause+j1.timer);     //timeBegin-time(NULL)+stats.timer
    fprintf(fp,"%d\n",spacestation.n_tasks);
    fprintf(fp,"%d\n",spacestation.total_rooms);
    fprintf(fp,"%d\n",spacestation.nr_created);
    fprintf(fp,"%d\n",spacestation.nd_left);

    for (int i=0;i<spacestation.nr_created;i++){
        fprintf(fp,"%d\n",spacestation.rooms[i].x);
        fprintf(fp,"%d\n",spacestation.rooms[i].y);
        fprintf(fp,"%d\n",spacestation.rooms[i].H);
        fprintf(fp,"%d\n",spacestation.rooms[i].L);
        fprintf(fp,"%d\n",spacestation.rooms[i].tasks.x);
        fprintf(fp,"%d\n",spacestation.rooms[i].tasks.y);
        fprintf(fp,"%d\n",spacestation.rooms[i].tasks.done);
        fprintf(fp,"%d\n",spacestation.rooms[i].n_doors);
        for (int j=0;j<spacestation.rooms[i].n_doors;j++){
            fprintf(fp,"%d\n",spacestation.rooms[i].doors[j].x);
            fprintf(fp,"%d\n",spacestation.rooms[i].doors[j].y);
            fprintf(fp,"%d\n",spacestation.rooms[i].doors[j].wall);
            fprintf(fp,"%d\n",spacestation.rooms[i].doors[j].open);
        }
    }
    fprintf(fp,"}\n");

    /*for(int i=0;i<nbsave;i++){
        if(strequal(tab[i],name)!=1){
            fprintf(fp,"{%s\n",*(tab+i));
            fprintf(fp,"%d\n",*(seedtab+i));
            fprintf(fp,"%d\n",((playertab+i)->x));
            fprintf(fp,"%d\n",( (playertab+i)->y));
            fprintf(fp,"%d\n",((playertab+i)->pv));
            fprintf(fp,"%d\n",((playertab+i)->exp));
            fprintf(fp,"%d\n",((playertab+i)->tasksDone));
            fprintf(fp,"%d\n",((playertab+i)->timer));

            fprintf(fp,"%d\n",((spacestationtab+i)->n_tasks));
            fprintf(fp,"%d\n",((spacestationtab+i)->total_rooms));
            fprintf(fp,"%d\n",((spacestationtab+i)->nr_created));
            fprintf(fp,"%d\n",((spacestationtab+i)->nd_left));

            for (int i=0;i<(spacestationtab+i)->nr_created;i++){
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[i].x));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[i].y));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[i].H));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[i].L));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[i].tasks.x));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[i].tasks.y));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[i].tasks.done));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[i].n_doors));
                for (int j=0;j<(spacestationtab+i)->rooms[i].n_doors;j++){
                    fprintf(fp,"%d\n",((spacestationtab+i)->rooms[i].doors[j].x));
                    fprintf(fp,"%d\n",((spacestationtab+i)->rooms[i].doors[j].y));
                    fprintf(fp,"%d\n",((spacestationtab+i)->rooms[i].doors[j].wall));
                    fprintf(fp,"%d\n",((spacestationtab+i)->rooms[i].doors[j].open));
                }
            }
        }
    }*/


    fclose(fp);

    return 0;
}




int main(){
    Character a;
    a.pv=88;
    Dungeon b;
    UpdateSave(b,a,"aeiou",12,500,480);
    printf("1");


    return 0;
}