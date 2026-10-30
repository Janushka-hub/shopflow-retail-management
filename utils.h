#ifndef UTILS_H
#define UTILS_H

// Lire et sécuriser un entier
int lireEntier(const char *message);

// Lire et sécuriser un nombre décimal (float)
float lireFlottant(const char *message);

// Lire une chaîne de caractères en évitant les dépassements
void lireChaine(const char *message, char *chaine, int tailleMax);

// Vérifier si une date (jour, mois, année) est valide
int validerDate(int jour, int mois, int annee);

#endif