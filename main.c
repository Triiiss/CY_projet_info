#include <stdio.h>
#define LONGUEUR 3
#define LARGEUR 3

typedef struct Map Map ;
struct Map
 {
    int res;
    Map* haut;
    Map* droite;
    Map* gauche;
    Map* bas;
};

typedef struct{
    int x;
    int y;
}Door;

typedef struct{
    int x;
    int y;
    int L;
    int l;
    int n_doors;
    Door* doors;
}Room;

long int magie(long int nb,int pui){
    if(pui<=1){
        return nb;
    }
    else{
        return magie((nb*(nb+150))%3037000499,pui-1);
    }
}

long int aleatoire(long int seed,int abs,int ord){

    long int res=0;
            res=magie(seed+2,ord); //+2 on seed in case it is zero or one
    for (int i=0;i<abs+3;i++){  // abs +3 in case it's equal to zero
        res=magie(res,ord+2);
    }
    return res;

}


void afficherMap(Map carte, int L, int l){
    Map louis;
    for (int i=0;i<L/2;i++){            //Monter en haut à gauche
        carte = *(carte.haut);
    }
    for (int i=0;i<l/2;i++){
        carte = *(carte.gauche);
    }

    for (int i=0;i<L;i++){
        louis = carte;

        for (int j=0;j<l;j++){
            printf("%d ",louis.res);
            if (louis.droite != NULL){
                louis = *(louis.droite);
            }
        }

        if (carte.bas != NULL){
            carte = *(carte.bas);
        }
        printf("\n");
    }
}

Room FirstRoom(int seed){
    Room salle;
    salle.L = aleatoire(seed,0,1)%12+3;
    salle.l = aleatoire(seed,1,0)%12+3;
    salle.n_doors = 4;
}

//Ta mère la pharmacienne Louis

void initialisation(Map* a){
    a->bas = NULL;
    a->droite = NULL;
    a->gauche = NULL;
    a->haut = NULL;
}


int main(){
    Map a,b,c,d,e,f,g,h,i;

    //a = constructeur(LONGUEUR,LARGEUR);

    a.res = 1;
    b.res = 2;
    c.res = 3;
    d.res = 4;
    e.res = 5;
    f.res = 6;
    g.res = 7;
    h.res = 8;
    i.res = 9;

    initialisation(&a);
    initialisation(&b);
    initialisation(&c);
    initialisation(&d);
    initialisation(&e);
    initialisation(&f);
    initialisation(&g);
    initialisation(&h);
    initialisation(&i);

    a.droite = &b;
    a.bas = &d;

    b.gauche = &a;
    b.droite = &c;
    b.bas = &e;

    c.gauche = &b;
    c.bas = &f;

    d.haut = &a;
    d.droite = &e;
    d.bas = &g;

    e.gauche = &d;
    e.haut = &b;
    e.droite = &f;
    e.bas = &h;

    f.gauche = &e;
    f.bas = &i;
    f.haut = &c;

    g.haut = &d;
    g.droite = &h;

    h.gauche = &g;
    h.haut = &e;
    h.droite = &i;

    i.droite = &h;
    i.haut = &f;

    afficherMap(e,LONGUEUR,LARGEUR);


    return 0;
}
