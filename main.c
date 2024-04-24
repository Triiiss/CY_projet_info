#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <ncurses.h>  //library that allows to move with arrow keys (need to check docstring)

typedef struct 
{
    int x; //player position on the x axis
    int y; //player position on the y axis
    int pv; // player health, must be above 0
    float exp; //player experience point
    int* bag;  // inventory of the player, contain object
}Player;

void afficher(int curseur){
    printw("╓─╴  ╓──╖  ╓─╴       ╶╥╴ ╓─╴\n");  //Print Title Screen
    printw("║    ║  ║  ╙─╖  ║╲╱║  ║  ║  \n");
    printw("╙─╴  ╙──╜  ╶─╜ ╶╨╴ ╨╴╶╨╴ ╙─╴\n");
    printw("      ╶╥╴ ╶╥╴\n");
    printw("       ╙───╢  ╓──╖       ╶╥──╮  ╥─╴  ╶╥─╮\n");
    printw("           ║  ║  ║  ║╲ ║  ║  │  ╟╴    ╟─╯\n");
    printw("          ╶╨╴ ╙──╜  ║ ╲║ ╶╨──╯  ╨─╴  ╶╨╴╲\n");

    printw("\n\n");

    printw("  New Game\n");         //Options
    printw("  Continue\n");
    printw("  Settings\n");
    printw("  Quit\n");

    mvprintw(9+curseur,0,"⁕");      //Print the option your hovering on
    mvprintw(13,0,"");          //Move cursor to the end
}

int quitter(){
    int quitter = 0;
    int k=-1;
    while (k!=10){
        clear();
        printw("Do you really want to quit ?");
        printw("\n\n");
        printw("  No        Yes");
        mvprintw(2,quitter*11,"⁕");
        mvprintw(3,0,"");
        refresh();
        k = getch();

        switch(k){
            case 260:
                if (quitter){
                    quitter = 0;
                }
                break;
            case 261:
                if (quitter == 0){
                    quitter = 1;
                }
                break;
        }
    }
    if (quitter){
        k=-1;
        quitter = 0;
        while (k!=10){
            clear();
            printw("Are you sure you want to give up on this game ?");
            printw("\n\n");
            printw("  No        No         Yes");
            mvprintw(2,quitter*11,"⁕");
            mvprintw(3,0,"");
            refresh();
            k = getch();

            switch(k){
                case 260:
                    if (quitter>0){
                        quitter--;
                    }
                    break;
                case 261:
                    if (quitter<2){
                        quitter++;
                    }
                    break;
            }
        }
        if (quitter==2){
            clear();
            printw("What have you done?");
            refresh();
            getch();
            return 1;
        }

    }
    return 0;
}

int titleScreen(){
    int curseur = 0;
    int k = -1;

    setlocale(LC_ALL,"");           //Set local for unicode characters
    initscr();
        keypad(stdscr, TRUE);       //For arrow keys

        afficher(0);
        k = getch();
        while (k != 27){            //Until you press escape
            switch (k){
                case 258:           //Down
                    curseur++;
                    break;
                case 259:           //Up
                    curseur--;
                    break;
                case 10:
                    switch(curseur){
                        case 0:                         //New game
                            break;
                        case 1:                         //Load save
                            break;
                        case 2:                         //Settings
                            break;
                        case 3:
                            if (quitter()==1){           //Quit
                                endwin();
                                return 0;
                            }
                            break;
                    }
            }
            
            if (curseur == -1){
                curseur = 3;
            }
            curseur = curseur%4;

            clear();
            afficher(curseur);
            refresh();

            k= getch();
        }


    endwin();

    return 0;
}


int main(){
    int seed=-1;
    int error = 0;
    
    error = titleScreen();

    if (error){            //Print the error if one (if menu return != 0) => Robustesse
        printw("Ceci est un message d'erreur");
        return -1;
    }
    
    return 0;
}
