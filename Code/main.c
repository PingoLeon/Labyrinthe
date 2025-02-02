#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include <windows.h>

//Structure d'une cellule sur un plateau de jeu :
//Attributs : - Position (x,y)
//            - Type ( Forme T, L, I) (Croisement, Tournant, Droit)
//            - Orientation (1,2,3,4)
//            - Compatibilité(1,2,3,4) (1: haut, 2: droite, 3: bas, 4: gauche)
//            - Mobilité (True, False) (True: la cellule peut bouger, False: la cellule ne peut pas bouger)
//            - Trésor (True, False) (True: la cellule contient un trésor, False: la cellule ne contient pas de trésor)
//            - Type_trésor (24 types de trésors) (1: trésor de type 1, 2: trésor de type 2, etc...)

typedef struct cellule{
int x;
int y;
int type;
int orientation;
int compatibilite[4];
bool mobilité;
bool tresor;
int type_tresor;
}cellule;

// Structure qui permet de créer un pion
struct Pion
{
    int numero;
    int x;
    int y;
    int numeroPerso;
    char symbole;
    bool estEnTrainDeJouer;
    char cartes[12];
    char Personnage[20];
};

// On génère les 4 pions possibles
struct Pion pion1 = {1, 0, 0, '*', true};
struct Pion pion2 = {2, 0, 6, '*', true};
struct Pion pion3 = {3, 6, 0, '*', true};
struct Pion pion4 = {4, 6, 6, '*', true};

struct Pion liste_pion[4];
typedef struct character {
    char name[20];
} character;

//Définition des attibuts de la structure cellule
#define T 1
#define L 2
#define I 3
#define HAUT 1
#define DROITE 2
#define BAS 3
#define GAUCHE 4

/*
0 : Noir
1 : Bleu foncé
2 : Vert foncé
3 : Turquoise
4 : Rouge foncé
5 : Violet
6 : Vert caca d'oie
7 : Gris clair
8 : Gris foncé
9 : Bleu fluo
10 : Vert fluo
11 : Turquoise
12 : Rouge fluo
13 : Violet 2
14 : Jaune
15 : Blanc
*/
#define COULEUR_TEXTE 15
#define COULEUR_BLOC_IMMOBILE 12
#define COULEUR_FOND 0


//Création d'un plateau de 7*7 cases ( 49 cases au total) (liste de liste de cellules)
cellule plateau[7][7];

//Création d'une cellule global_tile qui va être utilisée pour faire bouger le plateau
cellule global_tile; 
cellule global_tile_temp;
int nbJoueurs = 0;


//initialisation de la couleur du texte et du fond de la console
void Color(int couleurDuTexte,int couleurDeFond) // fonction d'affichage de couleurs
{
HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}

/*Comportement d'une cellule
- Une cellule type T ou L(type = 1 ou 2) a 4 orientations possibles, tandis qu'une cellule type I(type = 3) a 2 orientations possibles
- Les cellules ont 4 comptaibilités possibles, une pour chaque côté de la cellule, et si la cellule est sur le bord du plateau, elle n'a pas de compatibilité pour le côté correspondant
- Les cellules sont initialisées avec une mobilité à true, sauf celles qui ont leurs deux coordonnées paires.
- les 4 coins du plateau sont destinés à l'apparition des pions, et toutes les autres cases inamovibles (x et y paires) sont destinées à l'apparition des trésors
*/

/*Nombre de cellules et répartition du plateau : 
Le jeu comporte 16 tuiles inamovibles (x et y paires), et 34 tuiles amovibles (x et y impaires)
Parmis les 16 tuiles inamovibles : 
- 4 sont destinées à l'apparition des pions en L
- 12 sont destinées à l'apparition des trésors en T

Parmis les 34 tuiles amovibles, on compte :
- 6 Tuiles en T avec un trésor
- 6 Tuiles en L avec un trésor
- 10 tuiles en L sans trésor
- 12 tuiles en I sans trésor

Note : la tuile restante est mise à disposition des joueurs pour faire bouger le labyrinthe
*/

/*Compatibilité des cellules :

DANS LA LISTE COMPATBILITE, LES INDICES DES ELEMENTS CORRESPONDENT AUX DIRECTIONS (0: haut, 1: droite, 2: bas, 3: gauche)
En fonction de la compatibilité de la cellule, on met dans la liste de compatibilité toutes les directions possibles

cellule T : 4 orientations possibles, 4 compatibilités possibles
orientation 1 : T -> droite, bas, gauche
orientation 2 : T -> haut, bas, gauche
orientation 3 : T -> haut, droite, gauche
orientation 4 : T -> haut, droite, bas

cellule L : 4 orientations possibles, 4 compatibilités possibles
orientation 1 : L -> haut, droite
orientation 2 : L -> droite, bas
orientation 3 : L -> bas, gauche
orientation 4 : L -> gauche, haut

cellule I : 2 orientations possibles, 2 compatibilités possibles
orientation 1 : I -> haut, bas
orientation 2 : I -> droite, gauche
*/

//Fonction de mise à jour des compatibilités des cellules en fonction de leurs types et orientations
void maj_compatibilite(){
    int i, j;
    for(i=0;i<7;i++){
        for(j=0;j<7; j++){
            switch (plateau[i][j].type){
                case T:
                    if(plateau[i][j].orientation == 1){
                        //update the list of compatibilities
                        plateau[i][j].compatibilite[0] = 0;
                        plateau[i][j].compatibilite[1] = 1;
                        plateau[i][j].compatibilite[2] = 1;
                        plateau[i][j].compatibilite[3] = 1;
                    }else if(plateau[i][j].orientation == 2){
                        //update the list of compatibilities
                        plateau[i][j].compatibilite[0] = 1;
                        plateau[i][j].compatibilite[1] = 0;
                        plateau[i][j].compatibilite[2] = 1;
                        plateau[i][j].compatibilite[3] = 1;
                    }else if(plateau[i][j].orientation == 3){
                        //update the list of compatibilities
                        plateau[i][j].compatibilite[0] = 1;
                        plateau[i][j].compatibilite[1] = 1;
                        plateau[i][j].compatibilite[2] = 0;
                        plateau[i][j].compatibilite[3] = 1;
                    }else if(plateau[i][j].orientation == 4){
                        //update the list of compatibilities
                        plateau[i][j].compatibilite[0] = 1;
                        plateau[i][j].compatibilite[1] = 1;
                        plateau[i][j].compatibilite[2] = 1;
                        plateau[i][j].compatibilite[3] = 0;
                    }
                    break;
                case L:
                    if (plateau[i][j].orientation == 1){
                        //update the list of compatibilities
                        plateau[i][j].compatibilite[0] = 1;
                        plateau[i][j].compatibilite[1] = 1;
                        plateau[i][j].compatibilite[2] = 0;
                        plateau[i][j].compatibilite[3] = 0;
                    }else if(plateau[i][j].orientation == 2){
                        //update the list of compatibilities
                        plateau[i][j].compatibilite[0] = 0;
                        plateau[i][j].compatibilite[1] = 1;
                        plateau[i][j].compatibilite[2] = 1;
                        plateau[i][j].compatibilite[3] = 0;
                    }else if(plateau[i][j].orientation == 3){
                        //update the list of compatibilities
                        plateau[i][j].compatibilite[0] = 0;
                        plateau[i][j].compatibilite[1] = 0;
                        plateau[i][j].compatibilite[2] = 1;
                        plateau[i][j].compatibilite[3] = 1;
                    }else if(plateau[i][j].orientation == 4){
                        //update the list of compatibilities
                        plateau[i][j].compatibilite[0] = 1;
                        plateau[i][j].compatibilite[1] = 0;
                        plateau[i][j].compatibilite[2] = 0;
                        plateau[i][j].compatibilite[3] = 1;
                    }
                    break;
                case I:
                    if (plateau[i][j].orientation == 1){
                        //update the list of compatibilities
                        plateau[i][j].compatibilite[0] = 1;
                        plateau[i][j].compatibilite[1] = 0;
                        plateau[i][j].compatibilite[2] = 1;
                        plateau[i][j].compatibilite[3] = 0;
                    }else if(plateau[i][j].orientation == 2){
                        //update the list of compatibilities
                        plateau[i][j].compatibilite[0] = 0;
                        plateau[i][j].compatibilite[1] = 1;
                        plateau[i][j].compatibilite[2] = 0;
                        plateau[i][j].compatibilite[3] = 1;
                    }
                    break;
                default:
                    break;
            }

            //Si la cellule est au bord du plateau, on met à jour les compatibilités
            if(i==0){
                plateau[i][j].compatibilite[0] = 0;
            }
            if(i==6){
                plateau[i][j].compatibilite[1] = 0;
            }
            if(j==0){
                plateau[i][j].compatibilite[3] = 0;
            }
            if(j==6){
                plateau[i][j].compatibilite[2] = 0;
            }
        }     
    }
}

//Fonction qui génère la dernière tuile excroissante
void derniere_tuile(int t_counter, int l_counter, int l_nontresor_counter, int i_nontresor_counter){

    // Seed the random number generator with the current time
    srand(time(NULL));
    int choosen = 1;
    int random = rand()%4+1;
    while(choosen == 1){    
        if(random == 1 && t_counter < 6){
            global_tile.type = T;
            global_tile.orientation = rand()%4+1;
            global_tile.tresor = true;
            global_tile.type_tresor = rand()%24+1;
            t_counter++;
            choosen = 0;
        }else if(random == 2 && l_counter < 6){
            global_tile.type = L;
            global_tile.orientation = rand()%4+1;
            global_tile.tresor = true;
            global_tile.type_tresor = rand()%24+1;
            l_counter++;
            choosen = 0;
        }else if(random == 3 && l_nontresor_counter < 10){
            global_tile.type = L;
            global_tile.orientation = rand()%4+1;
            global_tile.tresor = false;
            global_tile.type_tresor = 0;
            l_nontresor_counter++;
            choosen = 0;
        }else if(random == 4 && i_nontresor_counter < 12){
            global_tile.type = I;
            global_tile.orientation = rand()%2+1;
            global_tile.tresor = false;
            global_tile.type_tresor = 0;
            i_nontresor_counter++;
            choosen = 0;
        }else{
            random = rand()%4+1;
        }
    }
}

//Fonction de remplissage du plateau
void remplissage_plateau(){

// Seed the random number generator with the current time
srand(time(NULL));

//correction des cellules inamovibles
int t_counter = 0;  // counter for T cells with a treasure
int l_counter = 0;  // counter for L cells with a treasure
int l_nontresor_counter = 0;  // counter for L cells without a treasure
int i_nontresor_counter = 0;  // counter for I cells without a treasure
for(int i=0;i<7;i++){
    for(int j=0;j<7;j++){
        //16 tuiles inamovibles : On dit qu'elles ont un trésor 
        if(plateau[i][j].mobilité == false ){
            if ((i!=0 || j!=0) && (i!=0 || j!=6) && (i!=6 || j!=0) && (i!=6 || j!=6)){ // Si la cellule n'est pas une des 4 coins, elle a alors une forme de T, et si c'est un coin, elle possède un trésor
                plateau[i][j].type = T;
                plateau[i][j].tresor = true;
                plateau[i][j].type_tresor = rand()%24+1;
                plateau[i][j].orientation = rand()%4+1;
            }else{
                plateau[i][j].type = L;
                plateau[i][j].tresor = false;

                //corners should be L cells with different orientations
                if(i==0 && j==0){
                    plateau[i][j].orientation = 2;
                }else if(i==0 && j==6){
                    plateau[i][j].orientation = 3;
                }else if(i==6 && j==0){
                    plateau[i][j].orientation = 1;
                }else if(i==6 && j==6){
                    plateau[i][j].orientation = 4;
                }
            }  
        }else{
            //generate a random number and assign it to a type of cell between the 4 stated above
            int choosen = 1;
            int random = rand()%4+1;
            while(choosen == 1){    
                if(random == 1 && t_counter < 6){
                    plateau[i][j].type = T;
                    plateau[i][j].orientation = rand()%4+1;
                    plateau[i][j].tresor = true;
                    plateau[i][j].type_tresor = rand()%24+1;
                    t_counter++;
                    choosen = 0;
                }else if(random == 2 && l_counter < 6){
                    plateau[i][j].type = L;
                    plateau[i][j].orientation = rand()%4+1;
                    plateau[i][j].tresor = true;
                    plateau[i][j].type_tresor = rand()%24+1;
                    l_counter++;
                    choosen = 0;
                }else if(random == 3 && l_nontresor_counter < 10){
                    plateau[i][j].type = L;
                    plateau[i][j].orientation = rand()%4+1;
                    plateau[i][j].tresor = false;
                    plateau[i][j].type_tresor = 0;
                    l_nontresor_counter++;
                    choosen = 0;
                }else if(random == 4 && i_nontresor_counter < 12){
                    plateau[i][j].type = I;
                    plateau[i][j].orientation = rand()%2+1;
                    plateau[i][j].tresor = false;
                    plateau[i][j].type_tresor = 0;
                    i_nontresor_counter++;
                    choosen = 0;
                }else{
                    random = rand()%4+1;
                }
            }   
        }
    }
}

// generate the last tile
derniere_tuile(t_counter, l_counter, l_nontresor_counter, i_nontresor_counter);
}

//Fonction d'initialisation du plateau qui crée un plateau rempli de cellules définies uniquement par leur mobilité
void init_plateau(){
    //création d'un plateau aléatoire : 
    int i,j;
    for(i=0;i<7;i++){
        for(j=0;j<7;j++){
            plateau[i][j].x = i;
            plateau[i][j].y = j;
            plateau[i][j].mobilité = (i%2 == 0 && j%2 == 0) ? false : true;  
        }
    }
    //Appel de la fonction de correction du plateau qui va remplir les cellules inamovibles et les cellules amovibles, et créer la dernière cellule excroissante 
    remplissage_plateau();

    //Appel de la fonction de correction des compatibilités qui va remplir les compatibilités des cellules
    maj_compatibilite();
}

void fill(int nb){
    //each text has 3 characters possible, being filled with double space, or a full block (u2588)
    //We need all combinations of those 3 characters to fill the line
    switch (nb)
    {
    case 5:
        printf("\u2588\u2588  \u2588\u2588"); //101
        break;

    case 7:
        printf("\u2588\u2588\u2588\u2588\u2588\u2588"); //111
        break;

    default:
        break;
    }
}

void fill_pion(int nb, int x, int y){
    //each text has 3 characters possible, being filled with double space, or a full block (u2588)
    //We need all combinations of those 3 characters to fill the line
    char *pion_à_placer = " ";
    int isTherePion = 0;
    if ((liste_pion[0].x == x && liste_pion[0].y == y) || (liste_pion[1].x == x && liste_pion[1].y == y) || (liste_pion[2].x == x && liste_pion[2].y == y) || (liste_pion[3].x == x && liste_pion[3].y == y)){
        isTherePion = 1;
        pion_à_placer = "*";
    }else{
        isTherePion = 0;
        pion_à_placer = " ";
    }

    switch (nb)
    {
    case 0:
        printf("   %s  ", pion_à_placer); //000
        break;
    case 1:
        printf("  %s \u2588\u2588", pion_à_placer); //001
        break;

    case 4:
        printf("\u2588\u2588 %s  ", pion_à_placer); //100
        break;

    case 5:
        printf("\u2588\u2588 %s\u2588\u2588", pion_à_placer); //101
        break;
    default:
        break;
    }
}

void afficher_cellule_ligne1(int type, int orientation) {
    if (type == T) {
        if (orientation == 1){
            fill(7);
        } else if (orientation == 2){
            fill(5);
        } else if (orientation == 3){
            fill(5);
        } else if (orientation == 4){
            fill(5);
        }
    } else if (type == L) {
        if (orientation == 1){
            fill(5);
        }else if (orientation == 2){
            fill(7);
        } else if(orientation == 3){
            fill(7);
        } else if (orientation == 4){
            fill(5);
        }
    } else if (type == I) {
        if (orientation == 1){
            fill(5);
        } else if (orientation == 2){
            fill(7);
        }
    }
}

void afficher_cellule_ligne2(int type, int orientation, int x, int y){
    //if a pion is on the cell, if pion.x == x and pion.y == y, then we print the pion
    if (type == T) {
        if (orientation == 1){
            fill_pion(0,x,y);
        } else if (orientation == 2){
            fill_pion(1,x,y);
        } else if (orientation == 3){
            fill_pion(0,x,y);
        } else if (orientation == 4){
            fill_pion(4,x,y);
        }
    }else if (type == L) {
        if (orientation == 1){
            fill_pion(4,x,y);
        }else if (orientation == 2){
            fill_pion(4,x,y);
        } else if(orientation == 3){
            fill_pion(1,x,y);
        } else if (orientation == 4){
            fill_pion(1,x,y);
        }
    } else if (type == I) {
        if (orientation == 1){
            fill_pion(5,x,y);
        } else if (orientation == 2){
            fill_pion(0,x,y);
        }
    }
}

void afficher_cellule_ligne3(int type, int orientation){
    if (type == T) {
        if (orientation == 1){
            fill(5);
        } else if (orientation == 2){
            fill(5);
        } else if (orientation == 3){
            fill(7);
        } else if (orientation == 4){
            fill(5);
        }
    }else if (type == L){
        if (orientation == 1){
            fill(7);
        }else if (orientation == 2){
            fill(5);
        } else if(orientation == 3){
            fill(5);
        } else if (orientation == 4){
            fill(7);
        }
    } else if (type == I){
        if (orientation == 1){
            fill(5);
        } else if (orientation == 2){
            fill(7);
        }
    }
}

void afficher_plateau() {
    setlocale(LC_ALL, "en_US.utf8");
    //print all numbers of row separated by a tabulation
    printf("x  y--\t   \t2           4           6      \n|\n");
    for (int i = 0; i < 7; i++) {
        for(int num_line = 0; num_line < 3; num_line++){
            if (num_line == 1){
                if (i%2 != 0){
                    printf("%d", i+1);
                }
            }   
            printf("\t");
            for (int j = 0; j < 7; j++) {
                if (num_line == 0){
                    if (plateau[i][j].mobilité == false){
                        Color(COULEUR_BLOC_IMMOBILE,COULEUR_FOND);
                    } else {
                        Color(COULEUR_TEXTE,COULEUR_FOND);
                    }
                    afficher_cellule_ligne1(plateau[i][j].type, plateau[i][j].orientation);
                    Color(15,0);
                } else if (num_line == 1){
                    if (plateau[i][j].mobilité == false){
                        Color(COULEUR_BLOC_IMMOBILE,COULEUR_FOND);
                    } else {
                        Color(COULEUR_TEXTE,COULEUR_FOND);
                    }
                    afficher_cellule_ligne2(plateau[i][j].type, plateau[i][j].orientation, plateau[i][j].x, plateau[i][j].y);
                    Color(15,0);
                } else if (num_line == 2){
                    if (plateau[i][j].mobilité == false){
                        Color(COULEUR_BLOC_IMMOBILE,COULEUR_FOND);
                    } else {
                        Color(COULEUR_TEXTE,COULEUR_FOND);
                    }
                    afficher_cellule_ligne3(plateau[i][j].type, plateau[i][j].orientation);
                    Color(15,0);
                }
            }
            if (num_line == 1){
                if (i%2 != 0){
                    printf("\t%d", i+1);
                }
            }   
            printf("\n");
        }
    }
    printf("\n\t   \t2           4           6      ");
}

//Fonction permettant de placer les tresors en fonction de la tuile imposée
//Si la tuile est déjà occupée, on ne fait rien
//Les tuiles I et celles sur les bords ne peuvent pas contenir de trésor
//Les tuiles L et T peuvent contenir un trésor si elles sont libres et si elles ne sont pas sur les bords du plateau
//il faut 6L avec tresor
//il faut 4L avec position des pions
//il faut 6L sans tresor
//il faut 6T avec tresor
//il faut 12 I sans tresor

void insertion_cellule(){
    int state = 0;
    int x, y;
    while(state == 0){
        char choix[3];

        //Affichage de la tuile globale
        printf("\n\n\nVoici la tuile en trop : \n");
        afficher_cellule_ligne1(global_tile.type, global_tile.orientation); 
        printf("\n");
        afficher_cellule_ligne2(global_tile.type, global_tile.orientation, -1, -1);
        printf("\n");
        afficher_cellule_ligne3(global_tile.type, global_tile.orientation);

        //Affichage du menu
        printf("\n\nA quelles coordonnées souhaitez-vous insérer la tuile ? (x,y) \n");
        printf("Entrez x , suivi de y, le tout coupé par une virgule : ");
        scanf("%d,%d", &x, &y);
        x -= 1;
        y -= 1;
        //Vérification de la saisie
        if(x >= 0 && x <= 7 && y >= 0 && y <= 7){
            //One of the two coordinates is either 0 or 6 we must verify it
            if(x == 0 || x == 6 || y == 0 || y == 6){
                state = 1;
                printf("Tuile insérée en %d,%d !\n", x, y);
            }
        }

        if(state == 0){
            printf("Attention : La saisie est incorrecte ! \n");
        }
    }

    //insertion tuile en x et y
    //Si L'utilisateur a choisi x = 6,  l'utilisateur l'a alors inséré sur une ligne, si y=6, l'utilisateur l'a inséré sur une colonne
    // Save the cellule at the opposite side of the plateau
    cellule opposite_cellule;
    if (x == 6) {
        opposite_cellule = plateau[0][y];
    }
    else if (x == 0) {
        opposite_cellule = plateau[6][y];
    }
    else if (y == 6) {
        opposite_cellule = plateau[x][0];
    }
    else if (y == 0) {
        opposite_cellule = plateau[x][6];
    }

    // Set the global_tile_temp to the opposite cellule
    global_tile_temp = opposite_cellule;

    // Move all the cells in the row or column
    if (x == 6) {
        // Move all the cells upside in the column
        for (int i = 0; i < 6; i++) {
            plateau[i][y] = plateau[i+1][y];
        }
    }else if(x == 0){
        // Move all the cells downside in the column
        for (int i = 6; i > 0; i--) {
            plateau[i][y] = plateau[i-1][y];
        }
    }else if(y == 6){
        // Move all the cells leftside in the row
        for (int i = 0; i < 6; i++) {
            plateau[x][i] = plateau[x][i+1];
        }
    }else if(y == 0){
        // Move all the cells rightside in the row
        for (int i = 6; i > 0; i--) {
            plateau[x][i] = plateau[x][i-1];
        }
    }
        

    // Insert the global_tile into the plateau
    global_tile.mobilité = true;
    plateau[x][y] = global_tile;

    // Update the global_tile with the global_tile_temp
    global_tile = global_tile_temp;
}

//Fonction permettant le mouvement de pions
void mouvement_pion(int nbpion){
    bool state = false;
    while(state == false){
        //Affichage du menu
        char name[20];
        if(liste_pion[nbpion].numeroPerso == 1){
            strcpy(name, "Asterix");
        }else if(liste_pion[nbpion].numeroPerso == 2){
            strcpy(name, "Obelix");
        }else if(liste_pion[nbpion].numeroPerso == 3){
            strcpy(name, "Panoramix");
        }else if(liste_pion[nbpion].numeroPerso == 4){
            strcpy(name, "Idéfix");
        }

        printf("\n\nC'est le tour de %s et vous êtes en (%d,%d) \n",name, liste_pion[nbpion].x, liste_pion[nbpion].y);
        int x = liste_pion[nbpion].x;
        int y = liste_pion[nbpion].y;
        //verify if the cell the pion is on is compatible with the cell the pion wants to go
        printf("Souhaitez-vous aller en haut (1), à droite (2), en bas (3) ou à gauche (4) ou arrêter de bouger (5) ? \n");
        int choix;
        scanf("%d", &choix);
        switch (choix){
            case 1:
                //if the cell the pion is on is on the top border of the board, it can't go up
                if(liste_pion[nbpion].x == 0){
                    printf("Le mouvement est impossible car vous êtes sur une bordure\n");
                    break;
                }else{
                    printf("x = %d, y = %d, x-1 = %d, y = %d, plateau[x][y].compatibilite[0] = %d, plateau[x-1][y].compatibilite[1] = %d\n", x, y, x-1, y, plateau[x][y].compatibilite[0], plateau[x-1][y].compatibilite[1]);
                    if(plateau[x][y].compatibilite[0] == 1 && plateau[x-1][y].compatibilite[2] == 1){
                        liste_pion[nbpion].x = x-1;
                        liste_pion[nbpion].y = y;
                        afficher_plateau();
                    }
                    else{
                        printf("Le mouvement est impossible\n");
                    }
                }
                break;

            case 2:
                //if the cell the pion is on is on the right border of the board, it can't go right
                if(liste_pion[nbpion].y == 6){
                    printf("Le mouvement est impossible car vous êtes sur une bordure\n");
                    break;
                }else{
                    printf("x = %d, y = %d, x = %d, y+1 = %d, plateau[x][y].compatibilite[2] = %d, plateau[x][y+1].compatibilite[3] = %d\n", x, y, x, y+1, plateau[x][y].compatibilite[2], plateau[x][y+1].compatibilite[3]);
                    if(plateau[x][y].compatibilite[1] == 1 && plateau[x][y+1].compatibilite[3] == 1){
                        liste_pion[nbpion].x = x;
                        liste_pion[nbpion].y = y+1;
                        afficher_plateau();
                    }
                    else{
                        printf("Le mouvement est impossible\n");
                    }
                }
                break;
            case 3:
                //if the cell the pion is on is on the bottom border of the board, it can't go down
                if(liste_pion[nbpion].x == 6){
                    printf("Le mouvement est impossible car vous êtes sur une bordure\n");
                    break;
                }else{
                    printf("x = %d, y = %d, x+1 = %d, y = %d, plateau[x][y].compatibilite[1] = %d, plateau[x+1][y].compatibilite[0] = %d\n", x, y, x+1, y, plateau[x][y].compatibilite[1], plateau[x+1][y].compatibilite[0]);
                    if(plateau[x][y].compatibilite[2] == 1 && plateau[x+1][y].compatibilite[0] == 1){
                        liste_pion[nbpion].x = x+1;
                        liste_pion[nbpion].y = y;
                        afficher_plateau();
                    }
                    else{
                        printf("Le mouvement est impossible\n");
                    }
                }
                break;
            case 4:
                //if the cell the pion is on is on the left border of the board, it can't go left
                if(liste_pion[nbpion].y == 0){
                    printf("Le mouvement est impossible car vous êtes sur une bordure\n");
                    break;
                }else{
                    printf("x = %d, y = %d, x = %d, y-1 = %d, plateau[x][y].compatibilite[3] = %d, plateau[x][y-1].compatibilite[2] = %d\n", x, y, x, y-1, plateau[x][y].compatibilite[3], plateau[x][y-1].compatibilite[2]);
                    if(plateau[x][y].compatibilite[3] == 1 && plateau[x][y-1].compatibilite[1] == 1){
                        liste_pion[nbpion].x = x;
                        liste_pion[nbpion].y = y-1;
                        afficher_plateau();
                    }
                    else{
                        printf("Le mouvement est impossible\n");
                    }
                }
                break;
            case 5:
                state = true;
                break;
            default:
                printf("Attention : La saisie est incorrecte ! \n");
                break;
        }
    }
}


void Boucle(){
    //Tout est initialisé, c'est là la boucle de jeu pour lancer un tour et ainsi de suite
    //Boucle while du jeu, répétée tant que les joueurs veulent jouer
    //Boucle while d'un tour de joueur, répétée nbJoueurs fois, et ensuite on recommence
    int Jeu = 1;
    while(Jeu == 1){
        //Boucle while d'un tour de joueur, répétée nbJoueurs fois, et ensuite on recommence
        for(int i = 0; i < nbJoueurs; i++){

            //Affichage du menu
            char name[20];
            if(liste_pion[i].numeroPerso == 1){
                strcpy(name, "Asterix");
            }else if(liste_pion[i].numeroPerso == 2){
                strcpy(name, "Obelix");
            }else if(liste_pion[i].numeroPerso == 3){
                strcpy(name, "Panoramix");
            }else if(liste_pion[i].numeroPerso == 4){
                strcpy(name, "Idéfix");
            }
            printf("\n\n\nC'est au tour de %s !\n", name);
            //Afficher le plateau
            afficher_plateau();

            //Montrer la carte à chercher : montrer le premier élément de la liste de trésors du joueur : les trésors sont représentés par des lettres
            printf("\n\n\nDans ta liste de cartes à chercher, tu dois d'abord trouver : \n");
            switch (i+1){
                case 1:
                    printf("Trésor : %c\n", pion1.cartes[0]);
                    break;
                case 2:
                    printf("Trésor : %c\n", pion2.cartes[0]);
                    break;
                case 3:
                    printf("Trésor : %c\n", pion3.cartes[0]);
                    break;
                case 4:
                    printf("Trésor : %c\n", pion4.cartes[0]);
                    break;
                
            }

            carteDeFace();

            carteDeDos();

            
            //Lui proposer d'insérer une pièce ou non
            int choix = 0;
            while(choix == 0){
                printf("\n\nVoulez-vous insérer une tuile ? (1 = oui, 0 = non) : ");
                scanf("%d", &choix);
                if(choix == 1){
                    insertion_cellule();
                    printf("Voici le nouveau labyrinthe : \n\n");
                    afficher_plateau();
                    choix = 1;
                }
                else if(choix == 0){
                    printf("Vous avez choisi de ne pas insérer de tuile !\n");
                    choix = 1;
                }
                else{
                    printf("Attention : La saisie est incorrecte ! \n");
                }
            }

            //Lui proposer de bouger ou non
            choix = 0;
            while(choix == 0){
                printf("\n\nVoulez-vous bouger ? (1 = oui, 0 = non) : ");
                scanf("%d", &choix);
                if(choix == 1){
                    //Bouger le gars
                    mouvement_pion(i);
                    afficher_plateau();
                    choix = 1;
                }
                else if(choix == 0){
                    printf("Vous avez choisi de ne pas bouger !\n");
                    choix = 1;
                }
                else{
                    printf("Attention : La saisie est incorrecte ! \n");
                }
            }
        }

        //On demande si les joueurs veulent continuer
        int choix = 0;
        while(choix == 0){
            printf("\n\nUn tour est fini, voulez vous continuer ? (1 = oui, 0 = non) :");
            scanf("%d", &choix);
            if(choix == 1){
                printf("Vous avez choisi de continuer !\n");
            }
            else if(choix == 0){
                printf("Vous avez choisi de ne pas continuer ! Retour au menu principal !\n");
                Jeu = 0;
            }
            else{
                printf("Attention : La saisie est incorrecte ! \n");
            }
        }
    }

    //On retourne au menu principal
    main();
}


void init_pions(){
    //On initialise les pions
    // On crée un tableau de 4 personnages
    character characters []= {"Asterix: vert", "Obelix: bleu", "Panoramix: rouge", "Idefix: blanc"};

    // On crée un tableau de 4 points d'entrée (pointeurs) de personnages
    character* players[4];
    //Demander combien de joueurs
    while(nbJoueurs < 2 || nbJoueurs > 4){
        printf("Combien de joueurs ? (2 à 4) : ");
        scanf("%d", &nbJoueurs);
        if(nbJoueurs < 2 || nbJoueurs > 4){
            printf("Attention : La saisie est incorrecte ! \n");
        }
    }

    //set Pion.estEnTrainDeJouer = false if numero > nbJoueurs
    liste_pion[0] = pion1;
    liste_pion[1] = pion2;
    if(nbJoueurs == 2){
        pion3.estEnTrainDeJouer = false;
        pion3.x = -1;
        pion3.y = -1;
        pion4.estEnTrainDeJouer = false;
        pion4.x = -1;
        pion4.y = -1;

    }
    else if(nbJoueurs == 3){
        liste_pion[2] = pion3;
        pion4.estEnTrainDeJouer = false;
        pion4.x = -1;
        pion4.y = -1;
    }else if(nbJoueurs == 4){
        liste_pion[3] = pion4;
    }

    // On demande à chaque joueur de choisir un personnage
    for (int i = 0; i < nbJoueurs; i++) {
        printf("\nJoueur %d, choisissez un personnage :\n", i + 1);
        for (int j = 0; j < 4; j++) {
        printf("%d) %s\n", j + 1, characters[j].name);
        }
        printf("\nVotre choix : ");

        int choice;
        scanf("%d", &choice);
        printf("%d\n", choice);
        printf("\n\n");

        // On vérifie que le personnage n'a pas déjà été choisi par un autre joueur
        int available = 1;
        for (int j = 0; j < i; j++) {
            if (players[j] == &characters[choice - 1]) {
                available = 0;
                break;
            }
        }

        if (available == 1) {
            // Si le personnage n'a pas été choisi, on affecte le pointeur correspondant au joueur courant
            players[i] = &characters [choice - 1];
            liste_pion[i].numeroPerso = choice;
        } else {
            // Sinon, on demande au joueur de choisir un autre personnage
            printf("Ce personnage a déjà été choisi, choisissez-en un autre.\n");
            i--;
        }
    }
}

// CARTE TRESOR
// Création d'une carte de face 
void carteDeFace() {

  char map[10][10] = {
    {' ', '_', '_', '_', '_', '_', '_', '_', '_', ' '},
    {'|', ' ', 'T', 'r', 'e', 's', 'o', 'r', ' ', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
    {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
    {'|', '_', '_', '_', '_', '_', '_', '_', '_', '|'} };


    char tresors[24] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};

    for (int i = 0; i < 10; i++) 
    {
        for (int j = 0; j < 10; j++) 
        {

        printf("%c ", map[i][j]);// pour afficher la carte en blanc
        //\033[1;37mTexte en blanc\033[0m     blanc

        if ( i==5 && j==4)
        {
        srand(time(0));
        int k = rand() % 24;

        printf("%c ", tresors[k]);// pour afficher le trésor en jaune
        //\033[33mTexte en jaune\033[33m       jaune
        }
       }
     printf("\n");
    }
}

// Création d'une carte trésor de dos
void carteDeDos (){
    char map[10][10] = {
        {' ', '_', '_', '_', '_', '_', '_', '_', '_', ' '},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', 'T', 'r', 'e', 's', 'o', 'r', ' ', '|'},
        {'|', ' ', ' ', 'r', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', 'e', ' ', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', 's', ' ', ' ', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', 'o', ' ', ' ', '|'},
        {'|', ' ', 'T', 'r', 'e', 's', 'o', 'r', ' ', '|'},
        {'|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|'},
        {'|', '_', '_', '_', '_', '_', '_', '_', '_', '|'} };

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {

                printf("%c ", map[i][j]); // en gris

            }
            printf("\n"); 
        }
        //printf("\n"); 
}

// Création d'une fonction qui va appeler les deux fonctions précédentes pour créer une carte trésor de face et de dos
void carteTresor ()
{
    carteDeFace();
    carteDeDos();
}

void distribuerCartes() {
    char tresors[24] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'};
    int countCardJoueur1 = 0;
    int countCardJoueur2 = 0;
    int countCardJoueur3 = 0;
    int countCardJoueur4 = 0;
    //Attributes the cards to the players randomly depending on the number of players. Example : 4 players = 6 cards each, Append to the player's hand array the card's name (A, B, C, etc.)
    //each player is represented by struct pion and his hand is represented by pion.carte[12] (12 for max number of cards in player's hand)
    //parse the array of tresors and assign the cards to a random player, then remove the card from the array of tresors 
    //Take in count the number of players (if pion.estEnTrainDeJouer == true, he's playing)
    for (int i = 0; i < 24; i++) {
        srand(time(0));
        int k = rand() % 24;
        if (countCardJoueur1 < (24 / nbJoueurs)){
            pion1.cartes[countCardJoueur1] = tresors[k];
            countCardJoueur1++;
        } else if (countCardJoueur2 < (24 / nbJoueurs)) {
            pion2.cartes[countCardJoueur2] = tresors[k];
            countCardJoueur2++;
        } else if (countCardJoueur3 < (24 / nbJoueurs) && nbJoueurs == 3) {
            pion3.cartes[countCardJoueur3] = tresors[k];
            countCardJoueur3++;
        } else if (countCardJoueur4 < (24 / nbJoueurs) && nbJoueurs == 4) {
            pion4.cartes[countCardJoueur4] = tresors[k];
            countCardJoueur4++;
        }
        tresors[k] = ' ';
    }
}

// Fonction qui permet de mettre en place le déroulement de la partie
void initialisationPartie () 
{
    init_plateau();
    init_pions();
    distribuerCartes();
    Boucle();
}

// Fonction qui affiche les règles du jeu
void Regles()
{
    int choixRegles;
    int choixDeroulement;

    do
    {
    printf(" 1 - But du jeu\n");
    printf(" 2 - Préparation du jeu\n");
    printf(" 3 - Déroulement du jeu\n");
    printf(" 4 - Fin de la partie\n");
    printf(" Tapez 0 si vous voulez revenir au menu principal\n");
    printf("\n Votre choix : ");
    scanf("%d", &choixRegles);
    
    switch(choixRegles)
    {
        case 1:
            printf("\nBut du jeu\n");
            printf("\t Dans un labyrinthe enchanté, les joueurs partent à la chasse aux objets et aux créatures magiques.\n");
            printf("\t Chacun cherche à se frayer un chemin jusqu’à eux en faisant coulisser astucieusement les couloirs.\n");
            printf("\t Le premier joueur à découvrir tous ses secrets et à revenir à son point de départ remporte cette passionnante chasse aux trésors.\n\n");
            break;
        
        case 2:
            printf("\nPréparation du jeu\n");
            printf("\t 1 - Mélanger les plaques face cachée, puis les placer sur les emplacements libres du plateau pour créer un labyrinthe aléatoire. La plaque supplémentaire servira à faire coulisser les couloirs du labyrinthe.\n");
            printf("\t 2 - Mélanger à leur tour les 24 cartes Objectif face cachée.\n");
            printf("\t 3 - En distribuer le même nombre à chaque joueur. Chacun les empile devant lui sans les regarder.\n");
            printf("\t 4 - Chaque joueur choisit ensuite un pion qu’il place sur sa case Départ au coin du plateau de la couleur correspondante.\n\n");
            break;
        
        case 3:
           printf("\nDéroulement du jeu\n");
           printf("\t Chaque joueur regarde secrètement la carte supérieure de sa pile. Le plus jeune joueur commence.\n");
           printf("\t La partie se poursuit dans le sens des aiguilles d’une montre.\n");
           printf("\t A son tour de jeu, le joueur doit essayer d’atteindre la plaque représentant le même dessin que celui sur la carte supérieure de sa pile.\n");
           printf("\t Pour cela il commence toujours par faire coulisser une rangée ou une colonne du labyrinthe en insérant la plaque supplémentaire du bord vers l’intérieur du plateau, puis il déplace son pion.\n");
           printf("\t Ainsi, un tour se compose toujours de deux phases :\n\n");

           printf(" 1 - Modification des couloirs (Introduction de la carte couloir supplémentaire).\n");
           printf(" 2 - Déplacement du pion.\n");
           printf("\n Votre choix : ");
           scanf("%d", &choixDeroulement);
           
           if (choixDeroulement == 1)
           {
           printf("\n\n 1 - Modification des couloirs (Introduction de la carte couloir supplémentaire).\n");
           printf("\t Quand vient son tour, le joueur choisit l’une de ces rangées ou colonnes et pousse la plaque supplémentaire vers l’intérieur du plateau jusqu’à ce qu’une nouvelle plaque soit expulsée à l’opposé.\n");
           printf("\t La plaque expulsée reste au bord du plateau jusqu’à ce qu’elle soit réintroduite à un autre endroit par le joueur suivant.\n");
           printf("\t Un joueur est toujours obligé de modifier le labyrinthe avant de déplacer son pion, même s’il aurait pu atteindre le dessin recherché sans déplacer les couloirs.\n");
           printf("\t Si, en faisant coulisser les couloirs du labyrinthe, un joueur expulse son pion ou un pion adverse du plateau, il est alors replacé à l’opposé, sur la plaque qui vient d’être introduite.\n\n");
           }
           
           else if (choixDeroulement == 2)
           {
           printf("\n\n 2 - Déplacement du pion.\n");
           printf("\t Dès qu’il a modifié le labyrinthe, le joueur peut déplacer son pion. Il peut le déplacer aussi loin qu’il veut jusqu’à n’importe quelle plaque en suivant un couloir ininterrompu.\n");
           printf("\t Un joueur peut même s’arrêter sur une case déjà occupée.\n");
           printf("\t Si le joueur n’atteint pas le dessin recherché (= celui figurant sur la carte supérieure de sa pile), il peut déplacer son pion aussi loin qu’il veut de manière à être en bonne position pour le prochain tour.\n");
           printf("\t S’il atteint le dessin recherché, il retourne sa carte à côté de sa pile. Il peut immédiatement regarder secrètement la carte suivante de sa pile pour connaître son prochain objectif.\n\n");
           }

           else 
            {
            printf("Erreur");
            main();
            }
           break;
        
        case 4:
            printf("\nFin de partie\n");
            printf("\t La partie s’arrête dès qu’un joueur a atteint tous ses objectifs et qu’il est revenu à son point de départ.\n");
            printf("\t C’est lui qui a su se déplacer le mieux dans le labyrinthe et il remporte la partie !\n\n");
            break;
    
        default:
            printf("------------------------------------\n\n");
            main();
            break;
    }

    } while (choixRegles != 0);
    
    main();
}


// Fonction qui permet de quitter le jeu
void Quitter()
{
    printf("Merci d'avoir joué au Labyrinth !\n");
    exit(0);
}
  

// Menu principal du jeu
void menuPrincipal()
{
    int choix;
    printf(" 1 - Nouvelle Partie\n");
    printf(" 2 - Afficher les règles / crédits\n");
    printf(" 3 - Quitter\n");
    printf("\n Votre choix : ");
    scanf("%d", &choix);

    switch(choix)
    {
        case 1:
            printf("\nNouvelle Partie\n");
            initialisationPartie();
            break;
        
        case 2:
            printf("\nAfficher les règles / crédits\n");
            Regles();
            break;
        
        case 3:
            printf("\nQuitter\n");
            Quitter();
            break;
        
        default:
            printf("Erreur");
            break;
    }
} 

int main (){
    menuPrincipal();
    return 0; 
}
