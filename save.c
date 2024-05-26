#include "main.h"

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

int NewSave(Dungeon spacestation, Character player, char* name, int seed, time_t timer){
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
        fprintf(fp,"%d\n",player.damage);
        fprintf(fp,"%d\n",player.hold);
        for (int i=0;i<4;i++){
            fprintf(fp,"%d\n",player.bag.use_pouch[i].x);
            fprintf(fp,"%d\n",player.bag.use_pouch[i].y);
            fprintf(fp,"%d\n",player.bag.use_pouch[i].pickup);
            fprintf(fp,"%d\n",player.bag.use_pouch[i].exp);
            fprintf(fp,"%d\n",player.bag.use_pouch[i].heal);
            fprintf(fp,"%d\n",player.bag.use_pouch[i].icon);
        }
        for (int i=0;i<2;i++){
            fprintf(fp,"%d\n",player.bag.weaponPouch[i].x);
            fprintf(fp,"%d\n",player.bag.weaponPouch[i].y);
            fprintf(fp,"%d\n",player.bag.weaponPouch[i].pickup);
            fprintf(fp,"%d\n",player.bag.weaponPouch[i].damage);
            fprintf(fp,"%d\n",player.bag.weaponPouch[i].icon);
        }

        fprintf(fp,"%ld\n",timer-time(NULL)+player.timer);

        fprintf(fp,"%d\n",spacestation.n_tasks);
        fprintf(fp,"%d\n",spacestation.n_tasks2create);
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

            fprintf(fp,"%d\n",spacestation.rooms[i].object.x);
            fprintf(fp,"%d\n",spacestation.rooms[i].object.y);
            fprintf(fp,"%d\n",spacestation.rooms[i].object.pickup);
            fprintf(fp,"%d\n",spacestation.rooms[i].object.exp);
            fprintf(fp,"%d\n",spacestation.rooms[i].object.heal);
            fprintf(fp,"%d\n",spacestation.rooms[i].object.icon);

            fprintf(fp,"%d\n",spacestation.rooms[i].weapon.x);
            fprintf(fp,"%d\n",spacestation.rooms[i].weapon.y);
            fprintf(fp,"%d\n",spacestation.rooms[i].weapon.pickup);
            fprintf(fp,"%d\n",spacestation.rooms[i].weapon.damage);
            fprintf(fp,"%d\n",spacestation.rooms[i].weapon.icon);

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
}

int SavingChoice(Character player, char* name, int seed, int tasks, time_t* timeBegin){
    int k=0,cursor = 0;
    time_t delay = time(NULL);

    nocbreak();
    cbreak();

    while(k!=10){
        delay = time(NULL);
        clear();
        switch(k){
            case KEYUP:
                if (cursor>0){
                    cursor--;
                }
                break;
            case KEYDOWN:
                if (cursor<2){
                    cursor++;
                }
                break;
        }

        PrintUI(player,name,seed,tasks,*timeBegin,3);
        PrintLetters(8);
        mvprintw(SCREENI+7,SCREENL/2-25,"Your game has been saved. What would you like to do?");
        mvprintw(SCREENI+9,SCREENL/2-5,"  Continue");
        mvprintw(SCREENI+10,SCREENL/2-5,"  Return to Title Screen");
        mvprintw(SCREENI+11,SCREENL/2-5,"  Quit");
        mvprintw(SCREENI+9+cursor,SCREENL/2-5,"⁕");
        mvprintw(SCREENH+2,0," ");
        refresh();
        k=getch();
        *timeBegin += time(NULL) - delay;       //Pause the timer
        delay = time(NULL);
    }
    if (cursor == 0){
        halfdelay(10);
    }

    return cursor;
}

int SearchSave(char* name){
    FILE* fp=NULL;
    int cursor=0;
    int size=0;
    size=strlen(name);
    char c=0;
    fp= fopen("saves.txt","a");
    fclose(fp);
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
    fclose(fp);
    return -1;
}

int GetSave(Dungeon* spacestation, Character* player, char* name, int* seed){
    FILE *fp;

    if(SearchSave(name)!=-1){
        fp=fopen("saves.txt","r+");
        fseek(fp,SearchSave(name)+1,SEEK_SET);
        fscanf(fp,"%d",seed);
        fscanf(fp,"%d",&(player->x));
        fscanf(fp,"%d",&(player->y));
        fscanf(fp,"%d",&(player->pv));
        fscanf(fp,"%d",&(player->exp));
        fscanf(fp,"%d",&(player->tasksDone));
        fscanf(fp,"%d",&(player->damage));
        fscanf(fp,"%d",&(player->hold));
        for (int i=0;i<4;i++){
            fscanf(fp,"%d",&(player->bag.use_pouch[i].x));
            fscanf(fp,"%d",&(player->bag.use_pouch[i].y));
            fscanf(fp,"%d",&(player->bag.use_pouch[i].pickup));
            fscanf(fp,"%d",&(player->bag.use_pouch[i].exp));
            fscanf(fp,"%d",&(player->bag.use_pouch[i].heal));
            fscanf(fp,"%d",&(player->bag.use_pouch[i].icon));
        }
        for (int i=0;i<2;i++){
            fscanf(fp,"%d",&(player->bag.weaponPouch[i].x));
            fscanf(fp,"%d",&(player->bag.weaponPouch[i].y));
            fscanf(fp,"%d",&(player->bag.weaponPouch[i].pickup));
            fscanf(fp,"%d",&(player->bag.weaponPouch[i].damage));
            fscanf(fp,"%d",&(player->bag.weaponPouch[i].icon));
        }

        fscanf(fp,"%d",&(player->timer));

        fscanf(fp,"%d",&(spacestation->n_tasks));
        fscanf(fp,"%d",&(spacestation->n_tasks2create));
        fscanf(fp,"%d",&(spacestation->total_rooms));
        fscanf(fp,"%d",&(spacestation->nr_created));
        fscanf(fp,"%d",&(spacestation->nd_left));

        spacestation->rooms = NULL;
        spacestation->rooms = malloc(spacestation->total_rooms*sizeof(Room));
        if(spacestation->rooms == NULL){
            return 2;
        }

        for (int i=0;i<spacestation->nr_created;i++){
            fscanf(fp,"%d",&(spacestation->rooms[i].x));
            fscanf(fp,"%d",&(spacestation->rooms[i].y));
            fscanf(fp,"%d",&(spacestation->rooms[i].H));
            fscanf(fp,"%d",&(spacestation->rooms[i].L));
            fscanf(fp,"%d",&(spacestation->rooms[i].tasks.x));
            fscanf(fp,"%d",&(spacestation->rooms[i].tasks.y));
            fscanf(fp,"%d",&(spacestation->rooms[i].tasks.done));

            fscanf(fp,"%d",&(spacestation->rooms[i].object.x));
            fscanf(fp,"%d",&(spacestation->rooms[i].object.y));
            fscanf(fp,"%d",&(spacestation->rooms[i].object.pickup));
            fscanf(fp,"%d",&(spacestation->rooms[i].object.exp));
            fscanf(fp,"%d",&(spacestation->rooms[i].object.heal));
            fscanf(fp,"%d",&(spacestation->rooms[i].object.icon));
            
            fscanf(fp,"%d",&(spacestation->rooms[i].weapon.x));
            fscanf(fp,"%d",&(spacestation->rooms[i].weapon.y));
            fscanf(fp,"%d",&(spacestation->rooms[i].weapon.pickup));
            fscanf(fp,"%d",&(spacestation->rooms[i].weapon.damage));
            fscanf(fp,"%d",&(spacestation->rooms[i].weapon.icon));

            fscanf(fp,"%d",&(spacestation->rooms[i].n_doors));

            spacestation->rooms[i].doors = NULL;
            spacestation->rooms[i].doors = malloc(spacestation->rooms[i].n_doors*sizeof(Door));
            if(spacestation->rooms[i].doors == NULL){
                return 2;
            }
            
            for (int j=0;j<spacestation->rooms[i].n_doors;j++){
                fscanf(fp,"%d",&(spacestation->rooms[i].doors[j].x));
                fscanf(fp,"%d",&(spacestation->rooms[i].doors[j].y));
                fscanf(fp,"%d",&(spacestation->rooms[i].doors[j].wall));
                fscanf(fp,"%d",&(spacestation->rooms[i].doors[j].open));
            }
        }
        fclose(fp);
        return 1;
    }
    return -1;
}

int getname(char ** tab){
    FILE* fp=NULL;
    char c=0;
    char* nom;
    int size=0;
    int indice=0;

    fp=fopen("saves.txt","r");
        c=fgetc(fp);
        while(c!=EOF){
            if(c=='{'){
                (nom) = malloc(sizeof(char)*(NAMELIMIT+1));

                for (int i=0;i<NAMELIMIT;i++){        // Initialise the name
                    nom[i] = '\0';
                }

                c=fgetc(fp);
                indice=0;
                do{
                    nom[indice]=c;
                    indice++;
                    c=fgetc(fp);
                }while(c!='\n');
                (tab[size]) = nom;
                size++;
            }

            c=fgetc(fp);
        }
    fclose(fp);
    return size;
}

int UpdateSave(Dungeon spacestation, Character player, char* name, int seed,time_t start, time_t pause){
    /*save the parameter in the file save.txt if the name does not exist return -1*/
    char** tab;
    char vide[20];
    int nbsave=0,flag=0,j=0;
    FILE* fp=NULL;
    Character* playertab=NULL;
    int* seedtab=NULL;
    Dungeon* spacestationtab=NULL;

    tab=malloc(4*sizeof(char*));
    for(int i=0;i<4;i++){
        tab[i]=NULL;
    }

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

    for(int i=0;i<nbsave;i++){
        if(strequal(tab[i],name) != 1){
            fseek(fp,SearchSave(tab[i])+1,SEEK_SET);
            fscanf(fp,"%d",&(seedtab[j]));
            fscanf(fp,"%d",&(playertab[j].x));
            fscanf(fp,"%d",&(playertab[j].y));
            fscanf(fp,"%d",&(playertab[j].pv));
            fscanf(fp,"%d",&(playertab[j].exp));
            fscanf(fp,"%d",&(playertab[j].tasksDone));
            fscanf(fp,"%d",&(playertab[j].damage));
            fscanf(fp,"%d",&(playertab[j].hold));
            for (int k=0;k<4;k++){
                fscanf(fp,"%d",&(playertab[j].bag.use_pouch[k].x));
                fscanf(fp,"%d",&(playertab[j].bag.use_pouch[k].y));
                fscanf(fp,"%d",&(playertab[j].bag.use_pouch[k].pickup));
                fscanf(fp,"%d",&(playertab[j].bag.use_pouch[k].exp));
                fscanf(fp,"%d",&(playertab[j].bag.use_pouch[k].heal));
                fscanf(fp,"%d",&(playertab[j].bag.use_pouch[k].icon));
            }
            for (int k=0;k<2;k++){
                fscanf(fp,"%d",&(playertab[j].bag.weaponPouch[k].x));
                fscanf(fp,"%d",&(playertab[j].bag.weaponPouch[k].y));
                fscanf(fp,"%d",&(playertab[j].bag.weaponPouch[k].pickup));
                fscanf(fp,"%d",&(playertab[j].bag.weaponPouch[k].damage));
                fscanf(fp,"%d",&(playertab[j].bag.weaponPouch[k].icon));
            }

            fscanf(fp,"%d",&(playertab[j].timer));

            fscanf(fp,"%d",&(spacestationtab[j].n_tasks));
            fscanf(fp,"%d",&(spacestationtab[j].n_tasks2create));
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

                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].object.x));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].object.y));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].object.pickup));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].object.exp));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].object.heal));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].object.icon));

                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].weapon.x));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].weapon.y));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].weapon.pickup));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].weapon.damage));
                fscanf(fp,"%d",&(spacestationtab[j].rooms[k].weapon.icon));

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
    fprintf(fp,"%d\n",player.x);
    fprintf(fp,"%d\n",player.y);
    fprintf(fp,"%d\n",player.pv);
    fprintf(fp,"%d\n",player.exp);
    fprintf(fp,"%d\n",player.tasksDone);
    fprintf(fp,"%d\n",player.damage);
    fprintf(fp,"%d\n",player.hold);
    for (int i=0;i<4;i++){
        fprintf(fp,"%d\n",player.bag.use_pouch[i].x);
        fprintf(fp,"%d\n",player.bag.use_pouch[i].y);
        fprintf(fp,"%d\n",player.bag.use_pouch[i].pickup);
        fprintf(fp,"%d\n",player.bag.use_pouch[i].exp);
        fprintf(fp,"%d\n",player.bag.use_pouch[i].heal);
        fprintf(fp,"%d\n",player.bag.use_pouch[i].icon);
    }
    for (int i=0;i<2;i++){
        fprintf(fp,"%d\n",player.bag.weaponPouch[i].x);
        fprintf(fp,"%d\n",player.bag.weaponPouch[i].y);
        fprintf(fp,"%d\n",player.bag.weaponPouch[i].pickup);
        fprintf(fp,"%d\n",player.bag.weaponPouch[i].damage);
        fprintf(fp,"%d\n",player.bag.weaponPouch[i].icon);
    }
    fprintf(fp,"%ld\n",start-pause+player.timer);     //timeBegin-time(NULL)+stats.timer
    fprintf(fp,"%d\n",spacestation.n_tasks);
    fprintf(fp,"%d\n",spacestation.n_tasks2create);
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

        fprintf(fp,"%d\n",spacestation.rooms[i].object.x);
        fprintf(fp,"%d\n",spacestation.rooms[i].object.y);
        fprintf(fp,"%d\n",spacestation.rooms[i].object.pickup);
        fprintf(fp,"%d\n",spacestation.rooms[i].object.exp);
        fprintf(fp,"%d\n",spacestation.rooms[i].object.heal);
        fprintf(fp,"%d\n",spacestation.rooms[i].object.icon);

        fprintf(fp,"%d\n",spacestation.rooms[i].weapon.x);
        fprintf(fp,"%d\n",spacestation.rooms[i].weapon.y);
        fprintf(fp,"%d\n",spacestation.rooms[i].weapon.pickup);
        fprintf(fp,"%d\n",spacestation.rooms[i].weapon.damage);
        fprintf(fp,"%d\n",spacestation.rooms[i].weapon.icon);

        fprintf(fp,"%d\n",spacestation.rooms[i].n_doors);

        for (int j=0;j<spacestation.rooms[i].n_doors;j++){
            fprintf(fp,"%d\n",spacestation.rooms[i].doors[j].x);
            fprintf(fp,"%d\n",spacestation.rooms[i].doors[j].y);
            fprintf(fp,"%d\n",spacestation.rooms[i].doors[j].wall);
            fprintf(fp,"%d\n",spacestation.rooms[i].doors[j].open);
        }
    }
    fprintf(fp,"}\n");

    j=0;
    for(int i=0;i<nbsave;i++){
        if(strequal(tab[i],name)!=1){
            fprintf(fp,"{%s\n",*(tab+i));
            fprintf(fp,"%d\n",*(seedtab+j));
            fprintf(fp,"%d\n",((playertab+j)->x));
            fprintf(fp,"%d\n",((playertab+j)->y));
            fprintf(fp,"%d\n",((playertab+j)->pv));
            fprintf(fp,"%d\n",((playertab+j)->exp));
            fprintf(fp,"%d\n",((playertab+j)->tasksDone));
            fprintf(fp,"%d\n",(playertab+j)->damage);
            fprintf(fp,"%d\n",(playertab+j)->hold);
            for (int k=0;k<4;k++){
                fprintf(fp,"%d\n",(playertab+j)->bag.use_pouch[k].x);
                fprintf(fp,"%d\n",(playertab+j)->bag.use_pouch[k].y);
                fprintf(fp,"%d\n",(playertab+j)->bag.use_pouch[k].pickup);
                fprintf(fp,"%d\n",(playertab+j)->bag.use_pouch[k].exp);
                fprintf(fp,"%d\n",(playertab+j)->bag.use_pouch[k].heal);
                fprintf(fp,"%d\n",(playertab+j)->bag.use_pouch[k].icon);
            }
            for (int k=0;k<2;k++){
                fprintf(fp,"%d\n",(playertab+j)->bag.weaponPouch[k].x);
                fprintf(fp,"%d\n",(playertab+j)->bag.weaponPouch[k].y);
                fprintf(fp,"%d\n",(playertab+j)->bag.weaponPouch[k].pickup);
                fprintf(fp,"%d\n",(playertab+j)->bag.weaponPouch[k].damage);
                fprintf(fp,"%d\n",(playertab+j)->bag.weaponPouch[k].icon);
            }
            fprintf(fp,"%d\n",((playertab+j)->timer));

            fprintf(fp,"%d\n",((spacestationtab+j)->n_tasks));
            fprintf(fp,"%d\n",(spacestationtab+j)->n_tasks2create);
            fprintf(fp,"%d\n",((spacestationtab+j)->total_rooms));
            fprintf(fp,"%d\n",((spacestationtab+j)->nr_created));
            fprintf(fp,"%d\n",((spacestationtab+j)->nd_left));

            for (int k=0;k<(spacestationtab+j)->nr_created;k++){
                fprintf(fp,"%d\n",((spacestationtab+j)->rooms[k].x));
                fprintf(fp,"%d\n",((spacestationtab+j)->rooms[k].y));
                fprintf(fp,"%d\n",((spacestationtab+j)->rooms[k].H));
                fprintf(fp,"%d\n",((spacestationtab+j)->rooms[k].L));
                fprintf(fp,"%d\n",((spacestationtab+j)->rooms[k].tasks.x));
                fprintf(fp,"%d\n",((spacestationtab+j)->rooms[k].tasks.y));
                fprintf(fp,"%d\n",((spacestationtab+j)->rooms[k].tasks.done));

                fprintf(fp,"%d\n",(spacestationtab+j)->rooms[k].object.x);
                fprintf(fp,"%d\n",(spacestationtab+j)->rooms[k].object.y);
                fprintf(fp,"%d\n",(spacestationtab+j)->rooms[k].object.pickup);
                fprintf(fp,"%d\n",(spacestationtab+j)->rooms[k].object.exp);
                fprintf(fp,"%d\n",(spacestationtab+j)->rooms[k].object.heal);
                fprintf(fp,"%d\n",(spacestationtab+j)->rooms[k].object.icon);
                
                fprintf(fp,"%d\n",(spacestationtab+j)->rooms[k].weapon.x);
                fprintf(fp,"%d\n",(spacestationtab+j)->rooms[k].weapon.y);
                fprintf(fp,"%d\n",(spacestationtab+j)->rooms[k].weapon.pickup);
                fprintf(fp,"%d\n",(spacestationtab+j)->rooms[k].weapon.damage);
                fprintf(fp,"%d\n",(spacestationtab+j)->rooms[k].weapon.icon);

                fprintf(fp,"%d\n",((spacestationtab+j)->rooms[k].n_doors));

                for (int l=0;l<(spacestationtab+j)->rooms[k].n_doors;l++){
                    fprintf(fp,"%d\n",((spacestationtab+j)->rooms[k].doors[l].x));
                    fprintf(fp,"%d\n",((spacestationtab+j)->rooms[k].doors[l].y));
                    fprintf(fp,"%d\n",((spacestationtab+j)->rooms[k].doors[l].wall));
                    fprintf(fp,"%d\n",((spacestationtab+j)->rooms[k].doors[l].open));
                }
            }
            fprintf(fp,"}\n");
            j++;
        }
    }

    fclose(fp);

    return 0;
}
