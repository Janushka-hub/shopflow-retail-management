#include <stdio.h>
#include <string.h>
#include "utils.h"

// Lire un entier proprement avec gestion des erreurs
int lireEntier(const char *message) {
    int valeur;
    while (1) {
        printf("%s", message);
        if (scanf("%d", &valeur) == 1) {
            // Vider le buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return valeur;
        }
        printf("Erreur : Entree invalide. Veuillez entrer un entier.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}
// Lire un nombre décimal proprement avec gestion des erreurs
float lireFlottant(const char *message) {
    float valeur;
    while (1) {
        printf("%s", message);
        if (scanf("%f", &valeur) == 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return valeur;
        }
        printf("Erreur : Entree invalide. Veuillez entrer un nombre.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}
// Lire une chaîne de caractères en toute sécurité (anti-dépassement)
void lireChaine(const char *message, char *chaine, int tailleMax) {
    printf("%s", message);
    if (fgets(chaine, tailleMax, stdin) != NULL) {
        chaine[strcspn(chaine, "\n")] = '\0';
    }
}