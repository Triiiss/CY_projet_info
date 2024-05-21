#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#define SIZE 100
#define MSALLE 5
#define MOBJET 5

typedef struct{
    Object bag[4];           //Inventory, contains object
    int x;              //Character's position on the x axis
    int y;              //Character's position on the y axis
    int tasksDone;
    int pv;             //Health, must be above 0
    int exp;            //Experience points
    int hold;
}Character;

typedef struct{
    int epee_bois;
    int epee_diament;
    int potion;
    int pistolet;
    int livre;
}Type;

typedef struct{
    int x;
    int y;
    Type type;
}Objet;

typedef struct{
    int nbr_objet;
    Objet objet[SIZE];
    int tailleminx;
    int tailleminy;
    int taillex;
    int tailley;
}Salle;

Salle tab[SIZE];




Salle valeurmaxx(Salle salle){
    if(salle.tailleminx>salle.taillex){
        salle.taillex=salle.tailleminx-(salle.tailleminx-salle.taillex);
    }                                                                               // si la veleur du min de la salle est inférieur à tailleminx, faire un calcul pour soustraire
    return salle;
}
Salle valeurmaxy(Salle salle){
    if(salle.tailleminy>salle.tailley){
        salle.tailley=salle.tailleminy-(salle.tailleminy-salle.tailley);
    }
    
}
//la salle a un mur au coordonnée 0 et 5;
Objet jsp(Type type, Salle n){
    Objet objet;
    objet.type=type;
    int tailley=5;
    int taillex=5;
    objet.x=rand()%(taillex-1)+1 +n.tailleminx;
    objet.y=rand()%(tailley-1)+1 +n.tailleminy;
    return objet;
}


void blabla(int nbrsalle,int nbrobjet,Salle salle){
    int nbrb=0;
    int nbrd=0;
    int nbrpo=0;
    int nbrpi=0;

    while(1==1){
        int i=rand()%101;
        if(i<=5){
            if(nbrd==1){
                return;
            }
            tab[nbrsalle].objet[nbrobjet]=jsp((Type){.epee_diament=1},salle );          //je veux que ca soit un int jsp ou un truc comme ca pour qu'a chaque fois que j'utilise cette fonction, il y a un nouveau objet avec de nouvelle coordonnées
            printf("Salle %d, objet %d: epee en diament: x=%d et y=%d\n",nbrsalle,nbrobjet,tab[nbrsalle].objet[nbrobjet].x,tab[nbrsalle].objet[nbrobjet].y);
           
            nbrd++;
        }
        else if(i<=45 && i>5){
            if(nbrb==1){
                return ;
            }          
            tab[nbrsalle].objet[nbrobjet]=jsp((Type){.epee_bois=1},salle);
            printf("Salle %d, objet %d: epee en bois: x=%d et y=%d\n",nbrsalle,nbrobjet,tab[nbrsalle].objet[nbrobjet].x,tab[nbrsalle].objet[nbrobjet].y);
            nbrb++;
        }
        else if(i<=80 && i>45){
            if(nbrpo==1){
                return;
            }
            tab[nbrsalle].objet[nbrobjet]=jsp((Type){.potion=1},salle);
            printf("Salle %d, objet %d: potion: x=%d et y=%d\n",nbrsalle,nbrobjet,tab[nbrsalle].objet[nbrobjet].x,tab[nbrsalle].objet[nbrobjet].y);
            nbrpo++;
        }
        else if(i>80){            
             if(nbrpi==1){
                return ;
            }
            tab[nbrsalle].objet[nbrobjet]=jsp((Type){.pistolet=1},salle);
            printf("Salle %d, objet %d: pistolet: x=%d et y=%d\n",nbrsalle,nbrobjet,tab[nbrsalle].objet[nbrobjet].x,tab[nbrsalle].objet[nbrobjet].y);
            nbrpi++;
        }
        printf("\n\n%d%d\n",salle.tailleminx,salle.tailleminy);
        nbrobjet++;
       
    }
    
}

int déplacement(Player n,Room r,int *map){
    Items i;

    int d;
    d = getch();
     switch(d){                 //si j'ai bien compris, c'est la version que vous voulez
        case KEYUP:                                   
            if((n.y-1==map[y-1]) && (n.x==map[x])){     //regarde si la case où l'on veut aller a la même valeur que la case sur laquelle on est.
                n.y-=1;                                // avance à la case que l'on veut.
            }
        break;
        case KEYRIGHT:
            if((n.x+1==map[x+1]) && (n.y==map[y])){
                n.x+=1;
            }
        break;
        case KEYDOWN:
            if((n.y+1==map[y+1]) && (n.x==map[x])){
                n.y+=1;
            }
        break;
        case KEYLEFT:
            if((n.x-1==map[x-1]) && (n.y==map[y])){
                n.x-=1;
            }
        break;
        /*case 'e':
            for(int h=0;h<nbrsalle;h++){
                for(int t=0;t<nbrobjet;t++){
                    if((n.x==tab[nbrsalle].objet[nbrobjet].x)&&(n.y==tab[nbrsalle].objet[nbrobjet].x)){
                        if(taillebag<4){
                            n.bag[taillebag]=     //superposer??
                            
                        }
                    }
                }
            }
        break;
            */
        default:
        break;
    }
    
}


int main(){
    srand(time(NULL)); 
    Salle salle;
    Character n;
    int tabf[SIZE];       
    int nbrsalle=1;
    int toto=0;
    int nbrobjet=1;
    salle.tailleminx=0;
    salle.tailleminy=0;
    int i=0;
    salle.taillex=0;
    salle.tailley=0;
    while(toto<4){
        salle.taillex=salle.tailleminx;
        salle.tailley=salle.tailleminy;
        if(i==1){
            salle.tailleminx=5;                          //salle.tailleminx vaut la valeur des plus petites coordonnées de la salle
            salle.tailleminy=5;                          // salle.taillex  est une variable qui permet de comparer. il faut utiliser pour dire si ca plus ca < salle.taille minx + pzef
        }
        else if (i==2){
            salle.tailleminx=8;                          // je ne sais pas comment je dois faire pour prendre la valeur min de la salle
            salle.tailleminy=10;
        }
        else if(i==3){
            salle.tailleminx=5;
            salle.tailleminy=6;
        }
        else if(i==4){
            salle.tailleminx=10;
            salle.tailleminy=11;
        }
        else if(i==5){
            salle.tailleminx=0;
            salle.tailleminy=0;
            
        }
        valeurmaxx(salle);
        valeurmaxy(salle);
       
        blabla(nbrsalle,nbrobjet,salle); //^prendre le minimum de la salle et l'ajouter ensuite au nombre aléatoire
        printf("\n--------------------------\n");
        toto++;
        nbrsalle++;
    i++;
    }
    return 0;
    
}



