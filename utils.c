#include <stdio.h>
#include <string.h>
#include "utils.h"

// Lire un entier proprement avec gestion des erreurs
int lireEntier(const char *message) {
    int valeur;
    while (1) {
        printf("%s", message);
        if (scanf("%d", &valeur) == 1) {
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
// Vérification simple et rigoureuse d'une date (incluant les années bissextiles)
int validerDate(int jour, int mois, int annee) {
    // 1. Vérifier l'année (par exemple entre 2000 et 2100)
    if (annee < 2000 || annee > 2100) return 0;

    // 2. Vérifier le mois
    if (mois < 1 || mois > 12) return 0;

    // 3. Définir le nombre de jours par mois
    int joursParMois[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Vérification de l'année bissextile pour février
    if ((annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0)) {
        joursParMois[2] = 29;
    }

    // 4. Vérifier le jour
    if (jour < 1 || jour > joursParMois[mois]) return 0;

    return 1; // La date est valide
}