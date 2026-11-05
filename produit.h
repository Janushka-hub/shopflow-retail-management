#ifndef PRODUIT_H
#define PRODUIT_H

#include "types.h"

void afficherCatalogue(const Boutique *b);

void ajouterProduit(Boutique *b);

void modifierProduit(Boutique *b);

void supprimerProduit(Boutique *b);

void rechercherProduit(const Boutique *b);

int verifierStock(const Boutique *b, int idProduit, int quantiteDemandee);

int mettreAJourStock(Boutique *b, int idProduit, int quantite, int estUneVente);

void verifierEtAfficherAlertesStock(const Boutique *b);

#endif // PRODUIT_H