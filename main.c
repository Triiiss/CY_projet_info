#include <ncurses.h>  // library that allows to move with arrow keys (need to check docstring)
#include <curses.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define KEYRIGHT 261
#define KEYLEFT 260
#define KEYUP 259
#define KEYDOWN 258

#define MAXTIME 500       // Idea : Settings change the variables
#define MAXL 40
#define MAXH 20
#define MIN_TASKS 7
#define MAXPV 100
#define TIMETASK 3

#define NAMELIMIT 20 
#define SCREENL 100
#define SCREENH 35
#define SCREENI 6
#define BARPV 20
#define MAXDATA 500000

typedef struct{
    char icon;
    int x;
    int y;
    int pickup;
    int damage;
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
    Weapon weapon;
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

// RANDOM ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int Rand(int seed , int x,int y ){            // x value of incremantation and y the number of loops
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

// ROOMS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int FindDoor(Dungeon spaceStation,Door** door, int x,int y){
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

int FindTask(Dungeon spaceStation, Task** task,int x,int y){
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

int FindObject(Dungeon spaceStation, Object** object,int x,int y){
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

int FindWeapon(Dungeon spaceStation, Weapon** weapon,int x,int y){
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
    spaceStation.rooms->weapon.x = 0;
    spaceStation.rooms->weapon.y = 0;
    spaceStation.rooms->weapon.pickup = 1;

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

        switch(Rand(seed,712,spaceStation->nr_created*5)%3){
            case 0:         // Potion heal
                spaceStation->rooms[spaceStation->nr_created].object.exp = Rand(seed,115,spaceStation->nr_created)%50+50;
                spaceStation->rooms[spaceStation->nr_created].object.heal = 25;
                spaceStation->rooms[spaceStation->nr_created].object.icon = 'h';
                break;
            case 1:         // Potion strengh
                spaceStation->rooms[spaceStation->nr_created].object.exp = Rand(seed,242,spaceStation->nr_created)%50+50;
                spaceStation->rooms[spaceStation->nr_created].object.heal = 10;
                spaceStation->rooms[spaceStation->nr_created].object.icon = 's';
                break;
            case 2:         // Book (for exp)
                spaceStation->rooms[spaceStation->nr_created].object.exp = Rand(seed,spaceStation->nr_created*8,14)%100+250;
                spaceStation->rooms[spaceStation->nr_created].object.heal = 0;
                spaceStation->rooms[spaceStation->nr_created].object.icon = 'b';
                break;
        }
    }
    else{           // No objects here
        spaceStation->rooms[spaceStation->nr_created].object.x = 0;
        spaceStation->rooms[spaceStation->nr_created].object.y = 0;
        spaceStation->rooms[spaceStation->nr_created].object.heal = 0;
        spaceStation->rooms[spaceStation->nr_created].object.exp = 0;
        spaceStation->rooms[spaceStation->nr_created].object.icon = '.';
        spaceStation->rooms[spaceStation->nr_created].object.pickup = 1;
    }

    if (Rand(seed,spaceStation->nr_created*7,1)%4 == 0){        // Weapon
        spaceStation->rooms[spaceStation->nr_created].weapon.x = Rand(seed,spaceStation->n_tasks2create*17,32)%(spaceStation->rooms[spaceStation->nr_created].L-2)+1+spaceStation->rooms[spaceStation->nr_created].x;
        spaceStation->rooms[spaceStation->nr_created].weapon.y = Rand(seed,4,spaceStation->nr_created*74)%(spaceStation->rooms[spaceStation->nr_created].H-2)+1+spaceStation->rooms[spaceStation->nr_created].y;
        spaceStation->rooms[spaceStation->nr_created].weapon.pickup = 0;

        if (Rand(seed,85,spaceStation->nr_created*56)%5 == 0 || Rand(seed,85,spaceStation->nr_created*56)%5 == 1){         // Swords
            spaceStation->rooms[spaceStation->nr_created].weapon.damage = 45;
            spaceStation->rooms[spaceStation->nr_created].weapon.icon = 's';
        }
        else{         // hammer
            spaceStation->rooms[spaceStation->nr_created].weapon.damage = 10;
            spaceStation->rooms[spaceStation->nr_created].weapon.icon = 'h';
        }
    }
    else{           // No weapons here
        spaceStation->rooms[spaceStation->nr_created].weapon.x = 0;
        spaceStation->rooms[spaceStation->nr_created].weapon.y = 0;
        spaceStation->rooms[spaceStation->nr_created].weapon.damage = 0;
        spaceStation->rooms[spaceStation->nr_created].weapon.icon = '.';
        spaceStation->rooms[spaceStation->nr_created].weapon.pickup = 1;
    }
    spaceStation->nr_created++;

    return 0;
}

// Affichage jeu ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

int PrintEnd(int victory,Character stats,char* name, int seed, time_t begin){
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
                    case 'h':
                        mvprintw(player.y-spaceStation.rooms[i].object.y + (SCREENH-SCREENI)/2 + SCREENI,spaceStation.rooms[i].object.x - player.x+SCREENL/2,"✚");
                        break;
                    case 'b':
                        mvprintw(player.y-spaceStation.rooms[i].object.y + (SCREENH-SCREENI)/2 + SCREENI,spaceStation.rooms[i].object.x - player.x+SCREENL/2,"🕮");
                        break;
                    case 's':
                        mvprintw(player.y-spaceStation.rooms[i].object.y + (SCREENH-SCREENI)/2 + SCREENI,spaceStation.rooms[i].object.x - player.x+SCREENL/2,"⚗");
                        break;
                }
                
            }
        }
        if ((spaceStation.rooms[i].weapon.x != 0 || spaceStation.rooms[i].weapon.y != 0) && spaceStation.rooms[i].weapon.pickup == 0){      // If there is a non-picked up weapon
            if ((spaceStation.rooms[i].weapon.x - player.x < SCREENL/2 && spaceStation.rooms[i].weapon.x - player.x > -SCREENL/2)&&(spaceStation.rooms[i].weapon.y - player.y < (SCREENH-SCREENI)/2 && spaceStation.rooms[i].weapon.y - player.y > -(SCREENH-SCREENI)/2)){
                switch(spaceStation.rooms[i].weapon.icon){
                    case 's':
                        mvprintw(player.y-spaceStation.rooms[i].weapon.y + (SCREENH-SCREENI)/2 + SCREENI,spaceStation.rooms[i].weapon.x - player.x+SCREENL/2,"⚔");
                        break;
                    case 'h':
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

int PrintUI(Character stats, char* name,int seed,  int tasks, time_t timeBegin, int step){
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
                if (stats.bag.weaponPouch[stats.hold].icon == 'h'){
                    mvprintw(2,SCREENL/5*3,"Obj: R. Tools");
                }
                else if (stats.bag.weaponPouch[stats.hold].icon == 's'){
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
                case 's':
                    mvprintw(SCREENI+10+i*2,SCREENL/2+3,"▢  Strenght potion     ⚗");
                    break;
                case 'b':
                    mvprintw(SCREENI+10+i*2,SCREENL/2+3,"▢  Book of knowledge   🕮");
                    break;
                case 'h':
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
                case 'h':
                    mvprintw(SCREENI+10+i*4,3,"▢  Reparing tools  ⚒");
                    break;
                case 's':
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

// Saves ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
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

    if(ftell(fp)<MAXDATA){      // IDk dude
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
    }
    fclose(fp);

    return -2;
    
}

int SavingChoice(Character player, char* name,int seed,  int tasks, time_t* timeBegin){
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
    FILE* fichier=NULL;
    int cursor=0;
    int size=0;
    size=strlen(name);
    size--;
    char c=0;
    fichier=fopen("saves.txt","r");
    if (fichier == NULL){
        return 2;
    }

    fseek(fichier,0,SEEK_SET);
    c = fgetc(fichier);
    while(c!=EOF){
        mvprintw(SCREENH+2,0,"%c %c",c,name[cursor]);
        if(c==name[cursor]){
            mvprintw(SCREENH+3,0,"%d %d",cursor,size);
            cursor++;
            if(cursor>=size){
                return ftell(fichier);
            }
        }
        else{
            cursor=0;
        }
        refresh();
        getch();
        c=fgetc(fichier);
    }
    fclose(fichier);
    return -1;
}

int GetSave(Dungeon* spacestation,Character* player,char* name,int* seed){
    FILE *fp;

    if(SearchSave(name)!=-1){
        fp=fopen("saves.txt","r");
        fseek(fp,SearchSave(name),SEEK_SET);
        fscanf(fp,"%d",seed);
        fscanf(fp,"%d",&(player->x));
        fscanf(fp,"%d",&(player->y));
        fscanf(fp,"%d",&(player->pv));
        fscanf(fp,"%d",&(player->exp));
        fscanf(fp,"%d",&(player->tasksDone));

        fscanf(fp,"%d",&(player->timer));

        fscanf(fp,"%d",&(spacestation->n_tasks));
        fscanf(fp,"%d",&(spacestation->total_rooms));
        fscanf(fp,"%d",&(spacestation->nr_created));
        fscanf(fp,"%d",&(spacestation->nd_left));

        for (int i=0;i<spacestation->nr_created;i++){
            fscanf(fp,"%d",&(spacestation->rooms[i].x));
            /*fscanf(fp,"%d",&(spacestation->rooms[i].y));
            fscanf(fp,"%d",&(spacestation->rooms[i].H));
            fscanf(fp,"%d",&(spacestation->rooms[i].L));
            fscanf(fp,"%d",&(spacestation->rooms[i].tasks.x));
            fscanf(fp,"%d",&(spacestation->rooms[i].tasks.y));
            fscanf(fp,"%d",&(spacestation->rooms[i].tasks.done));
            fscanf(fp,"%d",&(spacestation->rooms[i].n_doors));
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
    /*save the parameter in the file save.txt if the name does not exist return -1*
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

    if(SearchSave(name)==-1){
        return -2;
    }

    fp=fopen("saves.txt","a");
    if(fp==NULL){
        return 2;
    }
    
    
    fclose(fp);

    nbsave=getname(tab);
    spacestationtab=malloc((nbsave)*sizeof(Dungeon));
    seedtab=malloc((nbsave)*sizeof(int));
    playertab=malloc((nbsave)*sizeof(Character));
    
    if (spacestationtab == NULL || seedtab == NULL || playertab == NULL){
        return 2;
    }

    fp=fopen("saves.txt","r");

    int c=0;
    for(int i=0;i<nbsave;i++){
        if(strequal(tab[i],name)!=1){
            fseek(fp,SearchSave(tab[i])+1,SEEK_CUR);
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
                spacestationtab[j].rooms[k].doors = malloc(sizeof(Door)*spacestationtab[j].rooms[k].n_doors);
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

    for(int i=0;i<nbsave;i++){
        if(strequal(tab[i],name)!=1){
            fprintf(fp,"{%s\n",*(tab+i));
            fprintf(fp,"%d\n",*(seedtab+i));
            fprintf(fp,"%d\n",((playertab+i)->x));
            fprintf(fp,"%d\n",((playertab+i)->y));
            fprintf(fp,"%d\n",((playertab+i)->pv));
            fprintf(fp,"%d\n",((playertab+i)->exp));
            fprintf(fp,"%d\n",((playertab+i)->tasksDone));
            fprintf(fp,"%d\n",((playertab+i)->timer));

            fprintf(fp,"%d\n",((spacestationtab+i)->n_tasks));
            fprintf(fp,"%d\n",((spacestationtab+i)->total_rooms));
            fprintf(fp,"%d\n",((spacestationtab+i)->nr_created));
            fprintf(fp,"%d\n",((spacestationtab+i)->nd_left));

            for (int j=0;j<(spacestationtab+i)->nr_created;j++){
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[j].x));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[j].y));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[j].H));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[j].L));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[j].tasks.x));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[j].tasks.y));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[j].tasks.done));
                fprintf(fp,"%d\n",((spacestationtab+i)->rooms[j].n_doors));
                for (int k=0;k<(spacestationtab+i)->rooms[j].n_doors;k++){
                    fprintf(fp,"%d\n",((spacestationtab+i)->rooms[j].doors[k].x));
                    fprintf(fp,"%d\n",((spacestationtab+i)->rooms[j].doors[k].y));
                    fprintf(fp,"%d\n",((spacestationtab+i)->rooms[j].doors[k].wall));
                    fprintf(fp,"%d\n",((spacestationtab+i)->rooms[j].doors[k].open));
                }
            }
            fprintf(fp,"}\n");
        }
    }


    fclose(fp);

    return 0;
}
*/
// Game ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int Pause(Character stats,char* name, int seed, int tasks, time_t* begin){
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

int Inventory(Character* player,char* name,int seed, int tasks, time_t *begin){
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
                                            player->bag.use_pouch[cursorobj-1].icon = ' ';
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
                                                case 's':
                                                    mvprintw(SCREENH-4,SCREENL/4*3-10,"Potion of Strengh : Makes you able");
                                                    mvprintw(SCREENH-3,SCREENL/4*3-10,"to deal more damage.");
                                                    break;
                                                case 'h':
                                                    mvprintw(SCREENH-4,SCREENL/4*3-10,"Healing potion : ");
                                                    mvprintw(SCREENH-3,SCREENL/4*3-10,"Gives you %d health",player->bag.use_pouch[cursorobj -1].heal);
                                                    mvprintw(SCREENH-2,SCREENL/4*3-10,"(not more than your maximum)");
                                                    break;
                                                case 'b':
                                                    mvprintw(SCREENH-4,SCREENL/4*3-10,"Book of knowledge : ");
                                                    mvprintw(SCREENH-3,SCREENL/4*3-10,"Gives you more experience");
                                                    break;
                                            }
                                            break;
                                        case 2:     // Use it
                                            switch(player->bag.use_pouch[cursorobj-1].icon){
                                                case 's':
                                                    player->damage += player->bag.use_pouch[cursorobj-1].heal;
                                                    mvprintw(SCREENH+2,0,"You gained %d of strenght",player->bag.use_pouch[cursorobj-1].heal);
                                                    break;
                                                case 'h':
                                                    player->pv += player->bag.use_pouch[cursorobj-1].heal;
                                                    if (player->pv>MAXPV){
                                                        player->pv = 100;
                                                    }
                                                    mvprintw(SCREENH+2,0,"You gained %d health points",player->bag.use_pouch[cursorobj-1].heal);
                                                    break;
                                                case 'b':
                                                    player->exp += player->bag.use_pouch[cursorobj-1].exp;
                                                    mvprintw(SCREENH+2,0,"You gained %d experience points",player->bag.use_pouch[cursorobj-1].exp);
                                                    break;
                                            }
                                            player->bag.use_pouch[cursorobj-1].exp = 0;     // Erase it from your bag
                                            player->bag.use_pouch[cursorobj-1].heal = 0;
                                            player->bag.use_pouch[cursorobj-1].icon = ' ';
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
                                                player->bag.weaponPouch[-cursorobj-1].icon = ' ';
                                                mvprintw(SCREENH+2,0,"You dropped an object");
                                            }
                                            refresh();
                                            use = 0;
                                            break;
                                        case 1:         // Gives info
                                            mvprintw(SCREENH-6,SCREENL/4-10,"Info:");
                                            switch(player->bag.weaponPouch[-cursorobj-1].icon){
                                                case 's':
                                                    mvprintw(SCREENH-4,SCREENL/4-10,"Laser Sword : A great weapon");
                                                    mvprintw(SCREENH-3,SCREENL/4-10,"for a great soldier.");
                                                    mvprintw(SCREENH-2,SCREENL/4-10,"Deals 45 damage.");
                                                    break;
                                                case 'h':
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

int DoTask(Dungeon* spaceStation,Character* stats,Task* task, char* name,int seed ,time_t begin){
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

int Game(Dungeon* spaceStation,char* name, int seed,Character player,int newSave){
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
    if (newSave){           // Opposed to load a save
        //NewSave(*spaceStation,player,name,seed,begin);
    }

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
                    /*pause = time(NULL);
                    saved = UpdateSave(*spaceStation,player,name,seed,begin,pause);
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
                    }*/
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
        for (int i=0;i<21;i++){     // Reinitialise the name
            name[i] = '\0';
        }

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
            if (((k<91 && k>64)||(k<58 && k>47)||(k<123 && k>96)||(k==95))&&(len<NAMELIMIT)){       // Maj + min + num + _
                name[len] = k;
                len++;
            }
            else if ((k==263)&&(len>0)){        // If DEL
                name[len] = '\0';
                len--;
            }

            for (int i=0;i<len+1;i++){            // Print name chosen (stop at character limit)
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
        player.bag.use_pouch[i].icon = 0;
        player.bag.use_pouch[i].pickup = 0;
        player.bag.use_pouch[i].x = 0;
        player.bag.use_pouch[i].y = 0;
    }
    for (int i=0;i<2;i++){          // Initialisation of bag of weapons
        player.bag.weaponPouch[i].icon = 0;
        player.bag.weaponPouch[i].x = 0;
        player.bag.weaponPouch[i].y = 0;
        player.bag.weaponPouch[i].pickup = 0;
        player.bag.weaponPouch[i].damage = 0;
    }

    spaceStation = FirstRoom(seed);

    if (spaceStation.total_rooms == -1){        // Malloc failed
        return 2;
    }

    error = Game(&spaceStation,name,seed,player,1);

    for (int i=0;i<spaceStation.nr_created;i++){        // Free everything before returning to menu
        free(spaceStation.rooms[i].doors);
    }
    free(spaceStation.rooms);
    //free(name);

    return error;
}

int LoadSave(){
    Dungeon spaceStation;
    Character player;
    char* name=NULL;
    int seed=0;
    char temp_name[NAMELIMIT+1];
    int k=0,error=0,len=0,confirm=0,cursor=0, retry=1; 
    
    while (retry){
        while (!confirm){
            do{                 // Ask name so you can see character limit (20)
                clear();
                PrintUI(player,"",0,0,0,1);
                PrintLetters(3);        // Print NAME
                mvprintw(SCREENI+7,SCREENL/2-10,"Enter your name : ");
                if (((k<91 && k>64)||(k<58 && k>47)||(k<123 && k>96)||(k==95))&&(len<NAMELIMIT)){       // Maj + min + num + _
                    *(temp_name+len) = k;
                    len++;
                }
                else if ((k==263)&&(len>0)){        // If DEL
                    *(temp_name+len) = '\0';
                    len--;
                }

                for (int i=0;i<len;i++){            // Print name chosen (stop at character limit)
                    printw("%c", temp_name[i]);
                }

                refresh();
                k = getch();
            } while ((k!=10)||(len==0));        // Until you press enter

            name = malloc(sizeof(char)*(len));
            if (name==NULL){
                return 2;
            }
            
            for (int i=0;i<len;i++){                // If name chosen, the temp name will transfer to a dynamic string
            if(((temp_name[i]<91 && temp_name[i]>64)||(temp_name[i]<58 && temp_name[i]>47)||(temp_name[i]<123 && temp_name[i]>96))&&(len<NAMELIMIT)){
                *(name+i) = temp_name[i];
            }
            }
            //error = GetSave(&spaceStation,&player,name,&seed);

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

                    refresh();
                    k=getch();
                }while(k!=10);
                if (cursor){
                    return 0;                
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

    error = Game(&spaceStation,name,seed,player,0);

    for (int i=0;i<spaceStation.nr_created;i++){        // Free everything before returning to menu
        free(spaceStation.rooms[i].doors);
    }
    free(spaceStation.rooms);

    return error;
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
                            if(error == 1){
                                endwin();
                                return 0;
                            }
                            if (error != 0){
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


int main(){
    int error = 0;

    error = TitleScreen();

    if (error){            // Print the error if one (if menu return != 0) => Robustesse
        switch(error){
            case 2:
                printf("Erreur d'allocation mémoire");
                return error;
            case -1:
                printf("Erreur de la carte");
                return error;
            case -2:
                printf("Erreur de sauvegarde");
                return error;
            default:
                printf("Ceci est un message d'erreur");
                return 3;
                break;  // Diffrent error messages
        }
    }


    return 0;
}
