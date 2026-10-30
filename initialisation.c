#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "initialisation.h"

// Fonction d'initialisation de la boutique
void initialiserBoutique(Boutique *b, const char *nomBoutique) {
    // 1. Copier le nom de la boutique
    strncpy(b->nom, nomBoutique, sizeof(b->nom) - 1);
    b->nom[sizeof(b->nom) - 1] = '\0';

    // 2. Initialiser les compteurs à 0
    b->nbProduits = 0;
    b->nbClients = 0;
    b->numeroSuivant = 1001;    // Les numéros de commande commenceront à 1001[cite: 4]
    b->idClientSuivant = 1;      // Les identifiants clients commenceront à 1[cite: 4]

    // 3. Définir les capacités initiales des tableaux (ex: de quoi stocker 10 produits et 8 clients au début)
    b->capaciteProduits = 10;
    b->capaciteClients = 8;

    // 4. Allouer dynamiquement la mémoire pour les produits avec malloc (CM6)
    // On demande de la place pour 10 structures Produit d'un coup[cite: 7, 51]
    b->produits = (Produit *)malloc(b->capaciteProduits * sizeof(Produit));

    // Vérification indispensable : est-ce que l'ordinateur avait assez de mémoire ?[cite: 7, 50, 51]
    if (b->produits == NULL) {
        printf("Erreur fatale : échec de l'allocation mémoire pour les produits !\n");
        exit(1); // On arrête le programme si la mémoire refuse de s'allouer
    }

    // 5. Allouer dynamiquement la mémoire pour les clients avec malloc (CM6)
    b->clients = (Client *)malloc(b->capaciteClients * sizeof(Client));

    // Vérification de l'allocation pour les clients[cite: 7, 50, 51]
    if (b->clients == NULL) {
        printf("Erreur fatale : échec de l'allocation mémoire pour les clients !\n");
        free(b->produits); // On nettoie les produits avant de quitter
        exit(1);
    }

    printf("Boutique '%s' initialisée avec succès en mémoire !\n", b->nom);
}

// Fonction pour libérer toute la mémoire (Anti-fuites exigé par les profs !)
void detruireBoutique(Boutique *b) {
    if (b == NULL) return;

    // On parcourt les clients pour libérer ce qui a été alloué à l'intérieur d'eux (leurs commandes et lignes)[cite: 7]
    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            free(b->clients[i].commandes[j].lignes); // Libère les lignes de chaque commande[cite: 7]
        }
        free(b->clients[i].commandes); // Libère l'historique des commandes du client[cite: 7]
    }

    // On libère les deux grands tableaux principaux de la boutique[cite: 7]
    free(b->clients);
    free(b->produits);

    // Par sécurité, on remet les pointeurs à NULL (comme vu en CM6)[cite: 56]
    b->clients = NULL;
    b->produits = NULL;
    b->nbProduits = 0;
    b->nbClients = 0;
    b->capaciteProduits = 0;
    b->capaciteClients = 0;

    printf("Mémoire de la boutique entièrement libérée.\n");
}=