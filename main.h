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

typedef struct{
    int icon;
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
    int icon;
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


int Rand(int seed, int x, int y);

int FindDoor(Dungeon spaceStation, Door** door, int x, int y);

int FindTask(Dungeon spaceStation, Task** task, int x, int y);

int FindObject(Dungeon spaceStation, Object** object, int x, int y);

int FindWeapon(Dungeon spaceStation, Weapon** weapon, int x, int y);

Dungeon FirstRoom(int seed);

int CanDoorGoThere(Dungeon spaceStation, int x, int y, int wall);

int Overlap(Dungeon spaceStation, int x,int y, int L, int H, Door door);

int NewRoom(int seed, Dungeon* spaceStation, Door* door);

void PrintLetters(int letter);

int PrintEnd(int victory, Character stats, char* name, int seed, time_t begin);

int PrintMap(Dungeon spaceStation, Character player);

int PrintUI(Character stats, char* name,int seed, int tasks, time_t timeBegin, int step);

void PrintInventory(Character* player,char* name,int seed, int tasks, time_t *begin, int cursorobj);

int strequal(char* str1,char* str2);

int NewSave(Dungeon spacestation, Character player, char* name, int seed, time_t timer);

int SavingChoice(Character player, char* name, int seed, int tasks, time_t* timeBegin);

int SearchSave(char* name);

int GetSave(Dungeon* spacestation, Character* player, char* name, int* seed);

int getname(char ** tab);

int UpdateSave(Dungeon spacestation, Character player, char* name, int seed,time_t start, time_t pause);

int Pause(Character stats, char* name, int seed, int tasks, time_t* begin);

int InventoryFull(Character player, int choice);

int Inventory(Character* player, char* name, int seed, int tasks, time_t *begin);

int DoTask(Dungeon* spaceStation, Character* stats, Task* task, char* name, int seed , time_t begin);

int PickupObject(Character* player,Object* object);

int PickupWeapon(Character* player,Weapon* weapon);

int Movement(Dungeon spaceStation,int x,int y);

int Game(Dungeon* spaceStation,char* name, int seed,Character player);

int CreateGame();

int LoadSave();

int QuitGame(Character player);

int TitleScreen();