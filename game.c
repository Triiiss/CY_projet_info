#include "main.h"

int Pause(Character stats, char* name, int seed, int tasks, time_t* begin){
    /*Print the pause menu
    Character : stats : The adress of the player (the pv, xp, object, etc..)
    char* : name : The name of the player
    int : seed : The seed of the map
    int : tasks : number of tasks to do
    time_t* : begin : The adress of the time when the timer started (so we can pause it)
    Returns : int : The choice they made in the pause menu (continue, save, etc..)*/

    nocbreak();     // Cancels the halfdelay() bc timer is paused
    cbreak();

    int k=0, cursor=0;
    time_t delay = time(NULL);
    *begin+=1;      // Bc pausing takes about a second (to erase delay)
    
    while(k!=10){
        clear();
        switch(k){
            case KEYUP:
                if (cursor>0){
                    cursor--;
                }
                break;
            case KEYDOWN:
                if (cursor<3){
                    cursor++;
                }
                break;
        }
        PrintUI(stats,name,seed,tasks,*begin,3);
        PrintLetters(4);
        mvprintw((SCREENH-SCREENI)/5*2+4,SCREENL/5*2,"Continue");
        mvprintw((SCREENH-SCREENI)/5*2+5,SCREENL/5*2,"Save");
        mvprintw((SCREENH-SCREENI)/5*2+6,SCREENL/5*2,"Return to title");
        mvprintw((SCREENH-SCREENI)/5*2+7,SCREENL/5*2,"Quit");
        mvprintw(cursor+(SCREENH-SCREENI)/5*2+4,SCREENL/5*2-2,"⁕");
        mvprintw(SCREENH+2,0," ");
        refresh();

        k=getch();
        *begin += time(NULL) - delay;       //Pause the timer
        delay = time(NULL);
    }
    halfdelay(10);      // Restarts the delay for the timer
    return cursor;
}

int InventoryFull(Character player, int choice){
    /*Gives info about the inventory
    Character : player : to access the bag
    int : choice : If either weapon or object we're looking for
    Returns : int : -1 if the inventory is full, or the index of the first free slot in the inventory*/

    if (choice){
        for (int i=0;i<2;i++){
            if (player.bag.weaponPouch[i].pickup == 0){
                return i;
            }
        }
    }
    else{
        for (int i=0;i<4;i++){
            if (player.bag.use_pouch[i].pickup == 0){
                return i;
            }
        }
    }
    return -1;
}

int Inventory(Character* player, char* name, int seed, int tasks, time_t *begin){
    /*Print the inventory menu and makes you manage it
    Character* : player : The adress of the player (the pv, xp, object, etc..) to change the inventory
    char* : name : The name of the player
    int : seed : The seed of the map
    time_t* : begin : The adress of the time when the timer started (so we can pause it)
    Returns : int : 0 if no error*/

    nocbreak();     // Cancels the halfdelay()
    cbreak();
    int k=0, cursorobj=-1,cursorchoice=0,use=0,drop=1;
    time_t delay = time(NULL);

    while (k!=27){
        switch(k){      // Move the cursor
            case KEYLEFT:
                cursorobj=-1;
                break;
            case KEYRIGHT:
                cursorobj=1;
                break;
            case KEYDOWN:
                if (cursorobj<0 && cursorobj>-2){
                    cursorobj--;
                }
                if (cursorobj>0 && cursorobj<4){
                    cursorobj++;
                }
                break;
            case KEYUP:
                if (cursorobj<-1){
                    cursorobj++;
                }
                if (cursorobj>1){
                    cursorobj--;
                }
                break;
            case 10:
                k=0;
                if (cursorobj>0){      // In object choice's section
                    if (player->bag.use_pouch[cursorobj-1].pickup == 1){        // If there is an object here
                        cursorchoice = 2;
                        use = 1;
                        do{
                            PrintInventory(player,name,seed, tasks,begin,cursorobj);
                            switch(k){
                                case KEYDOWN:       // Move the cursor
                                    if (cursorchoice>0){
                                        cursorchoice--;
                                    }
                                    break;
                                case KEYUP:
                                    if (cursorchoice<2){
                                        cursorchoice++;
                                    }
                                    break;
                                case 10:
                                    switch (cursorchoice){
                                        case 0:         // Drop
                                            player->bag.use_pouch[cursorobj-1].exp = 0;
                                            player->bag.use_pouch[cursorobj-1].heal = 0;
                                            player->bag.use_pouch[cursorobj-1].icon = -1;
                                            player->bag.use_pouch[cursorobj-1].x = 0;
                                            player->bag.use_pouch[cursorobj-1].y = 0;
                                            player->bag.use_pouch[cursorobj-1].pickup = 0;
                                            mvprintw(SCREENH+2,0,"You dropped an object");
                                            refresh();
                                            use = 0;
                                            break;
                                        case 1:         //Gives info about the object
                                            mvprintw(SCREENH-6,SCREENL/4*3,"Info:");
                                            switch(player->bag.use_pouch[cursorobj-1].icon){
                                                case 1:
                                                    mvprintw(SCREENH-4,SCREENL/4*3-10,"Potion of Strengh : Makes you able");
                                                    mvprintw(SCREENH-3,SCREENL/4*3-10,"to deal more damage.");
                                                    break;
                                                case 0:
                                                    mvprintw(SCREENH-4,SCREENL/4*3-10,"Healing potion : ");
                                                    mvprintw(SCREENH-3,SCREENL/4*3-10,"Gives you %d health",player->bag.use_pouch[cursorobj -1].heal);
                                                    mvprintw(SCREENH-2,SCREENL/4*3-10,"(not more than your maximum)");
                                                    break;
                                                case 2:
                                                    mvprintw(SCREENH-4,SCREENL/4*3-10,"Book of knowledge : ");
                                                    mvprintw(SCREENH-3,SCREENL/4*3-10,"Gives you more experience");
                                                    break;
                                            }
                                            break;
                                        case 2:     // Use it
                                            switch(player->bag.use_pouch[cursorobj-1].icon){
                                                case 1:
                                                    player->damage += player->bag.use_pouch[cursorobj-1].heal;
                                                    mvprintw(SCREENH+2,0,"You gained %d of strenght",player->bag.use_pouch[cursorobj-1].heal);
                                                    break;
                                                case 0:
                                                    player->pv += player->bag.use_pouch[cursorobj-1].heal;
                                                    if (player->pv>MAXPV){
                                                        player->pv = 100;
                                                    }
                                                    mvprintw(SCREENH+2,0,"You gained %d health points",player->bag.use_pouch[cursorobj-1].heal);
                                                    break;
                                                case 2:
                                                    player->exp += player->bag.use_pouch[cursorobj-1].exp;
                                                    mvprintw(SCREENH+2,0,"You gained %d experience points",player->bag.use_pouch[cursorobj-1].exp);
                                                    break;
                                            }
                                            player->bag.use_pouch[cursorobj-1].exp = 0;     // Erase it from your bag
                                            player->bag.use_pouch[cursorobj-1].heal = 0;
                                            player->bag.use_pouch[cursorobj-1].icon = -1;
                                            player->bag.use_pouch[cursorobj-1].x = 0;
                                            player->bag.use_pouch[cursorobj-1].y = 0;
                                            player->bag.use_pouch[cursorobj-1].pickup = 0;
                                            refresh();
                                            use = 0;
                                            break;
                                    }
                                break;
                            }

                            mvprintw(SCREENI+10+(cursorobj-1)*2,SCREENL/2+3,"▣ ");
                            for (int j=1;j<4;j++){
                                mvprintw(SCREENH-j*2,SCREENL/2+3," ");
                            }
                            mvprintw(SCREENH-(cursorchoice+1)*2,SCREENL/2+3,"⁕");
                            mvprintw(SCREENH+3,0," ");

                            refresh();
                            k = getch();
                        }while (k != 27 && use);
                    }
                }

                else if (cursorobj<0){      // In weapon's section
                    if (player->bag.weaponPouch[-cursorobj-1].pickup == 1){     // If there is a weapon selected
                        cursorchoice = 3;
                        drop = 1;
                        do{
                            PrintInventory(player,name,seed, tasks,begin,cursorobj);
                            switch (k){         // Move cursor
                                case KEYDOWN:
                                    if (cursorchoice>0){
                                        cursorchoice--;
                                    }
                                    break;
                                case KEYUP:
                                    if (cursorchoice<3){
                                        cursorchoice++;
                                    }
                                    break;
                                case 10:
                                    switch(cursorchoice){
                                        case 0:     // Drop
                                            if (-cursorobj-1 == player->hold){
                                                mvprintw(SCREENH+2,0,"You are holding this weapon, you can't drop it");
                                            }
                                            else{
                                                player->bag.weaponPouch[-cursorobj-1].x = 0;
                                                player->bag.weaponPouch[-cursorobj-1].y = 0;
                                                player->bag.weaponPouch[-cursorobj-1].pickup = 0;
                                                player->bag.weaponPouch[-cursorobj-1].damage = 0;
                                                player->bag.weaponPouch[-cursorobj-1].icon = -1;
                                                mvprintw(SCREENH+2,0,"You dropped an object");
                                            }
                                            refresh();
                                            use = 0;
                                            break;
                                        case 1:         // Gives info
                                            mvprintw(SCREENH-6,SCREENL/4-10,"Info:");
                                            switch(player->bag.weaponPouch[-cursorobj-1].icon){
                                                case 0:
                                                    mvprintw(SCREENH-4,SCREENL/4-10,"Laser Sword : A great weapon");
                                                    mvprintw(SCREENH-3,SCREENL/4-10,"for a great soldier.");
                                                    mvprintw(SCREENH-2,SCREENL/4-10,"Deals 45 damage.");
                                                    break;
                                                case 1:
                                                    mvprintw(SCREENH-4,SCREENL/4-10,"Reparing tools : Used to fix");
                                                    mvprintw(SCREENH-3,SCREENL/4-10,"things on the ship");
                                                    mvprintw(SCREENH-2,SCREENL/4-10,"Deals 10 damage.");
                                                    break;
                                            }
                                            refresh();
                                            break;
                                        case 2:             // Unequip a weapon
                                            if (player->hold == -cursorobj-1){
                                                player->hold = -1;
                                                player->damage -= player->bag.weaponPouch[-cursorobj-1].damage;
                                                mvprintw(SCREENH+2,0,"You unequiped your weapon");
                                            }
                                            else{
                                                mvprintw(SCREENH+2,0,"This weapon wasn't equiped in the first place");
                                            }
                                            break;
                                        case 3:         // Equip a weapon
                                            if (player->hold != -cursorobj-1){
                                                if (player->hold != -1){
                                                    player->damage -= player->bag.weaponPouch[player->hold].damage;
                                                }
                                                player->hold = -cursorobj-1;
                                                player->damage += player->bag.weaponPouch[player->hold].damage;
                                                mvprintw(SCREENH+2,0,"Weapon equiped");
                                            }
                                            else{
                                                mvprintw(SCREENH+2,0,"This weapon is already equiped");
                                            }
                                            break;
                                    }
                                    break;
                            }
                            mvprintw(SCREENI+10+(-cursorobj-1)*2,SCREENL/2+3,"▣ ");
                            for (int j=1;j<4;j++){
                                mvprintw(SCREENH-j*2,3," ");
                            }
                            mvprintw(SCREENH-(cursorchoice+1)*2,3,"⁕");
                            mvprintw(SCREENH+3,0," ");
                            refresh();
                            k = getch();
                        }while (k!=27 && drop);
                    }
                }
                
                break;
        }
        PrintInventory(player, name, seed, tasks,begin,cursorobj);

        mvprintw(SCREENH+3,0," ");
        refresh();

        k=getch();
        *begin += time(NULL) - delay;       //Pause the timer
        delay = time(NULL);
    }
    halfdelay(10);      // Restarts the delay for the timer

    return 0;
}

int DoTask(Dungeon* spaceStation, Character* stats, Task* task, char* name, int seed , time_t begin){
    /*Do a task
    Dungeon* : spaceStation, char* : name, int : seed : to print the UI
    Character* : stats : The adress of the player to change the number of tasks done
    Task* : task : The adress of the task to do to change its icon
    time_t : begin : To pause the timer
    Returns : int : 0 if no error*/

    int done = 0,k=10;
    time_t time_task = time(NULL);
    halfdelay(10);
    while((k == 10 || k == ERR) && !done){
        if (time(NULL)-time_task>= TIMETASK){
            done = 1;
        }
        PrintUI(*stats,name,seed,spaceStation->n_tasks,begin,3);
        PrintMap(*spaceStation,*stats);
        mvprintw(SCREENH+2,0,"Doing task  wait %d seconds",TIMETASK);       // Wait 3 seconds to do a task
        for (int i=0;i<BARPV;i++){      // Little loading bar
            mvprintw(SCREENH+3,i,"▧");
        }
        for (int i=0;i<(time(NULL)-time_task)*BARPV/TIMETASK;i++){
            mvprintw(SCREENH+3,i,"▆");
        }
        mvprintw(SCREENH+4,0," ");
        refresh();
        k=getch();
    }
    if (done){          // Change the task to "done" if you haven't moved during the task
        task->done = 1;
        stats->tasksDone++;
        stats->exp += (Rand(seed,stats->tasksDone*5,3)%100)+250;
        clear();
        PrintUI(*stats,name,seed,spaceStation->n_tasks,begin,3);
        PrintMap(*spaceStation,*stats);
        mvprintw(SCREENH+2,0,"Task done");
        refresh();
        getch();
    }

    return 0;
}

int PickupObject(Character* player,Object* object){
    /*Used to pick up an object
    Character* : player : The adress of the player to add an object if picked up
    Object* : object: The adress of the object to take the info and not print it anymore if it is picked up
    Returns : 0 if no error*/

    int place = InventoryFull(*player,0);
    if (place  != -1){
        player->bag.use_pouch[place].exp = object->exp;
        player->bag.use_pouch[place].heal = object->heal;
        player->bag.use_pouch[place].icon = object->icon;
        player->bag.use_pouch[place].pickup = 1;

        object->pickup = 1;
        object->x = 0;
        object->y = 0;
        mvprintw(SCREENH+2,0,"You picked up an object                   ");
        refresh();
        getch();
    }
    else{
        mvprintw(SCREENH+2,0,"Your inventory is full, you can't pick up this object");
        refresh();
        getch();
    }

    return 0;
}

int PickupWeapon(Character* player,Weapon* weapon){
    /*Used to pick up an object
    Character* : player : The adress of the player to add a weapon if picked up
    Weapon* : weapon : The adress of the weapon to take the info and not print it anymore if it is picked up
    Returns : 0 if no error*/
    
    int place = InventoryFull(*player,1);
    if (place  != -1){
        player->bag.weaponPouch[place].damage = weapon->damage;
        player->bag.weaponPouch[place].icon = weapon->icon;
        player->bag.weaponPouch[place].pickup = 1;

        weapon->pickup = 1;
        weapon->x = 0;
        weapon->y = 0;
        mvprintw(SCREENH+2,0,"You picked up a weapon                   ");
    }
    else{
        mvprintw(SCREENH+2,0,"Your inventory is full, you can't pick up this weapon");
    }
    mvprintw(SCREENH+3,0," ");
    refresh();
    getch();

    return 0;
}

int Movement(Dungeon spaceStation,int x,int y){
    /*Check if you're able to to somewhere and what actions you can do (go through door, pick up object, wall, do a task)
    Dungeon : spaceStation : The map
    int : x : The coordinate we're checking
    int : y : The coordinate we're checking
    Returns : int : 0 if cannot go there (wall), 1 if you can just walk, 2 if it's a door, 3 if you can do a task, 4 if you can pick up an object*/

    for (int i=0; i<spaceStation.nr_created;i++){
        for (int j=0;j<spaceStation.rooms[i].n_doors;j++){     // Doors
            if (spaceStation.rooms[i].doors[j].x == x && spaceStation.rooms[i].doors[j].y == y && spaceStation.rooms[i].doors[j].open == 0){
                return 2;
            }
            if (spaceStation.rooms[i].doors[j].x == x && spaceStation.rooms[i].doors[j].y == y && spaceStation.rooms[i].doors[j].open == 1){
                return 1;
            }
        }
        for (int j=0;j<spaceStation.rooms[i].L;j++){       // Top and bottom wall
            if (spaceStation.rooms[i].x + j == x && (spaceStation.rooms[i].y == y || spaceStation.rooms[i].H -1 + spaceStation.rooms[i].y == y )){
                return 0;
            }
        }
        for (int j=0;j<spaceStation.rooms[i].H;j++){       // Left and right wall
            if ((spaceStation.rooms[i].x== x || spaceStation.rooms[i].x + spaceStation.rooms[i].L-1 == x) && spaceStation.rooms[i].y + j == y){
                return 0;
            }
        }
        if (spaceStation.rooms[i].tasks.x == x && spaceStation.rooms[i].tasks.y == y && spaceStation.rooms[i].tasks.done == 0 && spaceStation.rooms[i].tasks.x != 0 && spaceStation.rooms[i].tasks.y != 0){         // Tasks
            return 3;
        }
        if (spaceStation.rooms[i].object.x == x && spaceStation.rooms[i].object.y == y && spaceStation.rooms[i].object.pickup == 0){         // Object
            return 4;
        }
        if (spaceStation.rooms[i].weapon.x == x && spaceStation.rooms[i].weapon.y == y && spaceStation.rooms[i].weapon.pickup == 0){            // Weapon
            return 5;
        }
    }
    return 1;
}

int Game(Dungeon* spaceStation,char* name, int seed,Character player){
    /*The game itself
    Dungeon* : spaceStation : The adress of the map (to change it)
    char* : name : The name of the player
    int : seed : The seed of the map
    Character : player : The player (the pv, xp, object, etc..)
    Returns : int : 0 if no error*/

    int k=0,choice = 0,action = 0,error=0,saved=-1;
    time_t begin=time(NULL);
    time_t pause=time(NULL);
    Door*  door;
    Task* task;
    Object* object;
    Weapon* weapon;

    halfdelay(10);

    while (choice==0){
        while (k!=27 && k!=73 && k!=105 && begin-time(NULL)+player.timer >= 0 && player.pv > 0 && spaceStation->n_tasks > player.tasksDone){     // Escape, i and I
            switch(k){
                case KEYUP:     // Movement
                    if (Movement(*spaceStation,player.x,player.y+1) != 0){
                        player.y++;
                    }
                    break;
                case KEYDOWN:
                    if (Movement(*spaceStation,player.x,player.y-1) != 0){
                        player.y--;
                    }
                    break;
                case KEYLEFT:
                    if (Movement(*spaceStation,player.x-1,player.y) != 0){
                        player.x--;
                    }
                    break;
                case KEYRIGHT:
                    if (Movement(*spaceStation,player.x+1,player.y) != 0){
                        player.x++;
                    }
                    break;
                case 10:    // Interact
                    switch (action){
                        case 3:     // Task
                            error = DoTask(spaceStation, &player,task,name,seed,begin);        // Do the task if on a task and if enter is pressed
                            if (error){
                                return error;
                            }
                            break;
                        case 4:     // Object
                            error = PickupObject(&player,object);
                            if (error){
                                return error;
                            }
                            break;
                        case 5:     // Weapon
                            error = PickupWeapon(&player,weapon);
                            if (error){
                                return error;
                            }
                            break;
                    }
                    break;
            }
            action = Movement(*spaceStation,player.x,player.y);

            if (action == 2){            // Going through a door
                error = FindDoor(*spaceStation,&door,player.x,player.y);
                if (error != 0){
                    return -1;      // If cannot create a door 
                }
                if (door->open == 0){            // If the door is closed => open it and create a new room
                    error = NewRoom(seed, spaceStation,door);
                    if (error!=0){         // Robustesse
                        return error;
                    }
                }
            }
            PrintUI(player,name,seed,spaceStation->n_tasks,begin,3);
            PrintMap(*spaceStation,player);
            if (action == 3){       // Indicate that you have to press enter to do the task
                    mvprintw(SCREENH+2,0,"Press Enter to do the task");
                    error = FindTask(*spaceStation,&task,player.x,player.y);
                    if (error){
                        return error;
                    }
            }
            else if (action == 4){     // Indicate that you have to press Enter to pick up the object
                mvprintw(SCREENH+2,0,"Press Enter to pick up object");
                error = FindObject(*spaceStation,&object,player.x,player.y);
                if (error){
                    return error;
                }
            }
            else if (action == 5){     // Indicate that you have to press Enter to pick up the weapon
                mvprintw(SCREENH+2,0,"Press Enter to pick up weapon");
                error = FindWeapon(*spaceStation,&weapon,player.x,player.y);
                if (error){
                    return error;
                }
            }

            refresh();
            k = getch();
        }

        if (begin-time(NULL)+player.timer < 0){     // If timer ends
            return PrintEnd(0,player,name,seed,begin);
        }

        if (spaceStation->n_tasks <= player.tasksDone){     // If all the tasks are done
            return PrintEnd(1,player,name,seed,begin);
        }

        if (player.pv <= 0){        // If player dies => Restart at 0,0
            player.pv = MAXPV;
            player.x = 0;
            player.y = 0;
        }

        if (k == 27){       // Pause menu
            choice = Pause(player,name,seed,spaceStation->n_tasks,&begin);

            switch (choice){
                case 0:     // Continue
                    k = 0;
                    break;
                case 1:     // Save
                    pause = time(NULL);
                    if (SearchSave(name) == -1){
                        saved = NewSave(*spaceStation,player,name,seed,begin);
                    }
                    else{
                        saved = UpdateSave(*spaceStation,player,name,seed,begin,pause);
                    }

                    if (saved != 0){
                        return saved;
                    }
                    saved = SavingChoice(player,name,seed,spaceStation->n_tasks,&begin);
                    switch(saved){
                        case 0:
                            choice = 0;
                            saved = -1;
                            k=0;
                            break;
                        case 1:     // Title screen
                            nocbreak();
                            cbreak();
                            return 1;
                        case 2:
                            clear();
                            printw("Exiting Game...\nPress any keys...");
                            getch();
                            endwin();
                            return 0;
                    }
                    break;
                case 2:     // Return title
                    nocbreak();
                    cbreak();
                    return 1;
                case 3:     // Quit
                    clear();
                    printw("Exiting Game...\nPress any keys...");
                    getch();
                    endwin();
                    return 0;
            }
            
        }

        if ((k == 73)||(k == 105)){     // Inventory
            Inventory(&player,name,seed,spaceStation->n_tasks,&begin);
            k=0;
        }
    }
    endwin();
    return 0;
}

int CreateGame(){
    /*Ask for the name and seed number.
    Returns : int : 0 if everything went well, 1 if there's an error*/

    Character player;
    Dungeon spaceStation;
    char name[NAMELIMIT+1];
    int confirm=1,len=0,k=0,error=0,seed=0;

    while (confirm){
        for (int i=1;i<NAMELIMIT+1;i++){     // Reinitialise the name
            name[i] = '\0';
        }
        len = 0;

        do{                 // Ask for seed
            clear();
            PrintUI(player,"",0,0,0,0);
            PrintLetters(2);        // print SEED
            mvprintw(SCREENI+7,SCREENL/4,"Enter your seed number (must be positive): ");
            scanw("%d",&seed);
            refresh();
        }while (seed<=0);                //!\ conditions for the seed

        do{                 // Ask name so you can see character limit (20)
            clear();
            PrintUI(player,"",seed,0,0,1);
            PrintLetters(3);        // Print NAME
            mvprintw(SCREENI+7,SCREENL/2-10,"Enter your name : ");
            if (((k<91 && k>64)||(k<58 && k>47)||(k<123 && k>96)||(k==95))&&(len<=NAMELIMIT)){       // Maj + min + num + _
                name[len] = k;
                len++;
            }
            else if ((k==263)&&(len>0)){        // If DEL
                name[len] = '\0';
                len--;
            }

            for (int i=0;i<len;i++){            // Print name chosen (stop at character limit)
                printw("%c", name[i]);
            }

            refresh();
            k = getch();
        } while ((k!=10)||(len==0));        // Until you press enter
    
        k=0;
        confirm = 0;

        do{         // Ask to confirm
            clear();
            switch(k){
                case 260:
                    confirm = 0;
                    break;
                case 261:
                    confirm = 1;
                    break;
            }
            PrintUI(player,name,seed,0,0,2);
            mvprintw(SCREENI+7,SCREENL/3,"Are the informations here correct?");
            mvprintw(SCREENI+9,SCREENL/3,"Confirm :   Yes        No");
            mvprintw(SCREENI+9,confirm*11+SCREENL/3+10,"⁕");
            mvprintw(SCREENH+1,0," ");       // Move cursor to the end

            refresh();
            k = getch();
        }while(k!=10);
    }

    player.pv = MAXPV;
    player.exp = 0;
    player.damage = 55;
    player.x = 0;
    player.y = 0;
    player.tasksDone = 0;
    player.hold = -1;
    player.timer = MAXTIME;
    for (int i=0;i<4;i++){          // Initialisation of bag of object
        player.bag.use_pouch[i].exp = 0;
        player.bag.use_pouch[i].heal = 0;
        player.bag.use_pouch[i].icon = -1;
        player.bag.use_pouch[i].pickup = 0;
        player.bag.use_pouch[i].x = 0;
        player.bag.use_pouch[i].y = 0;
    }
    for (int i=0;i<2;i++){          // Initialisation of bag of weapons
        player.bag.weaponPouch[i].icon = -1;
        player.bag.weaponPouch[i].x = 0;
        player.bag.weaponPouch[i].y = 0;
        player.bag.weaponPouch[i].pickup = 0;
        player.bag.weaponPouch[i].damage = 0;
    }

    spaceStation = FirstRoom(seed);

    if (spaceStation.total_rooms == -1){        // Malloc failed
        return 2;
    }

    error = Game(&spaceStation,name,seed,player);

    for (int i=0;i<spaceStation.nr_created;i++){        // Free everything before returning to menu
        free(spaceStation.rooms[i].doors);
    }
    free(spaceStation.rooms);

    return error;
}

int LoadSave(){
    Dungeon spaceStation;
    Character player;
    char name[NAMELIMIT+1];
    int k=0,error=0,len=0,confirm=0,cursor=0, retry=1, seed=0; 
    
    while (retry){
        while (!confirm){
            for (int i=0;i<21;i++){     // Reinitialise the name
                name[i] = '\0';
            }
            len = 0;
                    
            do{                 // Ask name so you can see character limit (20)
                clear();
                PrintUI(player,"",seed,0,0,1);
                PrintLetters(3);        // Print NAME
                mvprintw(SCREENI+7,SCREENL/2-10,"Enter your name : ");
                if (((k<91 && k>64)||(k<58 && k>47)||(k<123 && k>96)||(k==95))&&(len<NAMELIMIT)){       // Maj + min + num + _
                    name[len] = k;
                    len++;
                }
                else if ((k==263)&&(len>0)){        // If DEL
                    name[len] = '\0';
                    len--;
                }

                for (int i=0;i<len;i++){            // Print name chosen (stop at character limit)
                    printw("%c", name[i]);
                }

                refresh();
                k = getch();
            } while ((k!=10)||(len==0));        // Until you press enter
        
            error = GetSave(&spaceStation,&player,name,&seed);

            if (error == 1){
                confirm = 1;
            }
            else{
                do{
                    switch(k){
                        case KEYRIGHT:
                            cursor = 1;
                            break;
                        case KEYLEFT:
                            cursor = 0;
                            break;
                    }
                    PrintUI(player,"",0,0,0,1);
                    PrintLetters(3);        // Print NAME
                    mvprintw(SCREENI+7,SCREENL/2-16,"The name given isn't in our data base");
                    mvprintw(SCREENI+8,SCREENL/2-15,"  Retry        Return to Title");
                    mvprintw(SCREENI+8,SCREENL/2-15+cursor*13,"⁕");
                    mvprintw(SCREENH+1,0," ");

                    refresh();
                    k=getch();
                }while(k!=10);
                if (cursor){
                    return 1;                
                }
            }
        }

        do{
            clear();
            switch(k){
                case KEYRIGHT:
                    if (cursor < 2){
                        cursor++;
                    }
                    break;
                case KEYLEFT:
                    if (cursor > 0){
                        cursor--;
                    }
                    break;
            }
            PrintUI(player,"",0,0,0,1);
            PrintLetters(8);        // Print SAVE
            mvprintw(SCREENI+7,SCREENL/2-29,"Name : %s  Seed : %d   Rooms created : %d  Time left : %d",name,seed,spaceStation.nr_created,player.timer);
            mvprintw(SCREENI+8,SCREENL/2-22,"  Play       Retry      Return to Title screen");
            mvprintw(SCREENI+8,SCREENL/2-22+cursor*11,"⁕");
            mvprintw(SCREENH+1,0," ");
            refresh();
            k=getch();
        }while (k!=10);
        
        if (cursor == 0){
            retry = 0;
        }
        if (cursor == 2){
            return 0;
        }
    }

    error = Game(&spaceStation,name,seed,player);

    /*for (int i=0;i<spaceStation.nr_created;i++){        // Free everything before returning to menu
        free(spaceStation.rooms[i].doors);
    }
    free(spaceStation.rooms);

    return error;*/
}

int QuitGame(Character player){
    /*The whole concept of quitting is hard for us. So will it be for you
    Character : player : placeholder (don't worry about it it's for the UI)
    Returns : int : 1 to quit and 0 if not*/

    int quit = 0, k=0;
    while (k!=10){
        clear();
        PrintUI(player,"",0,0,0,0);
        mvprintw(SCREENI+3,SCREENL/2-13,"Do you really want to quit ?");
        mvprintw(SCREENI+4,SCREENL/2-7,"  No         Yes");
        mvprintw(SCREENI+4,quit*11+SCREENL/2-7,"⁕");
        mvprintw(SCREENH+1,0," ");
        refresh();
        k = getch();

        if (k == KEYLEFT){              // Move Left
            quit = 0;
        }
        else if (k == KEYRIGHT){        // Move Right
            quit = 1;
        }
    }
    if (quit){
        k=-1;
        quit = 0;
        while (k!=10){
            clear();
            PrintUI(player,"",0,0,0,0);
            mvprintw(SCREENI+3,SCREENL/2-23,"Are you sure you want to give up on this game ?");
            mvprintw(SCREENI+5,SCREENL/2,"  No");
            mvprintw(SCREENI+6,SCREENL/2,"  Please no");
            mvprintw(SCREENI+7,SCREENL/2,"  Yes I hate this game");
            mvprintw(quit+SCREENI+5,SCREENL/2,"⁕");
            mvprintw(SCREENH+1,0," ");
            refresh();
            k = getch();

            switch(k){
                case KEYUP:
                    if (quit>0){
                        quit--;
                    }
                    break;
                case KEYDOWN:
                    if (quit<2){
                        quit++;
                    }
                    break;
            }
        }
        if (quit==2){
            clear();
            printw("Quitting game\nPress any key...");      // Actually quitting game
            refresh();
            getch();
            return 1;
        }
    }
    return 0;
}

int TitleScreen(){
    /*Print the title screen and allows you to chose options of the menu
    Returns : int : 0 if everything went well, 1 if there's an error*/

    int cursor = 0, seed=0, k = 0,choice=1,error=0;
    char* name = NULL;
    Character player;       // Placeholder

    setlocale(LC_ALL,"");           // Set local for unicode characters
    initscr();
    cbreak();
    echo();
    keypad(stdscr, TRUE);       // To allow arrow keys

        while (k != 27){            // Until you press escape
            switch (k){
                case KEYDOWN:           // Down
                    cursor++;
                    break;
                case KEYUP:           // Up
                    cursor--;
                    break;
                case 10:            // If enter key pressed
                    switch(cursor){
                        case 0:                         // New game
                            choice = CreateGame();
                            if (choice!=1){     // if choice == 1 return to Title
                                endwin();
                                return choice;
                            }
                            break;
                        case 1:                         // Load save
                            error = LoadSave();
                            if(error == 0){
                                endwin();
                                return 0;
                            }
                            if (error != 1){
                                return error;
                            }
                            break;
                        case 2:                         // Settings
                            break;
                        case 3:
                            if (QuitGame(player)==1){           // Quit
                                endwin();
                                return 0;
                            }
                            break;
                    }
            }

            if (cursor == -1){      // To be able to go up and chose the bottom option
                cursor = 3;
            }
            cursor = cursor%4;

            clear();
            PrintUI(player,name,seed,0,0,0);    
            PrintLetters(1);        // Print Cosmic Yonder
            mvprintw(SCREENI+11,SCREENL/2-5,"  New Game");         // Different options in title screen
            mvprintw(SCREENI+12,SCREENL/2-5,"  Continue");
            mvprintw(SCREENI+13,SCREENL/2-5,"  Settings");
            mvprintw(SCREENI+14,SCREENL/2-5,"  Quit");

            mvprintw(SCREENI+11+cursor,SCREENL/2-5,"⁕");      // Shows the option your hovering on
            mvprintw(SCREENH+1,0," ");          // Move cursor to the end so it doesn't bother the screen
            refresh();

            k = getch();
        }
    endwin(); // Close window if escape is pressed
    return 0;
}