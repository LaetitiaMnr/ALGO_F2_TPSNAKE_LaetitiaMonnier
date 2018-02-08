#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>

// chars representant la direction de la tete du serpent
#define SERP_TETE_H	'^'	/* haut */
#define SERP_TETE_B	'V'	/* bas */
#define SERP_TETE_G	'<'	/* gauche */
#define SERP_TETE_D	'>'	/* droite  */
#define SERP_CORP 'o'

//Directions
#define DIR_HAUT 72
#define DIR_BAS 80
#define DIR_DROITE 77
#define DIR_GAUCHE 75

// Taille
#define HAUTEUR 20
#define LARGEUR 30
#define TAILLE 200

enum ETATSERP
{
    estMort, seNourrit,
};

//Prototypes
void reglesDuJeu();
void creationMurs();
void avanceSerp(int*, int*, COORD [], COORD*, int*, int*);
enum ETATSERP etatDuSerpent(COORD[], COORD, int);
void affichagePerdu(int*);
COORD generationNourriture();

//BUT : Fonction principale contenant un menu dans lequel le joueur peut naviguer. Il peut jouer au snake, voir les règles du jeu ou quitter le programme.
//ENTREE : Le choix du joueur (1/2/3).
//SORTIE : En fonction du choix du joueur, le programme affichera des données différentes.
int main()
{
    srand(time(NULL));
    int arretJeu = 0;

    do {
        int choix = 0, vie = 1, nb = 4;
        int keyPress = 0, derniereTouche = 0;
        char rep = ' ';
        COORD corpSerpent[TAILLE] = {{10, 10}, {9, 10}, {8, 10}, {7, 10}};
        COORD fruit = {0,0};

        system("@cls||clear");
        printf("--- Bienvenu sur le jeu du Snake ---\n");
        printf("--- Menu ---\n");
        printf("1. Jouer :\n");
        printf("2. Regles du jeu :\n");
        printf("3. Quitter :\n");
        scanf("%d", &choix);

        switch (choix){
            case 1:
                system("@cls||clear");
                creationMurs();
                fruit = generationNourriture();

                do {
                    keyPress = _getch();
                    avanceSerp(&keyPress, &derniereTouche, corpSerpent, &fruit, &vie, &nb);
                } while (vie > 0);
                affichagePerdu(&arretJeu);
                break;

            case 2:
                system("@cls||clear");
                reglesDuJeu();
                printf("Retour menu ? O/N :\n");
                fflush(stdin);
                rep = getc(stdin);
                rep = tolower(rep);

                if (rep == 'n') {
                    printf("A bientot\n");
                    arretJeu = 1;
                }
                break;

            case 3:
                system("@cls||clear");
                printf("A bientot\n");
                arretJeu = 1;
                break;
        }
    } while (arretJeu == 0);

    system("pause");

    return 0;
}

//BUT : Fonction indiquant les règles du jeu consultable dans le menu par le joueur.
void reglesDuJeu()
{
    printf("- Utilisez les fleches afin de guider votre serpent\n");
    printf("- Nourrissez celui-ci afin qu'il soit le plus grand possible.\n");
    printf("- S'il touche le mur ou son propre corps le jeu se termine et vous avez perdu !\n");
}

//BUT : Fonction permetttant de créer les murs de la map.
//SORTIE : Un rectangle vide.
void creationMurs()
{
    int i = 0, j = 0;
    for(i = 0; i < HAUTEUR; i++){
        for(j = 0; j < LARGEUR; j++){
            if((i == 0) || (i == HAUTEUR - 1) || (j == 0) || (j == LARGEUR - 1))
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }
}

//BUT : Fonction permettant de faire avancer le serpent et de gérer les conséquences des mouvements (collisions, changements de direction, mange).
void avanceSerp(int *keyPress, int *derniereTouche, COORD corpSerpent[], COORD *fruit, int *vie, int *nb)
{
    char teteSerp = ' ';
    int i = 1, toucheValide = 0;

    //Tant que l'utilisateur n'a pas appuyé sur une autre touche, le serpent avance dans la même direction.
    while (!kbhit()){
        Sleep(200);

        //Permet d'effacer le corps.
        for (i = 0; i < *nb; i++) {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corpSerpent[i]);
            printf(" ");
        }

        //Permet de faire avancer tout le corps.
        for (i = *nb - 1; i > 0; i--) {
            corpSerpent[i].Y = corpSerpent[i - 1].Y;
            corpSerpent[i].X = corpSerpent[i - 1].X;

            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corpSerpent[i]);
            printf("%c", SERP_CORP);
        }

        toucheValide = 0;
        while (toucheValide == 0) {
            switch (*keyPress){
                case DIR_HAUT:
                    if (*derniereTouche != DIR_BAS) {
                        corpSerpent[0].Y -= 1;
                        teteSerp = SERP_TETE_H;
                        toucheValide = 1;
                    }
                    else
                        *keyPress = *derniereTouche;
                    break;

                case DIR_DROITE:
                    if (*derniereTouche != DIR_GAUCHE) {
                        corpSerpent[0].X += 1;
                        teteSerp = SERP_TETE_D;
                        toucheValide = 1;
                    }
                    else
                        *keyPress = *derniereTouche;
                    break;

                case DIR_GAUCHE:
                    if (*derniereTouche != DIR_DROITE) {
                        corpSerpent[0].X -= 1;
                        teteSerp = SERP_TETE_G;
                        toucheValide = 1;
                    }
                    else
                        *keyPress = *derniereTouche;
                    break;

                case DIR_BAS:
                    if (*derniereTouche != DIR_HAUT) {
                        corpSerpent[0].Y += 1;
                        teteSerp = SERP_TETE_B;
                        toucheValide = 1;
                    }
                    else
                        *keyPress = *derniereTouche;
                    break;
                default:
                    *keyPress = *derniereTouche;
            }
        }

        *derniereTouche = *keyPress;

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), corpSerpent[0]);
        printf("%c", teteSerp);

        //Appel de l'enum afin de tester les différents états du serpent.
        switch (etatDuSerpent(corpSerpent, *fruit, *nb)) {
            case seNourrit:
                *nb += 1;

                do {
                    *fruit = generationNourriture();
                } while ((fruit -> X == corpSerpent[i].X) && (fruit -> Y == corpSerpent[i].Y));
                break;
            case estMort:
                *vie -= 1;
                return; //Permet de sortir de la boucle while.
                break;
        }
    }
}

enum ETATSERP etatDuSerpent(COORD corpSerpent[], COORD fruit, int nb)
{
    int i = 0;

    for (i = 1; i < nb; i++){
        if ((corpSerpent[0].X == corpSerpent[i].X) && (corpSerpent[0].Y == corpSerpent[i].Y))
            return estMort;
    }

    if ((corpSerpent[0].X <= 0) || (corpSerpent[0].X >= LARGEUR - 1) || (corpSerpent[0].Y <= 0) || (corpSerpent[0].Y >= HAUTEUR - 1))
        return estMort;
    else if ((fruit.X == corpSerpent[0].X) && (fruit.Y == corpSerpent[0].Y))
        return seNourrit;
    else
        return -1;
}

//BUT : Fonction générant un fruit au hasard sur la map.
//SORTIE : Coordonnée d'un fruit puis son affichage.
COORD generationNourriture()
{
    COORD fruit;

    fruit.X = (rand() % (LARGEUR - 2)) + 1;
    fruit.Y = (rand() % (HAUTEUR - 2)) + 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), fruit);
    printf("O");

    return fruit;
}

//BUT : Fonction affichant que le joueur à perdu et demande s'il veut rejouer.
//ENTREE : 0ui ou non.
//SORTIE : Soit le joueur revient sur le menu,soit il quitte le programme.
void affichagePerdu(int *arretJeu)
{
    char rep = ' ';

    system("@cls||clear");
    printf("Vous etes mort !\n");
    printf("Voulez vous rejouer ? O/N\n");
    fflush(stdin);
    rep = getc(stdin);
    rep = tolower(rep);

    if (rep == 'n') {
        printf("A bientot\n");
        *arretJeu = 1;
    }
}
