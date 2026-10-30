#ifndef MINIPROJETSHOPFLOW_COMMANDE_H
#define MINIPROJETSHOPFLOW_COMMANDE_H

#include "types.h"

void creerCommande(Boutique *b);
void ajouterProduitCommande(Boutique *b);
void calculerMontantCommande(Boutique *b);
void afficherCommande(Boutique *b);
void changerStatutCommande(Boutique *b);
void retirerProduitCommande(Boutique *b);
#endif // MINIPROJETSHOPFLOW_COMMANDE_H