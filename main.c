#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

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
        char c = 'a';
        while (c != '\n') {
            c = getchar();
        }

    }
}
// demande une chaine de caractere appartenant a une liste
int AskChar(const char* message, char* tableau) {
    int iError = 0;
    char replay = 'n';
    while (iError == 0) {

        // reset buffer du scanf_s
        char c = 'a';
        while (c != '\n') {
            c = getchar();
        }

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
    int* SaveNum = (int*) malloc(sizeof(int) * 1);
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
        Bmin = AskInt("Choisi la borne min : \n", Bmin,1,10000);
        int Bmax = 0;
        Bmax = AskInt("Choisi la borne max : \n", Bmax, Bmin,10000);
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
                //Creation d'un stockage pour deplacer la valeur dans un SaveNum plus grand
                int* stockage = (int*)malloc(sizeof(int) * (partieGagne+1));
                for (int i = 0; i < partieGagne; i++) {
                    stockage[i] = SaveNum[i];
                }
                // On Libere l'espace de SaveNum ...
                free(SaveNum);
                // ... pour y mettre le stockage qui aura en plus récupéré les anciennes valeur mais aussi augmenter la taille de 1
                SaveNum = stockage;
                // Puis on enlève le contenu du stockage pour les prochaines fois
                stockage = NULL;
                // Et pour finir on assignation le nombre aléatoire a une adresse -1 pour que cela colle avec les indexs
                SaveNum[partieGagne-1] = Nb;
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