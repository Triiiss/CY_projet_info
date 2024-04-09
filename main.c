#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>          //Library that allows to print Unicode text
<<<<<<< HEAD
#include <io.h>
//#include <ncurses.h>  //library that allows to move with arrow keys (need to check docstring)
=======
//#include <curses.h>  //library that allows to move with arrow keys (need to check docstring)
>>>>>>> 16665135ca981cd71acf3a630363de9236e2727b

typedef struct 
{
    int x; //player position on the x axis
    int y; //player position on the y axis
    int pv; // player health, must be above 0
    float exp; //player experience point
    int* bag;  // inventory of the player, contain object
}Player;

int titleScreen(){
    _setmode(_fileno(stdout),_O_U16TEXT);       //Allows to print Unicode text 
    wprintf(L"╓─╴  ╓──╖  ╓─╴       ╶╥╴ ╓─╴\n");  //Title
    wprintf(L"║    ║  ║  ╙─╖  ║╲╱║  ║  ║  \n");
    wprintf(L"╙─╴  ╙──╜  ╶─╜ ╶╨╴ ╨╴╶╨╴ ╙─╴\n");
    wprintf(L"      ╶╥╴ ╶╥╴\n");
    wprintf(L"       ╙───╢  ╓──╖       ╶╥──╮  ╥─╴  ╶╥─╮\n");
    wprintf(L"           ║  ║  ║  ║╲ ║  ║  │  ╟╴    ╟─╯\n");
    wprintf(L"          ╶╨╴ ╙──╜  ║ ╲║ ╶╨──╯  ╨─╴  ╶╨╴╲\n");

    printf("\n\n");

    wprintf(L"New Game\n");                 //Choices
    wprintf(L"Continue Game\n");
    wprintf(L"Quit\n");
    

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
