#include "main.h"

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
                break;  // Different error messages
        }
    }

    return 0;
}