#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>  // Pour la fonction tolower
#include <conio.h> // Pour les fonctions kbhit() et getch()

#define BOARD_SIZE 8
#define B_CROISEUR 4
#define B_SOUS_MARIN 3
#define B_TORPILLEUR 2
#define C_EAU 0
#define C_EAU_T 1
#define C_CROISEUR 20
#define C_CROISEUR_T 21
#define C_SOUS_MARIN 30
#define C_SOUS_MARIN_T 31
#define C_TORPILLEUR 40
#define C_TORPILLEUR_T 41

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct {
    int croiseur, sousMarin, torpilleur, points;
    int grille[BOARD_SIZE][BOARD_SIZE];
    int grilleTir[BOARD_SIZE][BOARD_SIZE];
} Plateau;

typedef struct {
    int x, y;
    char d;
} Coordonnees;

void logo() {
	printf(ANSI_COLOR_GREEN);
	printf(" ____        _        _ _ _                               _      \n");
	printf("| __ )  __ _| |_ __ _(_) | | ___   _ __   __ ___   ____ _| | ___ \n");
	printf("|  _ \\ / _` | __/ _` | | | |/ _ \\ | '_ \\ / _` \\ \\ / / _` | |/ _ \\\n");
	printf("| |_) | (_| | || (_| | | | |  __/ | | | | (_| |\\ V / (_| | |  __/\n");
	printf("|____/ \\__,_|\\__\\__,_|_|_|_|\\___| |_| |_|\\__,_| \\_/ \\__,_|_|\\___|\n\n");
	printf(ANSI_COLOR_BLUE"        _     _     _ " ANSI_COLOR_RESET "__-=-//__  __\\\\-=-__" ANSI_COLOR_BLUE " _     _     _        \n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_BLUE".-.,.-'`(,.-'`(,.-'`(," ANSI_COLOR_RESET "\\_______/" ANSI_COLOR_BLUE ".." ANSI_COLOR_RESET "\\_______/" ANSI_COLOR_BLUE ",)`'-.,)`'-.,)`'-.,Ã‚Â¸.-.\n\n" ANSI_COLOR_RESET);
}

// Fonction pour convertir une chaÃƒÂ®ne de caractÃƒÂ¨res en coordonnÃƒÂ©es
Coordonnees strToCoord(char string[], int hasDirection) {
    Coordonnees c;
    char strX[3];
    int i;

    if (hasDirection > 0) {
        hasDirection = 1;
        c.d = string[strlen(string) - 1]; // Direction (h/v)
    } else {
        hasDirection = 0;
    }

    // Convertir 'A'-'H' en indice 0-7
    c.y = tolower(string[0]) - 'a';
    for (i = 0; i < strlen(string) - (1 + hasDirection); i++) {
        strX[i] = string[i + 1];
    }
    strX[i] = '\0';  
    // Convertir '1'-'8' en indice 0-7
    c.x = strtol(strX, NULL, 10) - 1;

    return c;
}

// Initialiser le plateau avec des valeurs par dÃƒÂ©faut
Plateau initPlateau() {
    Plateau p;
    int i, j;
    p.croiseur = B_CROISEUR;
    p.sousMarin = B_SOUS_MARIN;
    p.torpilleur = B_TORPILLEUR;
    p.points = 0;

    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            p.grille[i][j] = C_EAU;
            p.grilleTir[i][j] = C_EAU;
        }
    }

    return p;
}

// Fonction pour afficher la grille des bateaux et la grille des tirs
void afficherGrilles(int grilleBateaux[BOARD_SIZE][BOARD_SIZE], int grilleTirs[BOARD_SIZE][BOARD_SIZE], int showShips) {
    int i, j;
    
    // Affichage des titres
    printf("\nBateaux du joueur :\n");
    puts("  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |");
    puts("  +---+---+---+---+---+---+---+---+");
    
    // Affichage des bateaux du joueur
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%c |", i + 'A'); // Afficher A Ãƒ  H pour les lignes
        for (j = 0; j < BOARD_SIZE; j++) {
            if (grilleBateaux[i][j] == C_EAU) {
                printf("   |");  // Case d'eau vide
            } else if (grilleBateaux[i][j] == C_EAU_T) {
                printf("%s X %s|", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);  // Tir manquÃƒÂ© (eau touchÃƒÂ©e)
            } else if (grilleBateaux[i][j] == C_CROISEUR || grilleBateaux[i][j] == C_SOUS_MARIN || grilleBateaux[i][j] == C_TORPILLEUR) {
                printf("%s * %s|", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);  // Bateau non touchÃƒÂ©
            } else {
                printf("%s X %s|", ANSI_COLOR_RED, ANSI_COLOR_RESET);  // Bateau touchÃƒÂ©
            }
        }
        printf("\n  +---+---+---+---+---+---+---+---+\n");
    }

    // Affichage des tirs sur l'adversaire
    printf("\nTirs sur l'adversaire :\n");
    puts("  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |");
    puts("  +---+---+---+---+---+---+---+---+");
    
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%c |", i + 'A'); // Afficher A Ãƒ  H pour les lignes
        for (j = 0; j < BOARD_SIZE; j++) {
            if (grilleTirs[i][j] == C_EAU) {
                printf("   |");  // Case d'eau vide
            } else if (grilleTirs[i][j] == C_EAU_T) {
                printf("%s X %s|", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);  // Tir manquÃƒÂ© (eau touchÃƒÂ©e)
            } else if (grilleTirs[i][j] == C_CROISEUR_T || grilleTirs[i][j] == C_SOUS_MARIN_T || grilleTirs[i][j] == C_TORPILLEUR_T) {
                printf("%s X %s|", ANSI_COLOR_RED, ANSI_COLOR_RESET);  // Bateau touchÃƒÂ©
            } else {
                printf("   |");  // Case non rÃƒÂ©vÃƒÂ©lÃƒÂ©e (pas de tir)
            }
        }
        printf("\n  +---+---+---+---+---+---+---+---+\n");
    }
}

// Fonction pour placer un bateau sur le plateau (manuel)
Plateau placeShip(Plateau p, char* typeBateau, int taille, int idBateau) {
    Coordonnees c;
    char input[5];
    printf("Placez votre %s (%d cases). Entrez les coordonnÃƒÂ©es de dÃƒÂ©part (ex: a1) et la direction (h/v) : ", typeBateau, taille);
    scanf("%s", input);
    c = strToCoord(input, 1);  // RÃƒÂ©cupÃƒÂ¨re la direction aussi

    // Placement basique en fonction de la direction
    for (int i = 0; i < taille; i++) {
        if (c.d == 'h') {
            p.grille[c.y][c.x + i] = idBateau;
        } else if (c.d == 'v') {
            p.grille[c.y + i][c.x] = idBateau;
        }
    }
    
    // Afficher la grille aprÃƒÂ¨s le placement du bateau
    afficherGrilles(p.grille, p.grilleTir, 1);

    return p;
}

// Fonction pour placer un bateau automatiquement (alÃƒÂ©atoire)
Plateau placeShipAuto(Plateau p, int taille, int idBateau) {
    Coordonnees c;
    c.x = rand() % BOARD_SIZE;
    c.y = rand() % BOARD_SIZE;
    c.d = (rand() % 2 == 0) ? 'h' : 'v';  // Direction alÃƒÂ©atoire (horizontale ou verticale)

    // Placement basique en fonction de la direction
    for (int i = 0; i < taille; i++) {
        if (c.d == 'h' && (c.x + i) < BOARD_SIZE) {
            p.grille[c.y][c.x + i] = idBateau;
        } else if (c.d == 'v' && (c.y + i) < BOARD_SIZE) {
            p.grille[c.y + i][c.x] = idBateau;
        }
    }

    return p;
}

// Fonction pour tirer sur une case et mettre Ãƒ  jour la grille
Plateau tir(Plateau p, Coordonnees coord) {
    if (p.grille[coord.y][coord.x] >= C_CROISEUR && p.grille[coord.y][coord.x] <= C_TORPILLEUR) {
        printf("\n > TouchÃƒÂ© en %c%d !\n", coord.y + 'A', coord.x + 1);
        if (p.grille[coord.y][coord.x] == C_CROISEUR) {
            p.grille[coord.y][coord.x] = C_CROISEUR_T; // Croiseur touchÃƒÂ©
        } else if (p.grille[coord.y][coord.x] == C_SOUS_MARIN) {
            p.grille[coord.y][coord.x] = C_SOUS_MARIN_T; // Sous-Marin touchÃƒÂ©
        } else if (p.grille[coord.y][coord.x] == C_TORPILLEUR) {
            p.grille[coord.y][coord.x] = C_TORPILLEUR_T; // Torpilleur touchÃƒÂ©
        }
        p.points++; // Augmenter les points
        p.grilleTir[coord.y][coord.x] = C_CROISEUR_T; // Marquer sur la grille de tir
    } else {
        printf("\n > ManquÃƒÂ© en %c%d.\n", coord.y + 'A', coord.x + 1);
        p.grille[coord.y][coord.x] = C_EAU_T;
        p.grilleTir[coord.y][coord.x] = C_EAU_T;
    }
    return p;
}

void sauvegarderJeu(Plateau p1, Plateau p2) {
    FILE* file = fopen("sauvegarde.txt", "w");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier de sauvegarde.\n");
        return;
    }
    fprintf(file, "%d %d %d\n", p1.croiseur, p1.sousMarin, p1.torpilleur);
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            fprintf(file, "%d ", p1.grille[i][j]);
        }
        fprintf(file, "\n");
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            fprintf(file, "%d ", p1.grilleTir[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Jeu sauvegardÃƒÂ© avec succÃƒÂ¨s.\n");
}

// Fonction pour charger l'ÃƒÂ©tat du jeu
int chargerJeu(Plateau *p1, Plateau *p2) {
    FILE* file = fopen("sauvegarde.txt", "r");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier de sauvegarde.\n");
        return -1; // Indiquer une erreur
    }
    fscanf(file, "%d %d %d\n", &p1->croiseur, &p1->sousMarin, &p1->torpilleur);
    fscanf(file, "%d %d %d\n", &p2->croiseur, &p2->sousMarin, &p2->torpilleur);
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            fscanf(file, "%d ", &p1->grille[i][j]);
        }
        fscanf(file, "\n");
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            fscanf(file, "%d ", &p1->grilleTir[i][j]);
        }
        fscanf(file, "\n");
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            fscanf(file, "%d ", &p2->grille[i][j]);
        }
        fscanf(file, "\n");
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            fscanf(file, "%d ", &p2->grilleTir[i][j]);
        }
        fscanf(file, "\n");
    }
    fclose(file);
    printf("Jeu chargÃƒÂ© avec succÃƒÂ¨s.\n");
    return 0; // Indiquer le succÃƒÂ¨s
}

// Tir alÃƒÂ©atoire de l'ordinateur
Coordonnees tirOrdi(Plateau *p) {
    Coordonnees coord;
    coord.x = rand() % BOARD_SIZE;
    coord.y = rand() % BOARD_SIZE;
    return coord;
}

int main() {
     srand(time(NULL));  // Initialiser la graine pour les placements et tirs alÃƒÂ©atoires
    Plateau p1 = initPlateau();
    Plateau p2 = initPlateau();
    char choix;

    while (1) {
        logo();
        printf("\n=== Bataille Navale ===\n");
        printf("1. Jouer contre l'ordinateur\n");
        printf("2. Jouer contre un autre joueur\n");
        printf("C. Charger la partie\n");
        printf("S. Sauvegarder la partie\n"); // Option de sauvegarde
        printf("Votre choix : ");
        scanf(" %c", &choix);

        // Charger le jeu si 'C' ou 'c' est pressÃƒÂ©
        if (choix == 'C' || choix == 'c') {
            if (chargerJeu(&p1, &p2) == 0) {
               afficherGrilles(p1.grille, p2.grilleTir, 1); // Afficher les grilles chargÃƒÂ©es
    } else {
                printf("Erreur lors du chargement du jeu.\n");
            }
            continue; // Continuer Ãƒ  la prochaine itÃƒÂ©ration de la boucle
        }

        // Sauvegarder si 'S' ou 's' est pressÃƒÂ©
        if (choix == 'S' || choix == 's') {
            sauvegarderJeu(p1, p2); // Appeler la fonction de sauvegarde
            printf("Partie sauvegardÃƒÂ©e.\n");
            continue; // Continuer Ãƒ  la prochaine itÃƒÂ©ration de la boucle
        }

        if (choix == '1') {
            // Placement automatique des bateaux pour l'ordinateur (p2)
            p2 = placeShipAuto(p2, B_CROISEUR, C_CROISEUR);
            p2 = placeShipAuto(p2, B_SOUS_MARIN, C_SOUS_MARIN);
            p2 = placeShipAuto(p2, B_TORPILLEUR, C_TORPILLEUR);

            // Placement manuel pour le joueur (p1)
            p1 = placeShip(p1, "Croiseur", B_CROISEUR, C_CROISEUR);
            p1 = placeShip(p1, "Sous-Marin", B_SOUS_MARIN, C_SOUS_MARIN);
            p1 = placeShip(p1, "Torpilleur", B_TORPILLEUR, C_TORPILLEUR);

            // Boucle de jeu
            do {
                // VÃƒÂ©rification de la sauvegarde aprÃƒÂ¨s chaque tir
                char coordStr[5];
                afficherGrilles(p1.grille, p2.grilleTir, 0);
                printf("\nla croix rouge = bateau qui coule\nla croix bleu = cible manquÃƒÂ©\n");
                printf("\nEntrez une coordonnÃƒÂ©e pour tirer (ex: a1) ou 'S' pour sauvegarder : ");
                
                scanf("%s", coordStr);

                // VÃƒÂ©rifiez si le joueur veut sauvegarder
                if (coordStr[0] == 'S' || coordStr[0] == 's') {
                    sauvegarderJeu(p1, p2); // Appeler la fonction de sauvegarde
                    printf("Partie sauvegardÃƒÂ©e.\n");
                    continue; // Retour au dÃƒÂ©but de la boucle pour un nouveau tir
                }

                Coordonnees coord = strToCoord(coordStr, 0);
                p2 = tir(p2, coord);

                // Ordinateur tire
                coord = tirOrdi(&p1);
                printf("L'ordinateur tire en %c%d\n", coord.y + 'A', coord.x + 1);
                p1 = tir(p1, coord);

            } while (p1.points < 9 && p2.points < 9);

            printf("Jeu terminÃƒÂ© !\n");
        } else if (choix == '2') {
            // Mode deux joueurs : placement manuel pour les deux joueurs
            printf("\nJoueur 1, placez vos bateaux :\n");
            p1 = placeShip(p1, "Croiseur", B_CROISEUR, C_CROISEUR);
            p1 = placeShip(p1, "Sous-Marin", B_SOUS_MARIN, C_SOUS_MARIN);
            p1 = placeShip(p1, "Torpilleur", B_TORPILLEUR, C_TORPILLEUR);

            printf("\nJoueur 2, placez vos bateaux :\n");
            p2 = placeShip(p2, "Croiseur", B_CROISEUR, C_CROISEUR);
            p2 = placeShip(p2, "Sous-Marin", B_SOUS_MARIN, C_SOUS_MARIN);
            p2 = placeShip(p2, "Torpilleur", B_TORPILLEUR, C_TORPILLEUR);

            // Boucle de jeu
            do {
                // VÃƒÂ©rification de la sauvegarde aprÃƒÂ¨s chaque tir
                char coordStr[5];
                afficherGrilles(p1.grille, p2.grilleTir, 0);
                printf("\nla croix rouge = bateau qui coule\nla croix bleu = cible manquÃƒÂ©\n");
                printf("\nJoueur 1, entrez une coordonnÃƒÂ©e (ex: a1) pour tirer ou 'S' pour sauvegarder : ");
                
                scanf("%s", coordStr);

                // VÃƒÂ©rifiez si le joueur veut sauvegarder
                if (coordStr[0] == 'S' || coordStr[0] == 's') {
                    sauvegarderJeu(p1, p2); // Appeler la fonction de sauvegarde
                    printf("Partie sauvegardÃƒÂ©e.\n");
                    continue; // Retour au dÃƒÂ©but de la boucle pour un nouveau tir
                }

                Coordonnees coord = strToCoord(coordStr, 0);
                p2 = tir(p2, coord);

                // Joueur 2 tire
                afficherGrilles(p2.grille, p1.grilleTir, 0);
                printf("\nla croix rouge = bateau qui coule\nla croix bleu = cible manquÃƒÂ©\n");
                printf("\nJoueur 2, entrez une coordonnÃƒÂ©e (ex: a1) pour tirer ou 'S' pour sauvegarder : ");
                
                scanf("%s", coordStr);

                // VÃƒÂ©rifiez si le joueur veut sauvegarder
                if (coordStr[0] == 'S' || coordStr[0] == 's') {
                    sauvegarderJeu(p1, p2); // Appeler la fonction de sauvegarde
                    printf("Partie sauvegardÃƒÂ©e.\n");
                    continue; // Retour au dÃƒÂ©but de la boucle pour un nouveau tir
                }

                coord = strToCoord(coordStr, 0);
                p1 = tir(p1, coord);

            } while (p1.points < 9 && p2.points < 9);

            printf("Jeu terminÃƒÂ© !\n");
        }

        printf("Appuyez sur 'Q' ou 'q' pour quitter, ou toute autre touche pour continuer...\n");
        choix = getchar();  // Attendre une entrÃƒÂ©e de l'utilisateur
        getchar();  // Pour consommer le caractÃƒÂ¨re '\n' restant aprÃƒÂ¨s getchar

        if (choix == 'Q' || choix == 'q') {
            printf("Vous avez choisi de quitter le jeu.\n");
            break;  // Quitter la boucle et terminer le jeu
        }
    }

    return 0;
}