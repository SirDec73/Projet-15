#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

//Fonction pour enlever les caracteres du buffer
void ClearBuffer()
{
    char c = getchar();;
    while (c != '\n') {
        c = getchar();
    }
}

// Fonction qui demande une entre entiere encadre par des bornes Min et Max
int AskInt(const char* text, int Value, int Bmin, int Bmax) {
    int iError = 0;

    //Tant que la valeur rentre n'est pas un entier ou qu'il n'est pas compris dans les borns
    while (iError == 0) {
        printf(text);

        // Verifier que l'entre est un int
        iError = scanf_s("%d", &Value);
        if (iError == 1) {
            //Si il est dans les bornes
            if (Value <= Bmax && Value >= Bmin) {
                printf("\n");
                return Value;
            }
            //Si il n'est pas dans les bornes
            else {
                printf("ERREUR Hors bornes \nEntree entre %d et %d : \n", Bmin, Bmax);
                iError = 0;
            }
        }
        // reset buffer du scanf_s
        ClearBuffer();
    }
}
// demande une chaine de caractere appartenant a une liste
int AskChar(const char* message, char* tableau) {
    int iError = 0;
    char replay = 'n';
    while (iError == 0) {

        // reset buffer du scanf_s
        ClearBuffer();
        printf(message);
        // Verifier que l'entre est un char
        iError = scanf_s("%c", &replay, 1);
        replay = toupper(replay);
        for (int i = 0; i < 2; i++) {
            if (replay == tableau[0]) {
                printf("Partie Relance !\n");
                return 1;
            }
            else if (replay == tableau[1]) {
                printf("Jeu Fini !\n");
                return 0;
            }
        }
        printf("ERREUR Entree invalide, Appuyer sur Entrer pour continuer\n");
        iError = 0;
    }
}
int main()
{
    //Initialisation des variables
    int play = 1;
    int partieGagne = 0;
    //Initialisation malloc pour sauvegarder les nombres trouves
    int* SaveNum = (int*)malloc(sizeof(int) * 1);
    if (SaveNum == NULL) {
        exit(1);
    }
    *SaveNum = 0;
    // Liste des entrees pour rejouer
    char list[2] = { 'Y','N' };
    //lancement de la partie
    while (play == 1) {
        //Initialisation des variables de jeu
        int AskNb = 0;
        int Try = 5;
        int Tryuse = 0;
        int Bmin = 0;
        Bmin = AskInt("Choisi la borne min : \n", Bmin, 1, 10000);
        int Bmax = 0;
        Bmax = AskInt("Choisi la borne max : \n", Bmax, Bmin, 10000);
        //Recuperation des nombres aleatoires
        srand(time(NULL));
        int Nb = rand() % (Bmax - Bmin + 1) + Bmin;
        printf("Le nombre random %d\n", Nb);

        //Tant qu'il y a des coups à jouer et que le nombre n'est pas encore decouvert
        while (AskNb != Nb && Tryuse < Try)
        {
            //Demande un nombre
            AskNb = AskInt("Choisi un nombre : ", AskNb, Bmin, Bmax);
            //Differentes possibilitées
            if (Nb < AskNb) {
                printf("Moins\n");
            }
            else if (Nb > AskNb) {
                printf("Plus\n");
            }
            else if (Nb == AskNb) {

                printf("Vous Avez GAGNE\n");

                partieGagne++;
                //Realloc SaveNum pour aggrandire sa taille
                SaveNum = (int*)realloc(SaveNum, sizeof(int) * (partieGagne+1));
                SaveNum[partieGagne - 1] = Nb;
                break;
            }
            Tryuse++;
            printf("Il vous reste %d \n", Try - Tryuse);
        }
        // Affichage de la defaite
        if (Try == Tryuse) {
            printf("Vous Avez PERDU\n");
        }
        // Demande pour rejouer
        printf("Appuyez sur entrer pour continuer\n");
        play = AskChar("Voulez-vous rejouer ? (y,n)\n", list);
    }
    // Affiche les anciens resultat trouves
    printf("Voici les anciens resultat trouve :\n");
    for (int i = 0; i < partieGagne; i++) {
        printf("-> %d\n", SaveNum[i]);
    }

}
/*
// Exo:
void functionExo3(float* tableau) {
    for (int i = 0; i < 10; i++) {
        tableau[i] = 5;
    }
}
void functionExo4(float* tableau) {
    for (int i = 0; i < 10; i++) {
        tableau[i] = 5;
    }
}

int functionExo5(int Value, int Bmin, int Bmax) {
    int iError = 0;
    while (iError == 0) {
        if (scanf_s("%d", &Value)) {
            if (Value <= Bmax && Value >= Bmin) {
                return Value;
            }
        }
        char c = getchar();
        while (c != '\n') { c = getchar(); }
    }
}
int functionExo6(int Value, int Bmin, int Bmax) {
    int iError = 0;
    while (iError == 0) {
        if (scanf_s("%d", &Value)) {
            if (Value <= Bmax && Value >= Bmin) {
                return Value;
            }
        }
        char c = getchar();
        while (c != '\n') { c = getchar(); }
    }
}


int main() {
    // Exo:1
    printf("Exercice 1");
    float f1 = 5;
    float* pf = &f1;
    *pf = 10;
    float f2;
    pf = &f2;
    *pf = 20;
    printf("%f\n", f1);
    printf("%f\n", f2);

    // Exo:2
    printf("\nExercice2\n");
    // A
    float* pf1 = (float*)malloc(sizeof(float));
    if (pf1 == NULL) {
        exit(1);
    }
    // B
    *pf1 = 5;
    // C
    float* pf2 = (float*)malloc(sizeof(float));
    if (pf2 == NULL) {
        exit(1);
    }
    // D
    float* pf3;
    pf3 = pf2;
    pf2 = pf1;
    pf1 = pf3;

    printf("pf2 : %f\n", *pf2);
    printf("pf1 : %f\n", *pf1);
    //E
    free(pf2);
    free(pf1);
    // Exo:3
    printf("\nExercice3\n");
    float* tf = (float*)malloc(sizeof(float) * 10);
    if (tf == NULL) {
        exit(1);
    }
    for (int i = 0; i < 10; i++) {
        tf[i] = 0;
    }
    functionExo3(tf);
    for (int i = 0; i < 10; i++) {
        printf("%d | %f\n", i, tf[i]);
    }
    free(tf);
    // Exo:4
    printf("\nExercice4\n");
    float tff[10];
    for (int i = 0; i < 10; i++) {
        tff[i] = 0;
    }
    functionExo4(tff);
    for (int i = 0; i < 10; i++) {
        printf("%d | %f\n", i, tff[i]);
    }
    // Exo:5

    printf("\nExercice5\n");
    int Size = 0;
    printf("Choisi une taille de tableau : ");
    scanf_s("%d", &Size);
    int* tfff = (int*)malloc(sizeof(int) * Size);
    if (tfff == NULL) {
        exit(1);
    }

    for (int i = 0; i < Size; i++) {
        printf("Choisi un nombre entre 1 et 10000 pour la case n°%d : ", i + 1);
        tfff[i] = functionExo5(tfff[i], 1, 10000);
    }
    for (int i = 0; i < Size; i++) {
        printf("[%d] => %d\n", i, tfff[i]);
    }
    // Exo:6
    printf("\nExercice6\n");
    int i = 0;
    int* tableau6 = (int*)malloc(sizeof(int));
    if (tableau6 == NULL) {
        exit(1);
    }

    while (1) {
        printf("[%d] => ", i);
        tableau6[i] = functionExo5(tableau6[i], -1, 10000);
        printf("|%d|\n", tableau6[i]);
        if (tableau6[i] == -1) {
            break;
        }
        i++;
        tableau6 = (int*)realloc(tableau6, sizeof(int) * (i + 1));
    }
    for (int k = 0; k < i; k++) {
        printf("[%d] => %d\n", k, tableau6[k]);
    }
    free(tableau6);
}*/;