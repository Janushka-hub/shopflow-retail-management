//
// Created by Rakuvarman Janushka on 23/10/2026.

#ifndef MINIPROJETSHOPFLOW_INITIALISATION_H
#define MINIPROJETSHOPFLOW_INITIALISATION_H

#include "types.h"

// Initialise la boutique, ses tableaux dynamiques, ses compteurs et gère les erreurs d'allocation
int initialiserBoutique(Boutique *b, const char *nomBoutique);

// Permet d'agrandir dynamiquement la capacité du tableau de produits (realloc)
int redimensionnerProduits(Boutique *b);

// Permet d'agrandir dynamiquement la capacité du tableau de clients (realloc)
int redimensionnerClients(Boutique *b);

// Libère proprement toute la mémoire de la boutique en cascade (free)
void detruireBoutique(Boutique *b);

#endif // MINIPROJETSHOPFLOW_INITIALISATION_H