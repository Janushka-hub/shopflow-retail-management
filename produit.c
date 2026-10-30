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

// 3.3. Modifier un produit
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

    // 2. Verifier que le produit existe
    if (index == -1) {
        printf("Erreur : Produit introuvable ou supprime !\n");
        return;
    }

    printf("Produit trouve ! Entrez les nouvelles informations :\n");

    // 3. Modifier la designation
    printf("Nouvelle designation : ");
    getchar(); // Vider le buffer
    fgets(b->produits[index].designation, sizeof(b->produits[index].designation), stdin);
    b->produits[index].designation[strcspn(b->produits[index].designation, "\n")] = 0;

    // Modifier la categorie
    printf("Nouvelle categorie : ");
    scanf("%s", b->produits[index].categorie);

    // 4. Modifier et verifier le prix (>= 0)
    do {
        printf("Nouveau prix unitaire HT (>= 0) : ");
        scanf("%f", &b->produits[index].prixUnitaireHT);
        if (b->produits[index].prixUnitaireHT < 0) {
            printf("Erreur : Le prix ne peut pas etre negatif !\n");
        }
    } while (b->produits[index].prixUnitaireHT < 0);

    // 5. Modifier et verifier la quantite (>= 0)
    do {
        printf("Nouvelle quantite en stock (>= 0) : ");
        scanf("%d", &b->produits[index].quantiteStock);
        if (b->produits[index].quantiteStock < 0) {
            printf("Erreur : La quantite ne peut pas etre negative !\n");
        }
    } while (b->produits[index].quantiteStock < 0);

    // 6. Modifier et verifier le seuil d'alerte (>= 0)
    do {
        printf("Nouveau seuil d'alerte (>= 0) : ");
        scanf("%d", &b->produits[index].seuilAlerte);
        if (b->produits[index].seuilAlerte < 0) {
            printf("Erreur : Le seuil d'alerte ne peut pas etre negatif !\n");
        }
    } while (b->produits[index].seuilAlerte < 0);

    printf("Produit modifie avec succes !\n");
}

// 3.4. Supprimer un produit (suppression logique)
void supprimerProduit(Boutique *b) {
    char ref[20];
    printf("Entrer la reference du produit a supprimer : ");
    scanf("%s", ref);

    int index = -1;
    // 1. Rechercher le produit par sa reference et verifier s'il est actif
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, ref) == 0 && b->produits[i].actif == 1) {
            index = i;
            break;
        }
    }

    // 2. Verifier que le produit existe
    if (index == -1) {
        printf("Erreur : Produit introuvable ou deja supprime !\n");
        return;
    }

    // 3. Effectuer la suppression logique en modifiant le statut actif
    b->produits[index].actif = 0;

    printf("Produit supprime avec succes (suppression logique) !\n");
}

// 3.5. Rechercher un produit
void rechercherProduit(const Boutique *b) {
    char ref[20];
    printf("Entrer la reference du produit a rechercher : ");
    scanf("%s", ref);

    int trouve = 0;
    // 1. Rechercher le produit par sa reference et verifier s'il est actif
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

    // 2. Si le produit n'existe pas ou est supprime
    if (!trouve) {
        printf("Erreur : Produit introuvable ou supprime !\n");
    }
}

