#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produit.h"

// 3.1. Afficher le catalogue
void afficherCatalogue(const Boutique *b) {
    printf("\n=== CATALOGUE DES PRODUITS (%d / %d) ===\n", b->nbProduits, b->capaciteProduits);
    if (b->nbProduits == 0) {
        printf("Le catalogue est vide.\n");
        return;
    }
    for (int i = 0; i < b->nbProduits; i++) {
        Produit p = b->produits[i];
        printf("Ref: %s | Nom: %s | Cat: %s | Prix: %.2f EUR | Stock: %d (Seuil: %d) | Actif: %d\n",
               p.reference, p.designation, p.categorie, p.prixUnitaireHT,
               p.quantiteStock, p.seuilAlerte, p.actif);
    }
}

// 3.2. Ajouter un produit
void ajouterProduit(Boutique *b) {
    if (b->nbProduits >= b->capaciteProduits) {
        b->capaciteProduits *= 2;
        Produit *temp = realloc(b->produits, b->capaciteProduits * sizeof(Produit));
        if (temp == NULL) {
            printf("Erreur d'allocation memoire.\n");
            return;
        }
        b->produits = temp;
    }

    Produit p;
    printf("Entrer la reference du produit (ex: PRD-0001) : ");
    scanf("%s", p.reference);

    // Vérifier l'unicité de la référence
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, p.reference) == 0) {
            printf("Erreur : Ce produit existe deja !\n");
            return;
        }
    }

    printf("Entrer la designation : ");
    getchar(); // Vider le buffer
    fgets(p.designation, sizeof(p.designation), stdin);
    p.designation[strcspn(p.designation, "\n")] = 0;

    printf("Entrer la categorie : ");
    scanf("%s", p.categorie);

    printf("Entrer le prix unitaire HT : ");
    scanf("%f", &p.prixUnitaireHT);

    printf("Entrer la quantite en stock : ");
    scanf("%d", &p.quantiteStock);

    printf("Entrer le seuil d'alerte : ");
    scanf("%d", &p.seuilAlerte);

    p.actif = 1; // Actif par défaut

    b->produits[b->nbProduits] = p;
    b->nbProduits++;

    printf("Produit ajoute avec succes !\n");
}

// 3.3. Modifier un produit (laissé vide pour l'instant)
void modifierProduit(Boutique *b) {
}

// 3.4. Supprimer un produit (laissé vide pour l'instant)
void supprimerProduit(Boutique *b) {
}

// 3.5. Rechercher un produit (laissé vide pour l'instant)
void rechercherProduit(const Boutique *b) {
