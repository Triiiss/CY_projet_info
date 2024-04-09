#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>  //library that allows to move with arrow keys (need to check docstring)

typedef struct 
{
    int x; //player position on the x axis
    int y; //player position on the y axis
    int pv; // player health, must be above 0
    float exp; //player experience point
    int* bag;  // inventory of the player, contain object
}Player;

int titleScreen(){
    printf("╓─╴  ╓──╖  ╓─╴       ╶╥╴ ╓─╴\n");  //Title  1
    printf("║    ║  ║  ╙─╖  ║╲╱║  ║  ║  \n");
    printf("╙─╴  ╙──╜  ╶─╜ ╶╨╴ ╨╴╶╨╴ ╙─╴\n");

    printf("         ╓─╴        \n");  //Title 2
    printf("╓╴  ╓─╖  ╙─╖  ║╲╱║ ╶╥╴ ╓╴ \n");
    printf("╙╴  ╙─╜  ╶─╜ ╶╨╴ ╨╴╶╨╴ ╙╴\n");

    printf("      ╶╥╴ ╶╥╴\n");
    printf("       ╙───╢  ╓──╖       ╶╥──╮  ╥─╴  ╶╥─╮\n");
    printf("           ║  ║  ║  ║╲ ║  ║  │  ╟╴    ╟─╯\n");
    printf("          ╶╨╴ ╙──╜  ║ ╲║ ╶╨──╯  ╨─╴  ╶╨╴╲\n");

    printf("\n\n");
    printf("New Game\n");
    printf("Continue Game\n");
    printf("Quit\n");
    

    return 0;
}


int main(){
    int seed=-1;
    int error = 0;

    error = titleScreen();

    if (error){            //Print the error if one (if menu return != 0) => Robustesse
        printf("Ceci est un message d'erreur");
        return -1;
    }


    
    return 0;
}