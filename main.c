#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <ncurses.h>  //library that allows to move with arrow keys (need to check docstring)

#define NAMELIMIT 20
#define KEYRIGHT 261
#define KEYLEFT 260
#define KEYUP 259
#define KEYDOWN 258

typedef struct{
    int x;              //Player's position on the x axis
    int y;              //Player's position on the y axis
    int pv;             //Health, must be above 0
    char* name;
    float exp;          //Experience point
    int* bag;           //Inventory, contains object
}Player;

typedef struct{     //ajouter les n° de salles ?
    int x;      //Position of the door
    int y;      //Position of the door
}Door;

typedef struct{
    int x;              //Position of the bottom right corner x
    int y;              //Position of the bottom right corner y
    int ndoor;          //Number of doors
    Door* doors;    //List of the different doors
    int width;      //At least 3
    int lenght;     //At least 3
}Room;

void printLetters(int n){
    /*Print letters with unicode characters
    int : n : to chose which letters to print*/
    switch (n){
        case 1:     //Cosmic yonder
            printw("╓─╴  ╓──╖  ╓─╴       ╶╥╴ ╓─╴\n");  //Print Title Screen
            printw("║    ║  ║  ╙─╖  ║╲╱║  ║  ║  \n");
            printw("╙─╴  ╙──╜  ╶─╜ ╶╨╴ ╨╴╶╨╴ ╙─╴\n");
            printw("      ╶╥╴ ╶╥╴\n");
            printw("       ╙───╢  ╓──╖       ╶╥──╮  ╥─╴  ╶╥─╮\n");
            printw("           ║  ║  ║  ║╲ ║  ║  │  ╟╴    ╟─╯\n");
            printw("          ╶╨╴ ╙──╜  ║ ╲║ ╶╨──╯  ╨─╴  ╶╨╴╲\n");
            break;
        case 2:     //Seed
            printw("╓─╴  ╥─╴  ╥─╴  ╶╥──╮\n");
            printw("╙─╖  ╟╴   ╟╴    ║  │\n");
            printw("╶─╜  ╨─╴  ╨─╴  ╶╨──╯\n\n");
            break;
        case 3:     //Name
            printw("      ╓──╖         ╥─╴\n");
            printw("║╲ ║  ╟──╢  ║╲╱║   ╟╴\n");
            printw("║ ╲║ ╶╨  ╨╴╶╨╴ ╨╴  ╨─╴\n\n");
            break;
        case 4:     //Pause
            printw("╶╥─╮  ╓──╖  ╥  ╥  ╓─╴ ╥─╴\n");
            printw(" ╟─╯  ╟──╢  ║  ║  ╙─╖ ╟╴\n");
            printw("╶╨╴  ╶╨╴ ╨╴ ╙──╜  ╶─╜ ╨─╴\n");
            break;
        case 5:     //Inventory
            printw("╶╥╴             ╥─╴        ┌─╥─┐  ╓──╖  ╶╥─╮  ╶╥╴ ╶╥╴\n");
            printw(" ║  ║╲ ║  ╲  ╱  ╟╴  ║╲ ║     ║    ║  ║   ╟─╯   ╙───╢\n");
            printw("╶╨╴ ║ ╲║   ╲╱   ╨─╴ ║ ╲║    ╶╨╴   ╙──╜  ╶╨╴╲      ╶╨╴\n");
            break;
    }
}

void printMenu(int cursor){
    /*Prints the menu in the title screen and what you're hovering on
    int : cursor : allows to hover on the option chosen*/
    printLetters(1);        //Print Cosmic Yonder
    printw("\n\n  New Game\n  Continue\n  Settings\n  Quit\n");         //Different options in title screen

    mvprintw(9+cursor,0,"⁕");      //Shows the option your hovering on
    mvprintw(13,0,"");          //Move cursor to the end so it doesn't bother the screen
}

void printroom(Room room){
    /*Not done yet
    Prints (with wall and door) the room given
    Room : room : The room you want to print*/
}

int quitGame(){
    /*The whole concept of quitting is hard for us. So will it be for you
    Makes you choose to quit
    return 1 to quit and 0 if not quit*/
    int quitter = 0, k=-1;
    while (k!=10){
        clear();
        printw("Do you really want to quit ?\n\n  No        Yes");
        mvprintw(2,quitter*11,"⁕");
        mvprintw(3,0,"");
        refresh();
        k = getch();

        if (k == KEYLEFT){              //Move Left
            quitter = 0;
        }
        else if (k == KEYRIGHT){        //Move Right
            quitter = 1;
        }
    }

    if (quitter){
        k=-1;
        quitter = 0;
        while (k!=10){
            clear();
            printw("Are you sure you want to give up on this game ?\n\n  No\n  Please no\n  Yes I hate this game");
            mvprintw(quitter+2,0,"⁕");
            mvprintw(5,0,"");
            refresh();
            k = getch();

            switch(k){
                case KEYUP:
                    if (quitter>0){
                        quitter--;
                    }
                    break;
                case KEYDOWN:
                    if (quitter<2){
                        quitter++;
                    }
                    break;
            }
        }
        if (quitter==2){
            clear();
            printw("Quitting game\nPress any key...");      //Actually quitting game
            refresh();
            getch();
            return 1;
        }
    }
    return 0;
}

int game(char* name, int* seed){
    /*Do nothing for now
    char* : name : the name entered
    int* : seed : The seed used
    return int : 0 if everything went well, 1 if there's an error*/
    endwin();
    return 0;
}

int creategame(char* name, int* seed){
    /*Ask for the name and seed number.
    char* : name : the adress of the name
    int* : seed : Adress of the seed that will be used
    return int : 0 if everything went well, 1 if there's an error*/

    char temp_name[21];
    int confirm=1,len=0, k=-1;

    while (confirm){
        if (name != NULL){      //Free the name if there's something in it already
            free(name);
        }

        do{                 //Ask for seed
            clear();
            printLetters(2);        //print SEED
            printw("Enter your seed number (must be positive): ");
            scanw("%d",seed);
            refresh();
        }while (*(seed)<=0);                //!\ conditions for the seed

        do{                 //Ask name so you can see character limit (20)
            clear();
            printLetters(3);        //Print NAME
            printw("Seed number : %d\n",*seed);     //Show seed chosen before just in case
            printw("Enter your name : ");
            if (((k<91)&&(k>64))||((k<58)&&(k>47))||((k<123)&&(k>96))&&(len<NAMELIMIT)||(k==95)){       //Maj + min + num + _
                *(temp_name+len) = k;
                len++;
            }
            else if ((k==263)&&(len>0)){        //If DEL
                *(temp_name+len) = 0;
                len--;
            }

            for (int i=0;i<len;i++){            //Print name chosen (stop at character limit)
                printw("%c", *(temp_name+i));
            }

            refresh();
            k = getch();
        } while (k!=10);        //Until you press enter

        name = malloc(sizeof(char)*(len+1));

        if (name==NULL){
            return 1;
        }
        refresh();
        
        for (int i=0;i<len;i++){                //If name chosen, the temp name will transfer to a dynamic string
            *(name+i) = *(temp_name+i);
        }

        k=-1;
        confirm = 0;
        clear();

        do{         //Ask to confirm
            switch(k){
                case 260:
                    confirm = 0;
                    break;
                case 261:
                    confirm = 1;
                    break;
            }

            printw("Seed number : %d        Name : %s\n",*seed,name);       //Show previous informations
            printw("Are the informations here correct? Confirm :\n  Yes        No");       //Ask to confirm
            mvprintw(2,confirm*11,"⁕");
            mvprintw(3,0,"");       //Move cursor to the end

            refresh();
            k = getch();
            clear();
        }while(k!=10);
    }
    return game(name,seed);
}

int titleScreen(){
    /*Print the title screen and allows you to chose options of the menu
    return int : 0 if everything went well, 1 if there's an error*/
    int cursor = 0, seed=0, k = -1;
    char* name = NULL;

    setlocale(LC_ALL,"");           //Set local for unicode characters
    initscr();
        keypad(stdscr, TRUE);       //To allow arrow keys

        while (k != 27){            //Until you press escape
            switch (k){
                case KEYDOWN:           //Down
                    cursor++;
                    break;
                case KEYUP:           //Up
                    cursor--;
                    break;
                case 10:            //If enter key pressed
                    switch(cursor){
                        case 0:                         //New game
                            return creategame(name,&seed);
                            break;
                        case 1:                         //Load save
                            break;
                        case 2:                         //Settings
                            break;
                        case 3:
                            if (quitGame()==1){           //Quit
                                endwin();
                                return 0;
                            }
                            break;
                    }
            }
            
            if (cursor == -1){      //To be able to go up and chose the bottom option
                cursor = 3;
            }
            cursor = cursor%4;

            clear();
            printMenu(cursor);
            refresh();

            k= getch();
        }
    endwin(); //Close window if escape is pressed
    return 0;
}


int main(){
    Room salle;
    int seed=-1, error = 0;
    
    //error = titleScreen();
    setlocale(LC_ALL,"");           //Set local for unicode characters
    initscr();
    printLetters(5);
    refresh();
    getch();
    endwin();
    if (error){            //Print the error if one (if menu return != 0) => Robustesse
        switch(error){
            case 1:
                printf("Erreur d'allocation mémoire");
                return error;
            default:
                printf("Ceci est un message d'erreur");
                return -1;
                break;  //On peut mettre les différentes erreurs avec des messages différents.
        }
    }
    return 0;
}
