#ifndef COMMANDE_H
#define COMMANDE_H
#include "types.h"
void calculCommande(Commande *c);
int ajouterLigne(Boutique *b, Commande *c, const char *reference, int quantite);
int supprimerLigne(Boutique *b, Commande *c, const char *reference);
void annulerCommande(Boutique *b, Commande *c, Client *client);
void afficherCommande(const Commande *c);
void menuCommandes(Boutique *b);
#endif
