//
// Created by Rakuvarman Janushka on 23/10/2026.

#ifndef MINIPROJETSHOPFLOW_INITIALISATION_H
#define MINIPROJETSHOPFLOW_INITIALISATION_H

#include "types.h"

// Initialise la boutique, ses tableaux dynamiques, ses compteurs et gère les erreurs d'allocation
int initialiserBoutique(Boutique *b, const char *nomBoutique);
int redimensionnerProduits(Boutique *b);
int redimensionnerClients(Boutique *b);
void detruireBoutique(Boutique *b);

#endif
