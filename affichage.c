#include "main.h"

void PrintLetters(int letter){
    /*Print letters with unicode characters
    int : letter : to chose which text to print*/

    switch (letter){
        case 1:     // Cosmic yonder
            mvprintw(SCREENI+3,SCREENL/3,"╓─╴  ╓──╖  ╓─╴       ╶╥╴ ╓─╴\n");
            mvprintw(SCREENI+4,SCREENL/3,"║    ║  ║  ╙─╖  ║╲╱║  ║  ║  \n");
            mvprintw(SCREENI+5,SCREENL/3,"╙─╴  ╙──╜  ╶─╜ ╶╨╴ ╨╴╶╨╴ ╙─╴\n");
            mvprintw(SCREENI+6,SCREENL/3+4,"      ╶╥╴ ╶╥╴\n");
            mvprintw(SCREENI+7,SCREENL/3+4,"       ╙───╢  ╓──╖       ╶╥──╮  ╥─╴  ╶╥─╮\n");
            mvprintw(SCREENI+8,SCREENL/3+4,"           ║  ║  ║  ║╲ ║  ║  │  ╟╴    ╟─╯\n");
            mvprintw(SCREENI+9,SCREENL/3+4,"          ╶╨╴ ╙──╜  ║ ╲║ ╶╨──╯  ╨─╴  ╶╨╴╲\n");
            break;
        case 2:     // Seed
            mvprintw(SCREENI+3,SCREENL/2-10,"╓─╴  ╥─╴  ╥─╴  ╶╥──╮\n");
            mvprintw(SCREENI+4,SCREENL/2-10,"╙─╖  ╟╴   ╟╴    ║  │\n");
            mvprintw(SCREENI+5,SCREENL/2-10,"╶─╜  ╨─╴  ╨─╴  ╶╨──╯\n\n");
            break;
        case 3:     // Name
            mvprintw(SCREENI+3,SCREENL/2-10,"      ╓──╖         ╥─╴\n");
            mvprintw(SCREENI+4,SCREENL/2-10,"║╲ ║  ╟──╢  ║╲╱║   ╟╴\n");
            mvprintw(SCREENI+5,SCREENL/2-10,"║ ╲║ ╶╨  ╨╴╶╨╴ ╨╴  ╨─╴\n\n");
            break;
        case 4:     // Pause
            mvprintw(SCREENI+3,SCREENL/2-12,"╶╥─╮  ╓──╖  ╥  ╥  ╓─╴ ╥─╴\n");
            mvprintw(SCREENI+4,SCREENL/2-12," ╟─╯  ╟──╢  ║  ║  ╙─╖ ╟╴\n");
            mvprintw(SCREENI+5,SCREENL/2-12,"╶╨╴  ╶╨╴ ╨╴ ╙──╜  ╶─╜ ╨─╴\n");
            break;
        case 5:     // Inventory
            mvprintw(SCREENI+3,SCREENL/2-25,"╶╥╴             ╥─╴        ┌─╥─┐  ╓──╖  ╶╥─╮  ╶╥╴ ╶╥╴\n");
            mvprintw(SCREENI+4,SCREENL/2-25," ║  ║╲ ║  ╲  ╱  ╟╴  ║╲ ║     ║    ║  ║   ╟─╯   ╙───╢\n");
            mvprintw(SCREENI+5,SCREENL/2-25,"╶╨╴ ║ ╲║   ╲╱   ╨─╴ ║ ╲║    ╶╨╴   ╙──╜  ╶╨╴╲      ╶╨╴\n");
            break;
        case 6:     // You lost
            mvprintw(SCREENI+3,SCREENL/2-25,"╶╥╴ ╶╥╴");
            mvprintw(SCREENI+4,SCREENL/2-25," ╙───╢  ╓──╖  ╥  ╥       ╶╥╴    ╓──╖  ╓─╴   ┌─╥─┐");
            mvprintw(SCREENI+5,SCREENL/2-25,"     ║  ║  ║  ║  ║        ║     ║  ║  ╙─╖     ║  ");
            mvprintw(SCREENI+6,SCREENL/2-25,"    ╶╨╴ ╙──╜  ╙──╜       ╶╨──┘  ╙──╜  ╶─╜    ╶╨╴");
            return;     // Leave to not print the thing to fix the UI
        case 7:     // Victory
            mvprintw(SCREENI+3,SCREENL/2-20,"      ╶╥╴  ╓─╴ ┌─╥─┐  ╓──╖  ╶╥─╮  ╶╥╴ ╶╥╴");
            mvprintw(SCREENI+4,SCREENL/2-20,"╲  ╱   ║   ║     ║    ║  ║   ╟─╯   ╙───╢");
            mvprintw(SCREENI+5,SCREENL/2-20," ╲╱   ╶╨╴  ╙─╴  ╶╨╴   ╙──╜  ╶╨╴╲      ╶╨╴");
            return;     // Leave to not print the thing to fix the UI
        case 8:
            mvprintw(SCREENI+3,SCREENL/2-10,"╓─╴  ╓──╖        ╥─╴");
            mvprintw(SCREENI+4,SCREENL/2-10,"╙─╖  ╟──╢  ╲  ╱  ╟╴");
            mvprintw(SCREENI+5,SCREENL/2-10,"╶─╜ ╶╨╴ ╨╴  ╲╱   ╨─╴");
            break;
    }
    for (int i=SCREENI+3;i<SCREENI+10;i++){     // Doing mvprintw sometimes erases the UI (this code to "fix" the UI)
        mvprintw(i,SCREENL,"│");
    }
    mvprintw(SCREENI+6,0,"│");

}

int PrintEnd(int victory, Character stats, char* name, int seed, time_t begin){
    /*Print the end screen
    int : victory : If you won or not (to print "Victory" or "You lost")
    Returns : int: If you choose to quit or to go back to the menu*/

    int k=0, cursor=0, endTime=time(NULL);

    nocbreak();     // Don't refresh every second (no timer)
    cbreak();
    while(k!=10){
        clear();
        if (k==KEYLEFT && cursor == 1){
            cursor = 0;
        }
        else if (k==KEYRIGHT && cursor == 0){
            cursor = 1;
        }
        if (victory){
            PrintLetters(7);
            mvprintw(SCREENI+8,SCREENL/2-25,"Name : %s  Seed : %d  Time left : %ldmin %lds  Tasks done : %d.",name,seed,(begin-endTime+stats.timer)/60,(begin-endTime+stats.timer)%60,stats.tasksDone);
            mvprintw(SCREENI+10,SCREENL/2-5,"Score : %d",stats.exp*100);
        }
        else{
            PrintLetters(6);
            mvprintw(SCREENI+8,SCREENL/2-10,"You ran out of time");
        }
        mvprintw(SCREENI+15,SCREENL/2-10,"  Quit        Return to title screen");
        mvprintw(SCREENI+15,SCREENL/2-10+cursor*12,"⁕");
        mvprintw(SCREENI+16,0," ");
        refresh();
        k=getch();
    }

    return cursor;
}

int PrintMap(Dungeon spaceStation, Character player){   
    /*Print the map (rooms, doors, tasks, object and weapons)
    Dungeon : spaceStation : The map to print
    Character : player : The player with its coordinates (for relative position)
    Returns : int : 0 if no error*/

    for (int i=0;i<spaceStation.nr_created;i++){
        for (int j=0;j<spaceStation.rooms[i].L;j++){    // Top and bottom wall
            if ((spaceStation.rooms[i].x+j-player.x < SCREENL/2 && spaceStation.rooms[i].x+j-player.x> -SCREENL/2) && (spaceStation.rooms[i].y - player.y < (SCREENH-SCREENI)/2 && spaceStation.rooms[i].y - player.y +1 > -(SCREENH-SCREENI)/2)){
                mvprintw(player.y-spaceStation.rooms[i].y+SCREENI+(SCREENH-SCREENI)/2,spaceStation.rooms[i].x+j-player.x+SCREENL/2,"█");
            }
            if ((spaceStation.rooms[i].x+j-player.x < SCREENL/2 && spaceStation.rooms[i].x+j-player.x>-SCREENL/2)&&(spaceStation.rooms[i].y - player.y + spaceStation.rooms[i].H -1 < (SCREENH-SCREENI)/2 && spaceStation.rooms[i].y - player.y + spaceStation.rooms[i].H > -(SCREENH-SCREENI)/2)){
                mvprintw(player.y-spaceStation.rooms[i].y +SCREENI+(SCREENH-SCREENI)/2 - spaceStation.rooms[i].H+1,spaceStation.rooms[i].x+j-player.x+SCREENL/2,"█");
            }
        }
        for (int j=0;j<spaceStation.rooms[i].H;j++){    // Left and right wall
            if ((spaceStation.rooms[i].x - player.x < SCREENL/2 && spaceStation.rooms[i].x - player.x > -SCREENL/2) && (spaceStation.rooms[i].y - player.y + spaceStation.rooms[i].H - 1 - j < (SCREENH-SCREENI)/2 && spaceStation.rooms[i].y - player.y- j +spaceStation.rooms[i].H> -(SCREENH-SCREENI)/2)){
                mvprintw(player.y-spaceStation.rooms[i].y+SCREENI+(SCREENH-SCREENI)/2+j-spaceStation.rooms[i].H+1, spaceStation.rooms[i].x-player.x+SCREENL/2,"█");
            }
            if ((spaceStation.rooms[i].x - player.x + spaceStation.rooms[i].L -1 < SCREENL/2 && spaceStation.rooms[i].x - player.x  + spaceStation.rooms[i].L-1 > -SCREENL/2)&&(spaceStation.rooms[i].y - player.y + spaceStation.rooms[i].H - 1 - j < (SCREENH-SCREENI)/2 && spaceStation.rooms[i].y - player.y- j +spaceStation.rooms[i].H > -(SCREENH-SCREENI)/2)){
                mvprintw(player.y-spaceStation.rooms[i].y+SCREENI+(SCREENH-SCREENI)/2+j-spaceStation.rooms[i].H+1, spaceStation.rooms[i].x-player.x+SCREENL/2+spaceStation.rooms[i].L-1,"█");
            }
        }
        for (int j=0;j<spaceStation.rooms[i].n_doors;j++){  // Doors
            if ((spaceStation.rooms[i].doors[j].x - player.x < SCREENL/2 && spaceStation.rooms[i].doors[j].x - player.x > -SCREENL/2)&&(spaceStation.rooms[i].doors[j].y - player.y < (SCREENH-SCREENI)/2 && spaceStation.rooms[i].doors[j].y - player.y +1 > -(SCREENH-SCREENI)/2)){
                if ((spaceStation.rooms[i].doors[j].wall == 0)||(spaceStation.rooms[i].doors[j].wall == 2)){
                    mvprintw(player.y-spaceStation.rooms[i].doors[j].y+SCREENI+(SCREENH-SCREENI)/2, -player.x+spaceStation.rooms[i].doors[j].x+SCREENL/2,"━");      
                }
                if ((spaceStation.rooms[i].doors[j].wall == 1)||(spaceStation.rooms[i].doors[j].wall == 3)){
                    mvprintw(player.y-spaceStation.rooms[i].doors[j].y+SCREENI+(SCREENH-SCREENI)/2,-player.x+spaceStation.rooms[i].doors[j].x+SCREENL/2,"┃");
                }
            }
        }

        if (spaceStation.rooms[i].tasks.x != 0 || spaceStation.rooms[i].tasks.y != 0){      // If we have to print the tasks
            if ((spaceStation.rooms[i].tasks.x - player.x < SCREENL/2 && spaceStation.rooms[i].tasks.x - player.x > -SCREENL/2)&&(spaceStation.rooms[i].tasks.y - player.y < (SCREENH-SCREENI)/2 && spaceStation.rooms[i].tasks.y - player.y > -(SCREENH-SCREENI)/2)){
                if (spaceStation.rooms[i].tasks.done){      // The task is done
                    mvprintw(player.y-spaceStation.rooms[i].tasks.y + (SCREENH-SCREENI)/2 + SCREENI,spaceStation.rooms[i].tasks.x - player.x+SCREENL/2,"◈");
                }
                else{       // The task is yet to be done
                    mvprintw(player.y-spaceStation.rooms[i].tasks.y + (SCREENH-SCREENI)/2 + SCREENI,spaceStation.rooms[i].tasks.x - player.x+SCREENL/2,"◇");
                }
            }
        }
        if ((spaceStation.rooms[i].object.x != 0 || spaceStation.rooms[i].object.y != 0) && spaceStation.rooms[i].object.pickup == 0){      // If there is a non-picked up object
            if ((spaceStation.rooms[i].object.x - player.x < SCREENL/2 && spaceStation.rooms[i].object.x - player.x > -SCREENL/2)&&(spaceStation.rooms[i].object.y - player.y < (SCREENH-SCREENI)/2 && spaceStation.rooms[i].object.y - player.y > -(SCREENH-SCREENI)/2)){
                switch(spaceStation.rooms[i].object.icon){
                    case 0:
                        mvprintw(player.y-spaceStation.rooms[i].object.y + (SCREENH-SCREENI)/2 + SCREENI,spaceStation.rooms[i].object.x - player.x+SCREENL/2,"✚");
                        break;
                    case 2:
                        mvprintw(player.y-spaceStation.rooms[i].object.y + (SCREENH-SCREENI)/2 + SCREENI,spaceStation.rooms[i].object.x - player.x+SCREENL/2,"🕮");
                        break;
                    case 1:
                        mvprintw(player.y-spaceStation.rooms[i].object.y + (SCREENH-SCREENI)/2 + SCREENI,spaceStation.rooms[i].object.x - player.x+SCREENL/2,"⚗");
                        break;
                }
                
            }
        }
        if ((spaceStation.rooms[i].weapon.x != 0 || spaceStation.rooms[i].weapon.y != 0) && spaceStation.rooms[i].weapon.pickup == 0){      // If there is a non-picked up weapon
            if ((spaceStation.rooms[i].weapon.x - player.x < SCREENL/2 && spaceStation.rooms[i].weapon.x - player.x > -SCREENL/2)&&(spaceStation.rooms[i].weapon.y - player.y < (SCREENH-SCREENI)/2 && spaceStation.rooms[i].weapon.y - player.y > -(SCREENH-SCREENI)/2)){
                switch(spaceStation.rooms[i].weapon.icon){
                    case 0:
                        mvprintw(player.y-spaceStation.rooms[i].weapon.y + (SCREENH-SCREENI)/2 + SCREENI,spaceStation.rooms[i].weapon.x - player.x+SCREENL/2,"⚔");
                        break;
                    case 1:
                        mvprintw(player.y-spaceStation.rooms[i].weapon.y + (SCREENH-SCREENI)/2 + SCREENI,spaceStation.rooms[i].weapon.x - player.x+SCREENL/2,"⚒");
                        break;
                }
                
            }
        }
    }
    mvprintw((SCREENH-SCREENI)/2+SCREENI,SCREENL/2,"☺");        // Prints the player
    mvprintw(SCREENH+2,0," ");                                  // Get the cursor out of the way (down)

    return 0;
}

int PrintUI(Character stats, char* name,int seed, int tasks, time_t timeBegin, int step){
    /*Print the User Interface (PV, the box, name, etc..)
    Character : stats : The player (the pv, xp, object, etc..)
    char* : name : The name of the player
    int : seed : The seed of the map
    int : tasks : numbder of tasks to do
    time_t : timeBegin : The timer going down
    int : step : Which step it is on (when creating the game, the name and seed aren't chosen yet, but we still print the UI)
    Returns : int : 0 if no error*/

    clear();
    switch(step){
        case 0:          // During creating game part
            printw("\n\n  Name :\n\n  Seed :");
            break;
        case 1:
            printw("\n\n  Name : \n\n  Seed : %d",seed);
            break;
        case 2:
            printw("\n\n  Name : %s\n\n  Seed : %d",name,seed);
            break;
        case 3:
            printw("\n\n  Name : %s\n\n  Seed : %d",name,seed);
            mvprintw(4,SCREENL/5*3,"Tasks: %d/%d",stats.tasksDone,tasks);
            mvprintw(2,SCREENL/5*3,"Obj:");
            if (stats.hold != -1){
                if (stats.bag.weaponPouch[stats.hold].icon == 1){
                    mvprintw(2,SCREENL/5*3,"Obj: R. Tools");
                }
                else if (stats.bag.weaponPouch[stats.hold].icon == 0){
                    mvprintw(2,SCREENL/5*3,"Obj: L. Sword");
                }
            } 
            mvprintw(2,SCREENL/5*4+5,"Timer : ");      // Timer
            mvprintw(3,SCREENL/5*4+6,"%ld",timeBegin-time(NULL)+stats.timer);
            break;
    }

    mvprintw(0,0,"┌");              // Croners
    mvprintw(0,SCREENL,"┐");
    mvprintw(SCREENH,0,"└");
    mvprintw(SCREENH,SCREENL,"┘");

    for(int i=1;i<SCREENL;i++){     // The horizontal borders
        mvprintw(0,i,"─");
        mvprintw(SCREENH,i,"─");
        mvprintw(SCREENI,i,"─");
    }

    for(int i=1;i<SCREENH;i++){     // The vertical borders
        mvprintw(i,0,"│");
        mvprintw(i,SCREENL,"│");
    }

    for (int i=1; i<SCREENI;i++){           // The little box for the timer
        mvprintw(i,SCREENL/5*4,"│");
    }
    mvprintw(SCREENI,0,"├");
    mvprintw(SCREENI,SCREENL,"┤");
    mvprintw(0,SCREENL/5*4,"┬");
    mvprintw(SCREENI,SCREENL/5*4,"┴");

    if (step > 2){
        mvprintw(2,SCREENL/3,"PV : ");
        mvprintw(4,SCREENL/3,"XP : %d/1000",stats.exp%1000);    // XP bar
        mvprintw(4,SCREENL/2,"LV : %d",stats.exp/1000+1);  // Levels


        for (int i=0;i<BARPV;i++){         // Health bar
            mvprintw(2,SCREENL/3+5+i,"▧");
        }
        for (int i=0;i<(stats.pv*BARPV)/MAXPV;i++){
            mvprintw(2,SCREENL/3+5+i,"▆");
        }
        mvprintw(SCREENH+1,0,"Press Echap to pause ; i for inventory ; Enter to use ; Use arrow keys to move ; Space bar to attack");
    }

    refresh();
    return 0;
}

void PrintInventory(Character* player,char* name,int seed, int tasks, time_t *begin, int cursorobj){
    /*Prints the inventory
    Character* : player : The adress of infos of the player (to change the bag)
    char* : name : int : seed, int : tasks, time_t* : begin, to print the UI
    int : cursorobj : Which object the cursor is on*/

    clear();
    PrintUI(*player,name,seed,tasks,*begin,3);
    PrintLetters(5);            // INVENTORY
    for (int i=0;i<SCREENH-SCREENI-7;i++){      // Middle bar
        mvprintw(SCREENI+7+i,SCREENL/2,"│");
    }
    mvprintw(SCREENI+8,SCREENL/4-4,"Weapons");
    mvprintw(SCREENI+8,SCREENL/4*3-4,"Objects");

    for (int i=0;i<4;i++){      // Objects
        if (player->bag.use_pouch[i].pickup == 1){
            switch(player->bag.use_pouch[i].icon){
                case 1:
                    mvprintw(SCREENI+10+i*2,SCREENL/2+3,"▢  Strenght potion     ⚗");
                    break;
                case 2:
                    mvprintw(SCREENI+10+i*2,SCREENL/2+3,"▢  Book of knowledge   🕮");
                    break;
                case 0:
                    mvprintw(SCREENI+10+i*2,SCREENL/2+3,"▢  Healing potion      ✚");
                    break;
            }
        }
        else{
            mvprintw(SCREENI+10+i*2,SCREENL/2+3,"▢ ");
        }
    }
    for (int i=0;i<2;i++){      // Weapons
        if (player->bag.weaponPouch[i].pickup == 1){
            switch(player->bag.weaponPouch[i].icon){
                case 1:
                    mvprintw(SCREENI+10+i*4,3,"▢  Reparing tools  ⚒");
                    break;
                case 0:
                    mvprintw(SCREENI+10+i*4,3,"▢  Laser Sword     ⚔");
                    break;
            }
        }
        else{
            mvprintw(SCREENI+10+i*4,3,"▢ ");
        }
    }
    
    for (int i=0;i<2;i++){      // Choices
        mvprintw(SCREENH-4,3+SCREENL/2*i,"  Info");
        mvprintw(SCREENH-2,3+SCREENL/2*i,"  Drop");
    }

    mvprintw(SCREENH-6,3+SCREENL/2,"  Use");
    mvprintw(SCREENH-8,3,"  Equip");
    mvprintw(SCREENH-6,3,"  Unequip");

    if (cursorobj<0){                   // Weapon side (left)
        mvprintw(SCREENI+10-(cursorobj+1)*4,3,"▣ ");       // Prints the choice made by the cursor 
    }
    else{       // Object side (right)
        mvprintw(SCREENI+10+(cursorobj-1)*2,SCREENL/2+3,"▣ ");
    }
}