#include "main.h"

int Rand(int seed, int x, int y){            // x value of incremantation and y the number of loops
    /*Uses the seed to make "random" number
    int : seed : The seed 
    int : x : A coordonate
    int : y : Another coordonate
    Returns a "random" number*/

    if (y<=0){
        return seed;
    }
    return Rand(((seed+1650*y)*x)%32767,x,y-1);     // Don't think about it too much, it's random
}

int FindDoor(Dungeon spaceStation, Door** door, int x, int y){
    /*Used to find a door for a given set of coordinates
    Dungeon : spaceStation : The map
    Door ** : door : adress of a door (where we return the adress of the door found)
    int : x : The x coordinate
    int : y : The y coordinate
    Returns : int : 0 if the door was found,-1 if you haven't found the door)*/

    for (int i=0;i<spaceStation.nr_created;i++){
        for (int j=0;j<spaceStation.rooms[i].n_doors;j++){
            if (spaceStation.rooms[i].doors[j].x == x && spaceStation.rooms[i].doors[j].y == y){
                *door = &(spaceStation.rooms[i].doors[j]);
                return 0;
            }
        }
    }
    return -1;
}

int FindTask(Dungeon spaceStation, Task** task, int x, int y){
    /*Used to find a task for a given set of coordinates
    Dungeon : spaceStation : The map
    Task ** : door : adress of a task (where we return the adress of the task found)
    int : x : The x coordinate
    int : y : The y coordinate
    Returns : int : 0 if the door was found,-1 if you haven't found the task)*/

    for (int i=0;i<spaceStation.nr_created;i++){
        if (spaceStation.rooms[i].tasks.x == x && spaceStation.rooms[i].tasks.y == y){
            *task = &spaceStation.rooms[i].tasks;
            return 0;
        }
    }
    return -1;
}

int FindObject(Dungeon spaceStation, Object** object, int x, int y){
    /*Used to find a object for a given set of coordinates
    Dungeon : spaceStation : The map
    Object ** : door : adress of a object (where we return the adress of the object found)
    int : x : The x coordinate
    int : y : The y coordinate
    Returns : int : 0 if the door was found,-1 if you haven't found the object)*/

    for (int i=0;i<spaceStation.nr_created;i++){
        if (spaceStation.rooms[i].object.x == x && spaceStation.rooms[i].object.y == y){
            *object = &spaceStation.rooms[i].object;
            return 0;
        }
    }
    return -1;
}

int FindWeapon(Dungeon spaceStation, Weapon** weapon, int x, int y){
    /*Used to find a weapon for a given set of coordinates
    Dungeon : spaceStation : The map
    Weapon ** : door : adress of a weapon (where we return the adress of the weapon found)
    int : x : The x coordinate
    int : y : The y coordinate
    Returns : int : 0 if the door was found,-1 if you haven't found the weapon)*/

    for (int i=0;i<spaceStation.nr_created;i++){
        if (spaceStation.rooms[i].weapon.x == x && spaceStation.rooms[i].weapon.y == y){
            *weapon = &spaceStation.rooms[i].weapon;
            return 0;
        }
    }
    return -1;
}

Dungeon FirstRoom(int seed){
    /*Create the first room (4 doors, 0,0 in the middle of it)
    int : seed : The seed entered by the player (to do procedural generation)
    Returns : Dungeon : the structure of the dungeon with the first room created*/

    Dungeon spaceStation;
    spaceStation.n_tasks = Rand(seed,10,10)%11 + MIN_TASKS;                     // Number of all the tasks between 10 and 20
    spaceStation.n_tasks2create = spaceStation.n_tasks;
    spaceStation.total_rooms = Rand(seed, 11,10)%5 + spaceStation.n_tasks + 1;      // More rooms than tasks (no tasks  in first room)
    spaceStation.nr_created = 1;                                                // The first room that is being created
    spaceStation.nd_left = spaceStation.total_rooms - 5;                        // 1: first room ; 4 : the doors on the first room

    spaceStation.rooms = NULL;
    spaceStation.rooms = malloc(spaceStation.total_rooms*sizeof(Room));

    if (spaceStation.rooms == NULL){        // Robustesse
        spaceStation.total_rooms = 2;
        return spaceStation;
    }

    spaceStation.rooms->H = Rand(seed,50,51)%MAXH+3;              // H and L between 3 and 20
    spaceStation.rooms->L = Rand(seed,50,52)%MAXL+3;
    spaceStation.rooms->x = -(spaceStation.rooms->L)/2;                 // The middle is 0,0
    spaceStation.rooms->y = -(spaceStation.rooms->H)/2;

    spaceStation.rooms->tasks.x = 0;            // No tasks, object or weapons in the first room
    spaceStation.rooms->tasks.y = 0;
    spaceStation.rooms->tasks.done = 1;

    spaceStation.rooms->object.x = 0;
    spaceStation.rooms->object.y = 0;
    spaceStation.rooms->object.pickup = 1;
    spaceStation.rooms->object.exp = 0;
    spaceStation.rooms->object.heal = 0;
    spaceStation.rooms->object.icon = -1;
    spaceStation.rooms->weapon.x = 0;
    spaceStation.rooms->weapon.y = 0;
    spaceStation.rooms->weapon.pickup = 1;
    spaceStation.rooms->weapon.damage = 0;
    spaceStation.rooms->weapon.icon = -1;

    spaceStation.rooms->n_doors = 4;

    spaceStation.rooms->doors = NULL;
    spaceStation.rooms->doors = malloc(spaceStation.rooms[0].n_doors*sizeof(Door));

    if (spaceStation.rooms->doors == NULL){ // Robustesse
        spaceStation.total_rooms = 2;
        return spaceStation;
    }

    for (int i=0; i<spaceStation.rooms->n_doors;i++){       // Creation of the 4 doors
        spaceStation.rooms->doors[i].wall = i;
        spaceStation.rooms->doors[i].open = 0;      // All doors are closed
        switch (i){
            case 0:     // Top wall
                spaceStation.rooms->doors[i].x = Rand(seed,20,20)%(spaceStation.rooms->L-2)+spaceStation.rooms->x + 1;
                spaceStation.rooms->doors[i].y = spaceStation.rooms->H + spaceStation.rooms->y -1;
                break;
            case 1:     // Right wall
                spaceStation.rooms->doors[i].x = spaceStation.rooms->L + spaceStation.rooms->x -1;
                spaceStation.rooms->doors[i].y = Rand(seed,20,21)%(spaceStation.rooms->H-2)+spaceStation.rooms->y + 1;
                break;
            case 2:     // Bottom wall
                spaceStation.rooms->doors[i].x = Rand(seed,21,20)%(spaceStation.rooms->L-2)+spaceStation.rooms->x + 1;
                spaceStation.rooms->doors[i].y = spaceStation.rooms->y;
                break;
            case 3:     // Left wall
                spaceStation.rooms->doors[i].x = spaceStation.rooms->x;
                spaceStation.rooms->doors[i].y = Rand(seed,22,20)%(spaceStation.rooms->H-2)+spaceStation.rooms->y + 1;
                break;
        }
    }

    return spaceStation;
}

int CanDoorGoThere(Dungeon spaceStation, int x, int y, int wall){
    /*Check if there's enough space to fit a room and place a door
    Dungeon : spaceStation : the map itself
    int : x : The x coordinate of the door
    int : y : The y coordinate of the door
    int : wall : which wall the door is on (up, right, etc..)
    Returns 1 if a door can do here, and 0 if it cannot */

    for (int i=0;i<spaceStation.nr_created;i++){            // Go through every room
        for (int j=0;j<3;j++){                              // Check if a 3x3 area is available (technically 3x2 since walls are shared)
            for (int k=0;k<2;k++){ 
                switch (wall){
                    case 0:     // Top wall
                        if ((x-1+j >= (spaceStation.rooms+i)->x)&&(x-1+j < (spaceStation.rooms+i)->x + (spaceStation.rooms+i)->L)&&(y+1+k >= (spaceStation.rooms+i)->y)&&(y+1+k <= (spaceStation.rooms+i)->y + (spaceStation.rooms+i)->H)){
                            return 0;
                        }
                        break;
                    case 1:     // Right wall
                        if ((x+1+k >= (spaceStation.rooms+i)->x)&&(x+1+k < (spaceStation.rooms+i)->x + (spaceStation.rooms+i)->L)&&(y-1+j >= (spaceStation.rooms+i)->y)&&(y-1+j < (spaceStation.rooms+i)->y + (spaceStation.rooms+i)->H)){
                            return 0;
                        }
                        break;
                    case 2:     // Bottom wall
                        if ((x-1+j >= (spaceStation.rooms+i)->x)&&(x-1+j < (spaceStation.rooms+i)->x + (spaceStation.rooms+i)->L)&&(y-1-k >= (spaceStation.rooms+i)->y)&&(y-1-k <= (spaceStation.rooms+i)->y + (spaceStation.rooms+i)->H)){
                            return 0;
                        }
                        break;
                    case 3:     // Left wall
                        if ((x-1-k >= (spaceStation.rooms+i)->x)&&(x-1-k <= (spaceStation.rooms+i)->x + (spaceStation.rooms+i)->L)&&(y-1+j >= (spaceStation.rooms+i)->y)&&(y-1+j < (spaceStation.rooms+i)->y + (spaceStation.rooms+i)->H)){
                            return 0;
                        }
                        break;
                }
            }
        }
        for (int j=0;j<3;j++){              // Check all the potential rooms (doors)
            for (int k=0;k<2;k++){
                switch(wall){
                    case 0:
                        for (int l=0;l<spaceStation.rooms[i].n_doors;l++){
                            if (spaceStation.rooms[i].doors[l].open == 0){          // If the door is closed (not a room already)
                                switch(spaceStation.rooms[i].doors[l].wall){        // Check which wall the door checked is on
                                    case 0:
                                        if (((x-1+j >= spaceStation.rooms[i].doors[l].x-1)&&(x-1+j <= spaceStation.rooms[i].doors[l].x+1))&&((y+1+k >= spaceStation.rooms[i].doors[l].y+1)&&(y+1+k <= spaceStation.rooms[i].doors[l].y+2))){
                                            return 0;
                                        }
                                        break;
                                    case 1:
                                        if (((x-1+j >= spaceStation.rooms[i].doors[l].x+1)&&(x-1+j <= spaceStation.rooms[i].doors[l].x+2))&&((y+1+k >=spaceStation.rooms[i].doors[l].y-1)&&(y+1+k <=spaceStation.rooms[i].doors[l].y+1))){
                                            return 0;
                                        }
                                        break;
                                    case 2:
                                        if (((x-1+j >=spaceStation.rooms[i].doors[l].x-1)&&(x-1+j <= spaceStation.rooms[i].doors[l].x+1))&&((y+1+k >=spaceStation.rooms[i].doors[l].y-2)&&(y+1+k <=spaceStation.rooms[i].doors[l].y-1))){
                                            return 0;
                                        }
                                        break;
                                    case 3:
                                        if (((x-1+j >=spaceStation.rooms[i].doors[l].x-2)&&(x-1+j <= spaceStation.rooms[i].doors[l].x-1))&&((y+1+k >=spaceStation.rooms[i].doors[l].y-1)&&(y+1+k <=spaceStation.rooms[i].doors[l].y+1))){
                                            return 0;
                                        }
                                        break;
                                }
                            }
                        }
                        break;
                    case 1:
                        for (int l=0;l<spaceStation.rooms[i].n_doors;l++){
                            if (spaceStation.rooms[i].doors[l].open == 0){    
                                switch(spaceStation.rooms[i].doors[l].wall){        // Check which wall the door checked is on
                                    case 0:
                                        if (((x+1+k >= spaceStation.rooms[i].doors[l].x-1)&&(x+1+k <= spaceStation.rooms[i].doors[l].x+1))&&((y-1+j >= spaceStation.rooms[i].doors[l].y+1)&&(y-1+j <= spaceStation.rooms[i].doors[l].y+2))){
                                            return 0;
                                        }
                                        break;
                                    case 1:
                                        if (((x+1+k >= spaceStation.rooms[i].doors[l].x+1)&&(x+1+k <= spaceStation.rooms[i].doors[l].x+2))&&((y-1+j >=spaceStation.rooms[i].doors[l].y-1)&&(y-1+j <=spaceStation.rooms[i].doors[l].y+1))){
                                            return 0;
                                        }
                                        break;
                                    case 2:
                                        if (((x+1+k >=spaceStation.rooms[i].doors[l].x-1)&&(x+1+k <= spaceStation.rooms[i].doors[l].x+1))&&((y-1+j >=spaceStation.rooms[i].doors[l].y-2)&&(y-1+j <=spaceStation.rooms[i].doors[l].y-1))){
                                            return 0;
                                        }
                                        break;
                                    case 3:
                                        if (((x+1+k >=spaceStation.rooms[i].doors[l].x-2)&&(x+1+k <= spaceStation.rooms[i].doors[l].x-1))&&((y-1+j >=spaceStation.rooms[i].doors[l].y-1)&&(y-1+j <=spaceStation.rooms[i].doors[l].y+1))){
                                            return 0;
                                        }
                                        break;
                                }
                            } 
                        }
                        break;
                    case 2:
                        for (int l=0;l<spaceStation.rooms[i].n_doors;l++){
                            if (spaceStation.rooms[i].doors[l].open == 0){
                                switch(spaceStation.rooms[i].doors[l].wall){        // Check which wall the door checked is on
                                    case 0:
                                        if (((x-1+j >= spaceStation.rooms[i].doors[l].x-1)&&(x-1+j <= spaceStation.rooms[i].doors[l].x+1))&&((y-1-k >= spaceStation.rooms[i].doors[l].y+1)&&(y-1-k <= spaceStation.rooms[i].doors[l].y+2))){
                                            return 0;
                                        }
                                        break;
                                    case 1:
                                        if (((x-1+j >= spaceStation.rooms[i].doors[l].x+1)&&(x-1+j <= spaceStation.rooms[i].doors[l].x+2))&&((y-1-k >=spaceStation.rooms[i].doors[l].y-1)&&(y-1-k <=spaceStation.rooms[i].doors[l].y+1))){
                                            return 0;
                                        }
                                        break;
                                    case 2:
                                        if (((x-1+j >=spaceStation.rooms[i].doors[l].x-1)&&(x-1+j <= spaceStation.rooms[i].doors[l].x+1))&&((y-1-k >=spaceStation.rooms[i].doors[l].y-2)&&(y-1-k <=spaceStation.rooms[i].doors[l].y-1))){
                                            return 0;
                                        }
                                        break;
                                    case 3:
                                        if (((x-1+j >=spaceStation.rooms[i].doors[l].x-2)&&(x-1+j <= spaceStation.rooms[i].doors[l].x-1))&&((y-1-k >=spaceStation.rooms[i].doors[l].y-1)&&(y-1-k <=spaceStation.rooms[i].doors[l].y+1))){
                                            return 0;
                                        }
                                        break;
                                }
                            }       
                        }
                        break;
                    case 3:
                        for (int l=0;l<spaceStation.rooms[i].n_doors;l++){
                            if (spaceStation.rooms[i].doors[l].open == 0){
                                switch(spaceStation.rooms[i].doors[l].wall){        // Check which wall the door checked is on
                                    case 0:
                                        if (((x-1-k >= spaceStation.rooms[i].doors[l].x-1)&&(x-1-k <= spaceStation.rooms[i].doors[l].x+1))&&((y-1+j >= spaceStation.rooms[i].doors[l].y+1)&&(y-1+j <= spaceStation.rooms[i].doors[l].y+2))){
                                            return 0;
                                        }
                                        break;
                                    case 1:
                                        if (((x-1-k >= spaceStation.rooms[i].doors[l].x+1)&&(x-1-k <= spaceStation.rooms[i].doors[l].x+2))&&((y-1+j >=spaceStation.rooms[i].doors[l].y-1)&&(y-1+j <=spaceStation.rooms[i].doors[l].y+1))){
                                            return 0;
                                        }
                                        break;
                                    case 2:
                                        if (((x-1-k >=spaceStation.rooms[i].doors[l].x-1)&&(x-1-k <= spaceStation.rooms[i].doors[l].x+1))&&((y-1+j >=spaceStation.rooms[i].doors[l].y-2)&&(y-1+j <=spaceStation.rooms[i].doors[l].y-1))){
                                            return 0;
                                        }
                                        break;
                                    case 3:
                                        if (((x-1-k >=spaceStation.rooms[i].doors[l].x-2)&&(x-1-k <= spaceStation.rooms[i].doors[l].x-1))&&((y-1+j >=spaceStation.rooms[i].doors[l].y-1)&&(y-1+j <=spaceStation.rooms[i].doors[l].y+1))){
                                            return 0;
                                        }
                                        break;
                                }
                            } 
                        }
                        break;
                }
            }
        }
    }
    return 1;
}

int Overlap(Dungeon spaceStation, int x,int y, int L, int H, Door door){
    /*Check if a given room overlaps with another room in the dungeon
    Dungeon : spaceStation : the map
    int : x : The coordinate of the bottom left corner of the room (horizontal)
    int : y : The coordinate of the bottom left corner of the room (vertical)
    int : L : The lenght of the room
    int : H : The height of the room
    Door : door : The door we're coming through
    Returns : int : 1 if it overlaps, 0 if not*/

    for (int i=0;i<spaceStation.nr_created;i++){    // Go through all rooms and check if it overlaps
        for (int j=0;j<L;j++){      // Top and bottom walls
            if (((spaceStation.rooms[i].x<x+j)&&(spaceStation.rooms[i].x+spaceStation.rooms[i].L-1>x+j))&&((spaceStation.rooms[i].y<y)&&(spaceStation.rooms[i].y+spaceStation.rooms[i].H-1>y))){
                return 1;
            }
            if (((spaceStation.rooms[i].x<x+j)&&(spaceStation.rooms[i].x+spaceStation.rooms[i].L-1>x+j))&&((spaceStation.rooms[i].y<y+H-1)&&(spaceStation.rooms[i].y+spaceStation.rooms[i].H-1>y+H-1))){
                return 1;
            }
        }
        for (int j=0;j<H;j++){      // Right and left walls
            if (((spaceStation.rooms[i].x<x)&&(spaceStation.rooms[i].x+spaceStation.rooms[i].L-1>x))&&((spaceStation.rooms[i].y<y+j)&&(spaceStation.rooms[i].y+spaceStation.rooms[i].H-1>y+j))){
                return 1;
            }
            if (((spaceStation.rooms[i].x<x+L-1)&&(spaceStation.rooms[i].x+spaceStation.rooms[i].L-1>x+L-1))&&((spaceStation.rooms[i].y<y+j)&&(spaceStation.rooms[i].y+spaceStation.rooms[i].H-1>y+j))){
                return 1;
            }
        }

        for (int j=0;j<spaceStation.rooms[i].n_doors;j++){              // Check all the potential rooms (doors and the space they need)
            if ((door.x == spaceStation.rooms[i].doors[j].x)&&(door.y == spaceStation.rooms[i].doors[j].y)&&(door.wall == spaceStation.rooms[i].doors[j].wall)||(spaceStation.rooms[i].doors[j].open == 1)){    // If the door is the one we're entring from or already a room
                continue;
            }
            for (int l=0;l<3;l++){
                for (int k=0;k<2;k++){
                    switch(spaceStation.rooms[i].doors[j].wall){
                        case 0:
                            if (((spaceStation.rooms[i].doors[j].x-1+l<=x+L-1)&&(spaceStation.rooms[i].doors[j].x-1+l>=x))&&((y<=spaceStation.rooms[i].doors[j].y+1+k)&&(spaceStation.rooms[i].doors[j].y+1+k<=y+H-1))){
                                return 1;
                            }
                            break;
                        case 1:
                            if (((spaceStation.rooms[i].doors[j].x+1+k<=x+L-1)&&(spaceStation.rooms[i].doors[j].x+1+k>=x))&&((y<=spaceStation.rooms[i].doors[j].y-1+l)&&(spaceStation.rooms[i].doors[j].y-1+l<=y+H-1))){
                                return 1;
                            }
                            break;
                        case 2:
                            if (((spaceStation.rooms[i].doors[j].x-1+l<=x+L-1)&&(spaceStation.rooms[i].doors[j].x-1+l>=x))&&((y<=spaceStation.rooms[i].doors[j].y-1-k)&&(spaceStation.rooms[i].doors[j].y-1-k<=y+H-1))){
                                return 1;
                            }
                            break;
                        case 3:
                            if (((spaceStation.rooms[i].doors[j].x-1-k<=x+L-1)&&(spaceStation.rooms[i].doors[j].x-1-k>=x))&&((y<=spaceStation.rooms[i].doors[j].y-1+l)&&(spaceStation.rooms[i].doors[j].y-1+l<=y+H-1))){
                                return 1;
                            }
                            break;
                    }
                }
            }
        }
    }
    return 0;
}

int NewRoom(int seed, Dungeon* spaceStation, Door* door){
    /*Creates a new room from a door we go through
    int : seed : The seed of the map
    Dungeon* : spaceStation : The adress of the map
    Door* : door : The adress of the door we're entering from (to change from closed to open)
    Returns : int : 0 if no error, 2 if an error of malloc, -1 if you can't create another door*/
   
    int maxDoors=0, flag=1, j=1, enteringWall=-1, flag0=0,flag1=0,flag2=0,flag3=0,count=1,m=0,l=0;

    if (spaceStation->nr_created == spaceStation->total_rooms){  // If there's no rooms left to be made (shouldn't happend)
        return -1;
    }

    switch(door->wall){      // The corresponding wall of the entering door
        case 0:
            enteringWall = 2;
            break;
        case 1:
            enteringWall = 3;
            break;
        case 2:
            enteringWall = 0;
            break;
        case 3:
            enteringWall = 1;
            break;
    }

    switch(door->wall){          // Creation of the dimension and coordinates of the room
        case 0:     // Takes Rand dimension, and check if it overlaps, if it doesn't then it's good
            do{
                if ((Rand(seed,75,j*spaceStation->nr_created)%MAXL+1 < Rand(seed,250,j*spaceStation->nr_created)%MAXL+2)&&(!Overlap(*spaceStation,door->x-Rand(seed,75,j*spaceStation->nr_created)%MAXL-1,door->y,Rand(seed,250,j*spaceStation->nr_created)%MAXL+3,Rand(seed,j*spaceStation->nr_created,300)%MAXH+3,*door))){      // First condition : door is connected
                    spaceStation->rooms[spaceStation->nr_created].x = door->x-Rand(seed,75,j*spaceStation->nr_created)%MAXL-1;
                    spaceStation->rooms[spaceStation->nr_created].y = door->y;
                    spaceStation->rooms[spaceStation->nr_created].L = Rand(seed,250,j*spaceStation->nr_created)%MAXL+3;
                    spaceStation->rooms[spaceStation->nr_created].H = Rand(seed,j*spaceStation->nr_created,300)%MAXH+3;
                    flag = 0;
                }
                j++;
            }while(flag);               // do while() because we know there's space for at least a 3x3 space (if not, there wouldn't be a door)
            break;
        case 1:
            do{
                if ((Rand(seed,75,j*spaceStation->nr_created)%MAXH+1 < Rand(seed,j*spaceStation->nr_created,300)%MAXH+2)&&(!Overlap(*spaceStation, door->x,door->y-Rand(seed,75,j*spaceStation->nr_created)%MAXH-1, Rand(seed,250,j*spaceStation->nr_created)%MAXL+3, Rand(seed,j*spaceStation->nr_created,300)%MAXH+3,*door))){
                    spaceStation->rooms[spaceStation->nr_created].x =door->x;
                    spaceStation->rooms[spaceStation->nr_created].y =door->y-Rand(seed,75,j*spaceStation->nr_created)%MAXH-1;
                    spaceStation->rooms[spaceStation->nr_created].L = Rand(seed,250,j*spaceStation->nr_created)%MAXL+3;
                    spaceStation->rooms[spaceStation->nr_created].H = Rand(seed,j*spaceStation->nr_created,300)%MAXH+3;
                    flag = 0;
                }
                j++;
            }while (flag);               // do while() because we know there's space for at least a 3x3 space (if not, there wouldn't be a door)
            break;
        case 2:
            do{
                if ((Rand(seed,75,j*spaceStation->nr_created)%MAXL+1 < Rand(seed,250,j*spaceStation->nr_created)%MAXL+2)&&(!Overlap(*spaceStation, door->x-Rand(seed,75,j*spaceStation->nr_created)%MAXL-1,door->y-Rand(seed,j*spaceStation->nr_created,300)%MAXH-2, Rand(seed,250,j*spaceStation->nr_created)%MAXL+3, Rand(seed,j*spaceStation->nr_created,300)%MAXH+3,*door))){
                    spaceStation->rooms[spaceStation->nr_created].x = door->x-Rand(seed,75,j*spaceStation->nr_created)%MAXL-1;
                    spaceStation->rooms[spaceStation->nr_created].y = door->y-Rand(seed,j*spaceStation->nr_created,300)%MAXH-2;
                    spaceStation->rooms[spaceStation->nr_created].L = Rand(seed,250,j*spaceStation->nr_created)%MAXL+3;
                    spaceStation->rooms[spaceStation->nr_created].H = Rand(seed,j*spaceStation->nr_created,300)%MAXH+3;
                    flag = 0;
                }
                j++;
            }while(flag);               // do while() because we know there's space for at least a 3x3 space (if not, there wouldn't be a door)
            break;
        case 3:
            do{
                if ((Rand(seed,75,j*spaceStation->nr_created)%MAXH+1 < Rand(seed,j*spaceStation->nr_created,300)%MAXH+2)&&(!Overlap(*spaceStation, door->x-Rand(seed,250,j*spaceStation->nr_created)%MAXL-2, door->y-Rand(seed,75,j*spaceStation->nr_created)%MAXH-1, Rand(seed,250,j*spaceStation->nr_created)%MAXL+3, Rand(seed,j*spaceStation->nr_created,300)%MAXH+3,*door))){
                    spaceStation->rooms[spaceStation->nr_created].x = door->x-Rand(seed,250,j*spaceStation->nr_created)%MAXL-2;
                    spaceStation->rooms[spaceStation->nr_created].y = door->y-Rand(seed,75,j*spaceStation->nr_created)%MAXH-1;
                    spaceStation->rooms[spaceStation->nr_created].L = Rand(seed,250,j*spaceStation->nr_created)%MAXL+3;
                    spaceStation->rooms[spaceStation->nr_created].H = Rand(seed,j*spaceStation->nr_created,300)%MAXH+3;
                    flag = 0;
                }
                j++;
            }while(flag);               // do while() because we know there's space for at least a 3x3 space (if not, there wouldn't be a door)
            break;
    }

    for (int i=0;i<4;i++){      // Check how many doors we can have (if doors can fit)
        if ((i==0)&&(enteringWall!=0)){     // We don't check on the wall where the door we're coming from is
            for (int j=1;j<spaceStation->rooms[spaceStation->nr_created].L-1;j++){
                if (CanDoorGoThere(*spaceStation, spaceStation->rooms[spaceStation->nr_created].x + j, spaceStation->rooms[spaceStation->nr_created].H + spaceStation->rooms[spaceStation->nr_created].y-1,0)){
                    maxDoors++;     // max doors is between 0 and 3
                    flag0=1;
                    break;
                }
            }
        }
        if ((i==1)&&(enteringWall!=1)){
            for (int j=1;j<spaceStation->rooms[spaceStation->nr_created].H-1;j++){
                if (CanDoorGoThere(*spaceStation, spaceStation->rooms[spaceStation->nr_created].x + spaceStation->rooms[spaceStation->nr_created].L-1,spaceStation->rooms[spaceStation->nr_created].y + j,1)){
                    maxDoors++;
                    flag1=1;
                    break;
                }
            }
        }
        if ((i==2)&&(enteringWall!=2)){
            for (int j=1;j<spaceStation->rooms[spaceStation->nr_created].L-1;j++){
                if (CanDoorGoThere(*spaceStation, spaceStation->rooms[spaceStation->nr_created].x+ j, spaceStation->rooms[spaceStation->nr_created].y,2)){
                    maxDoors++;
                    flag2=1;
                    break;
                }
            }
        }
        if ((i==3)&&(enteringWall!=3)){
            for (int j=1;j<spaceStation->rooms[spaceStation->nr_created].H-1;j++){
                if (CanDoorGoThere(*spaceStation, spaceStation->rooms[spaceStation->nr_created].x,spaceStation->rooms[spaceStation->nr_created].y + j,3)){
                    maxDoors++;
                    flag3=1;
                    break;
                }
            }
        }
    }

    while((maxDoors>=0)&&(spaceStation->nd_left<maxDoors)){       // Be careful to not create more doors than there is doors left to create
        maxDoors--;
    }

    if (maxDoors < 1){
        spaceStation->rooms[spaceStation->nr_created].n_doors = 1;
    }
    else if (maxDoors==1){      // If possible, we try to create at least 2 doors
        spaceStation->rooms[spaceStation->nr_created].n_doors = 2;
    }
    else{   
        spaceStation->rooms[spaceStation->nr_created].n_doors = Rand(seed,spaceStation->nr_created,15)%(maxDoors)+2;       // Make random number of doors
    }

    spaceStation->nd_left = spaceStation->nd_left-(spaceStation->rooms[spaceStation->nr_created].n_doors-1);
    spaceStation->rooms[spaceStation->nr_created].doors = NULL;
    spaceStation->rooms[spaceStation->nr_created].doors = malloc((spaceStation->rooms[spaceStation->nr_created].n_doors)*sizeof(Door));

    if (spaceStation->rooms[spaceStation->nr_created].doors == NULL){     // Robustesse
        return 2;
    }

    spaceStation->rooms[spaceStation->nr_created].doors[0].wall = enteringWall;        // The first is the door we're entering from
    spaceStation->rooms[spaceStation->nr_created].doors[0].x = door->x;
    spaceStation->rooms[spaceStation->nr_created].doors[0].y = door->y;
    spaceStation->rooms[spaceStation->nr_created].doors[0].open = 1;
    door->open = 1;     // Our door is open
 
    count = 1;   // Count which door we're on in order to not make more doors than n_doors
    while(spaceStation->rooms[spaceStation->nr_created].n_doors>1 && count<spaceStation->rooms[spaceStation->nr_created].n_doors){          // Creating all the other doors
        m=0;
        switch(Rand(seed,45*j,45+j)%4){        // Chooses randomly which wall we're on
            case 0:
                if(flag0){
                    while (!CanDoorGoThere(*spaceStation,spaceStation->rooms[spaceStation->nr_created].x+Rand(seed,85*m,50)%(spaceStation->rooms[spaceStation->nr_created].L-2)+1,spaceStation->rooms[spaceStation->nr_created].y+spaceStation->rooms[spaceStation->nr_created].H-1,0)){
                        m++;
                    }
                    spaceStation->rooms[spaceStation->nr_created].doors[count].x = spaceStation->rooms[spaceStation->nr_created].x+Rand(seed,85+m,50)%(spaceStation->rooms[spaceStation->nr_created].L-2)+1;
                    spaceStation->rooms[spaceStation->nr_created].doors[count].y = spaceStation->rooms[spaceStation->nr_created].y+spaceStation->rooms[spaceStation->nr_created].H -1;
                    spaceStation->rooms[spaceStation->nr_created].doors[count].wall = 0;
                    spaceStation->rooms[spaceStation->nr_created].doors[count].open = 0;
                    count++;
                    flag0 = 0;
                }
                j++;
                break;
            case 1:
                if(flag1){
                    while (!CanDoorGoThere(*spaceStation,spaceStation->rooms[spaceStation->nr_created].x + spaceStation->rooms[spaceStation->nr_created].L - 1,spaceStation->rooms[spaceStation->nr_created].y+Rand(seed,85+m,50)%(spaceStation->rooms[spaceStation->nr_created].H-2)+1,1)){
                        m++;
                    }
                    spaceStation->rooms[spaceStation->nr_created].doors[count].x = spaceStation->rooms[spaceStation->nr_created].x + spaceStation->rooms[spaceStation->nr_created].L - 1;
                    spaceStation->rooms[spaceStation->nr_created].doors[count].y = spaceStation->rooms[spaceStation->nr_created].y+Rand(seed,85+m,50)%(spaceStation->rooms[spaceStation->nr_created].H-2)+1;
                    spaceStation->rooms[spaceStation->nr_created].doors[count].wall = 1;
                    spaceStation->rooms[spaceStation->nr_created].doors[count].open = 0;
                    count++;
                    flag1 = 0;
                }
                j++;
                break;
            case 2:
                if(flag2){
                    while (!CanDoorGoThere(*spaceStation,spaceStation->rooms[spaceStation->nr_created].x+Rand(seed,85,50+m)%(spaceStation->rooms[spaceStation->nr_created].L-2)+1,spaceStation->rooms[spaceStation->nr_created].y,2)){
                        m++;
                    }
                    spaceStation->rooms[spaceStation->nr_created].doors[count].x = spaceStation->rooms[spaceStation->nr_created].x+Rand(seed,85,50+m)%(spaceStation->rooms[spaceStation->nr_created].L-2)+1;
                    spaceStation->rooms[spaceStation->nr_created].doors[count].y = spaceStation->rooms[spaceStation->nr_created].y;
                    spaceStation->rooms[spaceStation->nr_created].doors[count].wall = 2;
                    spaceStation->rooms[spaceStation->nr_created].doors[count].open = 0;
                    count++;
                    flag2 = 0;
                }
                j++;
                break;
            case 3:
                if(flag3){
                    while (!CanDoorGoThere(*spaceStation,spaceStation->rooms[spaceStation->nr_created].x,spaceStation->rooms[spaceStation->nr_created].y+Rand(seed,85+m,50)%(spaceStation->rooms[spaceStation->nr_created].H-2)+1,3)){
                        m++;
                    }
                    spaceStation->rooms[spaceStation->nr_created].doors[count].x = spaceStation->rooms[spaceStation->nr_created].x;
                    spaceStation->rooms[spaceStation->nr_created].doors[count].y = spaceStation->rooms[spaceStation->nr_created].y+Rand(seed,85+m,50)%(spaceStation->rooms[spaceStation->nr_created].H-2)+1;
                    spaceStation->rooms[spaceStation->nr_created].doors[count].wall = 3;
                    spaceStation->rooms[spaceStation->nr_created].doors[count].open = 0;
                    count++;
                    flag3 = 0;
                }
                j++;
                break;
            default:
                j++;
                break;
        }
    }
    
    if (spaceStation->n_tasks2create >= (spaceStation->total_rooms - spaceStation->nr_created)||(Rand(seed,spaceStation->nr_created*40,spaceStation->n_tasks2create*3)%2)){          // Tasks
        spaceStation->rooms[spaceStation->nr_created].tasks.x = Rand(seed,spaceStation->n_tasks2create*8,15)%(spaceStation->rooms[spaceStation->nr_created].L-2)+1+spaceStation->rooms[spaceStation->nr_created].x;
        spaceStation->rooms[spaceStation->nr_created].tasks.y = Rand(seed,78,spaceStation->nr_created*9)%(spaceStation->rooms[spaceStation->nr_created].H-2)+1+spaceStation->rooms[spaceStation->nr_created].y;
        spaceStation->rooms[spaceStation->nr_created].tasks.done = 0;
        spaceStation->n_tasks2create--;
    }
    else{           // No task in this room
        spaceStation->rooms[spaceStation->nr_created].tasks.x = 0;
        spaceStation->rooms[spaceStation->nr_created].tasks.y = 0;
        spaceStation->rooms[spaceStation->nr_created].tasks.done = 1;
    }

    l=1;

    if (Rand(seed,spaceStation->nr_created*3,78)%2==0){        // Object
        spaceStation->rooms[spaceStation->nr_created].object.x = Rand(seed,spaceStation->n_tasks2create*54,15)%(spaceStation->rooms[spaceStation->nr_created].L-2)+1+spaceStation->rooms[spaceStation->nr_created].x;
        spaceStation->rooms[spaceStation->nr_created].object.y = Rand(seed,78,spaceStation->nr_created*44)%(spaceStation->rooms[spaceStation->nr_created].H-2)+1+spaceStation->rooms[spaceStation->nr_created].y;
        spaceStation->rooms[spaceStation->nr_created].object.pickup = 0;

        spaceStation->rooms[spaceStation->nr_created].object.icon = Rand(seed,712,spaceStation->nr_created*5)%3;
        switch(Rand(seed,712,spaceStation->nr_created*5)%3){
            case 0:         // Potion heal
                spaceStation->rooms[spaceStation->nr_created].object.exp = Rand(seed,115,spaceStation->nr_created)%50+50;
                spaceStation->rooms[spaceStation->nr_created].object.heal = 25;
                break;
            case 1:         // Potion strengh
                spaceStation->rooms[spaceStation->nr_created].object.exp = Rand(seed,242,spaceStation->nr_created)%50+50;
                spaceStation->rooms[spaceStation->nr_created].object.heal = 10;
                break;
            case 2:         // Book (for exp)
                spaceStation->rooms[spaceStation->nr_created].object.exp = Rand(seed,spaceStation->nr_created*8,14)%100+250;
                spaceStation->rooms[spaceStation->nr_created].object.heal = 0;
                break;
        }
    }
    else{           // No objects here
        spaceStation->rooms[spaceStation->nr_created].object.x = 0;
        spaceStation->rooms[spaceStation->nr_created].object.y = 0;
        spaceStation->rooms[spaceStation->nr_created].object.heal = 0;
        spaceStation->rooms[spaceStation->nr_created].object.exp = 0;
        spaceStation->rooms[spaceStation->nr_created].object.icon = -1;
        spaceStation->rooms[spaceStation->nr_created].object.pickup = 1;
    }

    if (Rand(seed,spaceStation->nr_created*7,1)%4 == 0){        // Weapon
        spaceStation->rooms[spaceStation->nr_created].weapon.x = Rand(seed,spaceStation->n_tasks2create*17,32)%(spaceStation->rooms[spaceStation->nr_created].L-2)+1+spaceStation->rooms[spaceStation->nr_created].x;
        spaceStation->rooms[spaceStation->nr_created].weapon.y = Rand(seed,4,spaceStation->nr_created*74)%(spaceStation->rooms[spaceStation->nr_created].H-2)+1+spaceStation->rooms[spaceStation->nr_created].y;
        spaceStation->rooms[spaceStation->nr_created].weapon.pickup = 0;

        if (Rand(seed,85,spaceStation->nr_created*56)%5 == 0 || Rand(seed,85,spaceStation->nr_created*56)%5 == 1){         // Swords
            spaceStation->rooms[spaceStation->nr_created].weapon.damage = 45;
            spaceStation->rooms[spaceStation->nr_created].weapon.icon = 0;
        }
        else{         // hammer
            spaceStation->rooms[spaceStation->nr_created].weapon.damage = 10;
            spaceStation->rooms[spaceStation->nr_created].weapon.icon = 1;
        }
    }
    else{           // No weapons here
        spaceStation->rooms[spaceStation->nr_created].weapon.x = 0;
        spaceStation->rooms[spaceStation->nr_created].weapon.y = 0;
        spaceStation->rooms[spaceStation->nr_created].weapon.damage = 0;
        spaceStation->rooms[spaceStation->nr_created].weapon.icon = -1;
        spaceStation->rooms[spaceStation->nr_created].weapon.pickup = 1;
    }
    spaceStation->nr_created++;

    return 0;
}