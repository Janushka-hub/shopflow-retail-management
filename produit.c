#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>
#include "produit.h"

void afficherCatalogue(const Boutique *b) {

    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }
    if (b->nbProduits == 0) {
        printf("Le catalogue est vide.\n");
        return;
    }
    printf("\n===== CATALOGUE DES PRODUITS =====\n");
    for (int i = 0; i < b->nbProduits; i++) {
        const Produit *p = &b->produits[i];
        if (p->actif == 1) {
            printf("\nReference : %s\n", p->reference);
            printf("Designation : %s\n", p->designation);
            printf("Categorie : %s\n", p->categorie);
            printf("Prix HT : %.2f EUR\n", p->prixUnitaireHT);
            printf("Stock : %d\n", p->quantiteStock);
            printf("Seuil d'alerte : %d\n", p->seuilAlerte);
            printf("---------------------------\n");
        }
    }
}

// Lecture securisee d'une chaine de caracteres
static void lireChaineProduit(const char *message, char *destination, size_t taille) {
    char buffer[256];

    while (1) {
        printf("%s", message);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\nErreur de lecture.\n");
            exit(EXIT_FAILURE);
        }

        size_t longueur = strlen(buffer);

        if (longueur > 0 && buffer[longueur - 1] == '\n') {
            buffer[longueur - 1] = '\0';
            longueur--;
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            printf("Erreur : saisie trop longue.\n");
            continue;
        }

        if (longueur == 0 || longueur >= taille) {
            printf("Erreur : texte vide ou trop long (maximum %zu caracteres).\n",
                   taille - 1);
            continue;
        }

        strcpy(destination, buffer);
        return;
    }
}


// Lecture securisee d'un entier positif ou nul
static int lireEntierProduit(const char *message) {
    char buffer[128];

    while (1) {
        printf("%s", message);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\nErreur de lecture.\n");
            exit(EXIT_FAILURE);
        }

        if (strchr(buffer, '\n') == NULL) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            printf("Erreur : saisie trop longue.\n");
            continue;
        }

        char *fin;
        errno = 0;
        long valeur = strtol(buffer, &fin, 10);

        while (isspace((unsigned char)*fin)) {
            fin++;
        }

        if (errno == ERANGE || fin == buffer || *fin != '\0' ||
            valeur < 0 || valeur > INT_MAX) {
            printf("Erreur : entrez un entier positif ou nul.\n");
            continue;
        }

        return (int)valeur;
    }
}


// Lecture securisee d'un prix strictement positif
static float lirePrixProduit(const char *message) {
    char buffer[128];

    while (1) {
        printf("%s", message);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\nErreur de lecture.\n");
            exit(EXIT_FAILURE);
        }

        if (strchr(buffer, '\n') == NULL) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            printf("Erreur : saisie trop longue.\n");
            continue;
        }

        char *fin;
        errno = 0;
        float valeur = strtof(buffer, &fin);

        while (isspace((unsigned char)*fin)) {
            fin++;
        }

        if (errno == ERANGE || fin == buffer || *fin != '\0' ||
            !isfinite(valeur) || valeur <= 0) {
            printf("Erreur : entrez un prix strictement positif.\n");
            continue;
        }

        return valeur;
    }
}


// Ajouter un nouveau produit au catalogue
void ajouterProduit(Boutique *b) {

    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }

    if (b->nbProduits < 0 || b->capaciteProduits < 0 ||
        b->nbProduits > b->capaciteProduits ||
        (b->capaciteProduits > 0 && b->produits == NULL)) {
        printf("Erreur : catalogue invalide.\n");
        return;
    }

    Produit nouveau = {0};

    printf("\n===== AJOUTER UN PRODUIT =====\n");

    // 1. Reference unique
    while (1) {
        lireChaineProduit("Reference : ",
                         nouveau.reference,
                         sizeof(nouveau.reference));

        int existe = 0;

        for (int i = 0; i < b->nbProduits; i++) {
            if (strcmp(b->produits[i].reference, nouveau.reference) == 0) {
                existe = 1;
                break;
            }
        }

        if (existe) {
            printf("Erreur : cette reference existe deja.\n");
        } else {
            break;
        }
    }

    // 2. Informations du produit
    lireChaineProduit("Designation : ",
                     nouveau.designation,
                     sizeof(nouveau.designation));

    lireChaineProduit("Categorie : ",
                     nouveau.categorie,
                     sizeof(nouveau.categorie));

    nouveau.prixUnitaireHT = lirePrixProduit("Prix unitaire HT : ");

    nouveau.quantiteStock = lireEntierProduit("Quantite en stock : ");

    nouveau.seuilAlerte = lireEntierProduit("Seuil d'alerte : ");

    nouveau.actif = 1;

    // 3. Agrandir le tableau si necessaire
    if (b->nbProduits == b->capaciteProduits) {

        size_t nouvelleCapacite =
            b->capaciteProduits == 0
                ? 4u
                : (size_t)b->capaciteProduits * 2u;

        if (nouvelleCapacite > INT_MAX ||
            nouvelleCapacite > SIZE_MAX / sizeof(Produit)) {
            printf("Erreur : capacite maximale atteinte.\n");
            return;
        }

        Produit *temp = realloc(
            b->produits,
            nouvelleCapacite * sizeof(Produit)
        );

        if (temp == NULL) {
            printf("Erreur : allocation memoire impossible.\n");
            return;
        }

        b->produits = temp;
        b->capaciteProduits = (int)nouvelleCapacite;
    }

    // 4. Ajouter le produit
    b->produits[b->nbProduits] = nouveau;
    b->nbProduits++;

    printf("\nProduit ajoute avec succes !\n");
}