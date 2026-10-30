#ifndef PRODUIT_H
#define PRODUIT_H

#include "types.h"

void afficherCatalogue(const Boutique *b);
void ajouterProduit(Boutique *b);
void modifierProduit(Boutique *b);
void supprimerProduit(Boutique *b);
void rechercherProduit(const Boutique *b);

#endif // PRODUIT_H
