#ifndef UTILS_H
#define UTILS_H
#include "types.h"
void lireTexte(const char *message, char *texte, int taille);
int lireEntier(const char *message, int minimum, int maximum);
float lirePrix(const char *message, float minimum);
int dateValide(Date d);
Date lireDate(void);
void lireAdresse(Adresse *a);
void afficherAdresse(Adresse a);
int emailValide(const char *email);
int telephoneValide(const char *telephone);
int reserver(void **tableau, int *capacite, int nombre, unsigned long taille);
Produit *chercherProduit(Boutique *b, const char *reference);
Client *chercherClient(Boutique *b, int id);
Commande *chercherCommande(Boutique *b, int numero, Client **proprietaire);
#endif
