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

    printf("Boutique '%s' initialisee avec succes en memoire !\n", b->nom);
    return 1;
}

// Fonction pour libérer toute la mémoire
void detruireBoutique(Boutique *b) {
    if (b == NULL) return;

    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            free(b->clients[i].commandes[j].lignes);
        }
        free(b->clients[i].commandes);
    }

    free(b->clients);
    free(b->produits);

    b->clients = NULL;
    b->produits = NULL;
    b->nbProduits = 0;
    b->nbClients = 0;
    b->capaciteProduits = 0;
    b->capaciteClients = 0;

    printf("Memoire de la boutique entierement liberee.\n");
}