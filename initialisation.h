#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "initialisation.h"

// Fonction d'initialisation de la boutique (retourne int pour valider le succès)
int initialiserBoutique(Boutique *b, const char *nomBoutique) {
    if (b == NULL) return 0;

    // 1. Copier le nom de la boutique
    strncpy(b->nom, nomBoutique, sizeof(b->nom) - 1);
    b->nom[sizeof(b->nom) - 1] = '\0';

    // 2. Initialiser les compteurs
    b->nbProduits = 0;
    b->nbClients = 0;
    b->numeroSuivant = 1001;
    b->idClientSuivant = 1;

    // 3. Définir les capacités initiales
    b->capaciteProduits = 10;
    b->capaciteClients = 8;

    // 4. Allouer dynamiquement la mémoire pour les produits
    b->produits = (Produit *)malloc(b->capaciteProduits * sizeof(Produit));
    if (b->produits == NULL) {
        printf("Erreur fatale : echec de l'allocation memoire pour les produits !\n");
        return 0;
    }

    // 5. Allouer dynamiquement la mémoire pour les clients
    b->clients = (Client *)malloc(b->capaciteClients * sizeof(Client));
    if (b->clients == NULL) {
        printf("Erreur fatale : echec de l'allocation memoire pour les clients !\n");
        free(b->produits);
        return 0;
    }

