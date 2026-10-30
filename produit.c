#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produit.h"

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

    p.actif = 1;
    b->produits[b->nbProduits] = p;
    b->nbProduits++;

    printf("Produit ajoute avec succes !\n");
}
void modifierProduit(Boutique *b) {
    char ref[20];
    printf("Entrer la reference du produit a modifier : ");
    scanf("%s", ref);

    int index = -1;
    // 1. Rechercher le produit par sa reference et verifier s'il est actif
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, ref) == 0 && b->produits[i].actif == 1) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Erreur : Produit introuvable ou supprime !\n");
        return;
    }
    printf("Produit trouve ! Entrez les nouvelles informations :\n");
    printf("Nouvelle designation : ");
    getchar(); // Vider le buffer
    fgets(b->produits[index].designation, sizeof(b->produits[index].designation), stdin);
    b->produits[index].designation[strcspn(b->produits[index].designation, "\n")] = 0;

    printf("Nouvelle categorie : ");
    scanf("%s", b->produits[index].categorie);
    do {
        printf("Nouveau prix unitaire HT (>= 0) : ");
        scanf("%f", &b->produits[index].prixUnitaireHT);
        if (b->produits[index].prixUnitaireHT < 0) {
            printf("Erreur : Le prix ne peut pas etre negatif !\n");
        }
    } while (b->produits[index].prixUnitaireHT < 0);
    do {
        printf("Nouvelle quantite en stock (>= 0) : ");
        scanf("%d", &b->produits[index].quantiteStock);
        if (b->produits[index].quantiteStock < 0) {
            printf("Erreur : La quantite ne peut pas etre negative !\n");
        }
    } while (b->produits[index].quantiteStock < 0);
    do {
        printf("Nouveau seuil d'alerte (>= 0) : ");
        scanf("%d", &b->produits[index].seuilAlerte);
        if (b->produits[index].seuilAlerte < 0) {
            printf("Erreur : Le seuil d'alerte ne peut pas etre negatif !\n");
        }
    } while (b->produits[index].seuilAlerte < 0);
    printf("Produit modifie avec succes !\n");
}
void supprimerProduit(Boutique *b) {
    char ref[20];
    printf("Entrer la reference du produit a supprimer : ");
    scanf("%s", ref);
    int index = -1;
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, ref) == 0 && b->produits[i].actif == 1) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Erreur : Produit introuvable ou deja supprime !\n");
        return;
    }
    b->produits[index].actif = 0;
    printf("Produit supprime avec succes (suppression logique) !\n");
}
void rechercherProduit(const Boutique *b) {
    char ref[20];
    printf("Entrer la reference du produit a rechercher : ");
    scanf("%s", ref);
    int trouve = 0;

    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, ref) == 0 && b->produits[i].actif == 1) {
            printf("\n--- Details du produit trouve ---\n");
            printf("Reference : %s\n", b->produits[i].reference);
            printf("Designation : %s\n", b->produits[i].designation);
            printf("Categorie : %s\n", b->produits[i].categorie);
            printf("Prix unitaire HT : %.2f\n", b->produits[i].prixUnitaireHT);
            printf("Quantite en stock : %d\n", b->produits[i].quantiteStock);
            printf("Seuil d'alerte : %d\n", b->produits[i].seuilAlerte);
            trouve = 1;
            break;
        }
    }
    if (!trouve) {
        printf("Erreur : Produit introuvable ou supprime !\n");
    }
}

