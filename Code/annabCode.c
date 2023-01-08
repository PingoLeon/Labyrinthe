#include <stdio.h>
#include <stdlib.h>
#include <time.h>




int carteDeFace(void) {

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

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {



        printf("\033[1;37m%c\033[0m ", map[i][j]);// pour afficher la carte en blanc
        //\033[1;37mTexte en blanc\033[0m     blanc



        if ( i==5 && j==4)
        {

        srand(time(0));
        int k = rand() % 24;


        printf("\033[33m%c\033[33m ", tresors[k]);// pour afficher le trésor en jaune
        //\033[33mTexte en jaune\033[33m       jaune
        }
        

    }
    printf("\n");
}
}


int carteDeDos (void){
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

            printf("\033[0;37m%c\033[0m ", map[i][j]); // en gris

            }
            printf("\n"); 
            }
            //printf("\n"); 

}





int casesTableau(void){


    char caseL [3][3]= {
        {'#', ' ', '#'},
        {'#', ' ', ' '},
        {'#', '#', '#'},
    };
    // 6L avec trésor
    // 4L avec les positions des pions
    // 6L sans trésor


    char caseT [3][3]= {
        {'#', ' ', '#'},
        {' ', ' ', ' '},
        {'#', '#', '#'},
    };
    // 6T avec trésor

    char caseI [3][3]= {
        {'#', ' ', '#'},
        {'#', ' ', '#'},
        {'#', ' ', '#'},
    };
    // 12I sans trésor





    for (int i = 0; i < 3; i++) {
    for (int k = 0; k < 6; k++) { // devrait faire 6 fois les cartes avec les trésors
    for (int j = 0; j < 3; j++) {

        if ( i==2 && j==2){


            //afficher les 6 premières lettres pour la case L
            //printf("\033[33mT\033[33m ", caseL[i][j]);

        }
    


        printf("\033[1;37m%c\033[0m ", caseL[i][j]);// pour afficher la carte en blanc

}
printf("  ");
}
printf("\n");
}
printf("\n");

    for (int i = 0; i < 3; i++) {
    for (int k = 0; k < 6; k++) { // devrait faire 6 fois les cartes avec les trésors
    for (int j = 0; j < 3; j++) {

        if ( i==2 && j==2){


            //afficher les 6 lettres suivantes pour case T
            //printf("\033[33mT\033[33m ", caseT[i][j]);

        }
    
        printf("\033[1;37m%c\033[0m ", caseT[i][j]);

}
printf("  ");
}
printf("\n");
}
printf("\n");
    


    for (int i = 0; i < 3; i++) {
    for (int k = 0; k < 10; k++) { // multiplier 10 fois le carte
    for (int j = 0; j < 3; j++) {

        //printf("\033[1;37m%c\033[0m ", caseL[i][j]);// pour afficher la carte en blanc


//printf("  ");  
}
printf("\n");
}
//printf("\n");
}


    for (int i = 0; i < 3; i++) {
    for (int k = 0; k < 12; k++) { // multiplier 12 fois la carte
    for (int j = 0; j < 3; j++) {


        printf("\033[1;37m%c\033[0m ", caseI[i][j]);// pour afficher la carte en blanc

        }
        printf("  ");
        }
        printf("\n");
        }

}


//  lancer le jeu
//v     combien de joueur
//?     noms des joueurs
//v     placer les joueurs
//v     appeler l'initialisation du tab
//v     afficher le tab
//      cartes trésors données aléatoirement 
//      joueur choisi alatoirement pour commencer 

//  gérération du lab
//      Afficher le tab (16 tuiles fixes, 34 autres)

//  déroulement d'un tour de jeu 
//      glisser rangée
//      déplacer pion si on le souhaite 



int main(void)
{
    //faire une diff entre la carte en face et de dos
    //
    carteDeFace();
    carteDeDos(); 

    //casesTableau();
    return 0;
}