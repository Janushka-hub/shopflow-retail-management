#ifndef PRODUIT_H
#define PRODUIT_H

#include "types.h"

// 1. Afficher le catalogue des produits
void afficherCatalogue(const Boutique *b);

// 2. Ajouter un produit
void ajouterProduit(Boutique *b);

// 3. Modifier un produit
void modifierProduit(Boutique *b);

// 4. Supprimer un produit
void supprimerProduit(Boutique *b);

// 5. Rechercher un produit
void rechercherProduit(const Boutique *b);

#endif // PRODUIT_H
