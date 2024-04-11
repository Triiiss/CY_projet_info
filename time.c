#include <stdio.h>
#include <time.h>
#include<stdlib.h>
int main() {
    int duree = 10;
    time_t tempsDebut = time(NULL);
    int timerestant = duree;
    int *p1=0;
    
    while (1) {
        time_t tempsActuel = time(NULL);
        int time=duree /*  - *p1   */;
        time=time-(tempsActuel - tempsDebut);
        //int time = duree - (tempsActuel - tempsDebut) - p1;

        if (time != timerestant) {
            printf("Il reste %d seconde\n", time);
            timerestant = time;
        }
        if(time<1){
            printf("perdu");
            return 0;
        }
        if(60<50/*je ne sais pas la condition*/){
            *p1=50-time;
            exit(1);
        }
    }
    printf("perdu");
    
    return 0;
}
//pour le temps quand il y a la sauvegarde, je pensais a rajouter un pointeur qui prends le temps 
//déja mis avant de faire la sauvegarde pour aller au menu principale.
