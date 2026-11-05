#include <stdio.h>
#include <string.h>
#include "statistique.h"

void afficherStatistiques(Boutique *b)
{
    if (b == NULL)
    {
        printf("Erreur : boutique inexistante.\n");
        return;
    }
    int produitsActifs = 0;
    int nbCommandes = 0;
    float chiffreAffaires = 0.0f;

    printf("\n========== STATISTIQUES ==========\n");

    // 1. Produits actifs
    for (int i = 0; i < b->nbProduits; i++)
    {
        if (b->produits[i].actif == 1)
        {
            produitsActifs++;
        }
    }
    // 2. Commandes et Chiffre d'affaires (hors commandes ANNULEES)[cite: 6]
    for (int i = 0; i < b->nbClients; i++)
    {
        nbCommandes += b->clients[i].nbCommandes;
        for (int j = 0; j < b->clients[i].nbCommandes; j++)
        {
            Commande *cmd = &b->clients[i].commandes[j];
            if (strcmp(cmd->statut, "ANNULEE") != 0)
            {
                chiffreAffaires += cmd->montantTTC;
            }
        }
    }
    printf("Nombre total de produits : %d\n", b->nbProduits);
    printf("Produits actifs : %d\n", produitsActifs);
    printf("Nombre total de clients : %d\n", b->nbClients);
    printf("Nombre total de commandes : %d\n", nbCommandes);
    printf("Chiffre d'affaires (hors annulation) : %.2f EUR\n", chiffreAffaires);

    // 3. Quantités vendues par produit (hors commandes ANNULEES)[cite: 6]
    printf("\n===== QUANTITES VENDUES PAR PRODUIT =====\n");
    int maxVentes = 0;
    char produitPlusVendu[100] = "";

    for (int p = 0; p < b->nbProduits; p++)
    {
        int totalVendu = 0;
        for (int c = 0; c < b->nbClients; c++)
        {
            for (int co = 0; co < b->clients[c].nbCommandes; co++)
            {
                Commande *cmd = &b->clients[c].commandes[co];
                if (strcmp(cmd->statut, "ANNULEE") != 0)
                {
                    for (int l = 0; l < cmd->nbLignes; l++)
                    {
                        if (strcmp(cmd->lignes[l].refProduit, b->produits[p].reference) == 0)
                        {
                            totalVendu += cmd->lignes[l].quantite;
                        }
                    }
                }
            }
        }
        printf("%s : %d ventes\n", b->produits[p].designation, totalVendu);

        if (totalVendu > maxVentes)
        {
            maxVentes = totalVendu;
            strcpy(produitPlusVendu, b->produits[p].designation);
        }
    }
    printf("\nProduit le plus vendu : %s (%d ventes)\n", produitPlusVendu, maxVentes);

    // 4. Produits en rupture de stock
    printf("\n===== PRODUITS EN RUPTURE =====\n");
    int ruptureTrouvee = 0;
    for (int i = 0; i < b->nbProduits; i++)
    {
        if (b->produits[i].quantiteStock == 0)
        {
            printf("- %s\n", b->produits[i].designation);
            ruptureTrouvee = 1;
        }
    }
    if (!ruptureTrouvee) printf("Aucun produit en rupture.\n");

    // 5. Produits sous le seuil d'alerte[cite: 4]
    printf("\n===== PRODUITS SOUS LE SEUIL D'ALERTE =====\n");
    int alerteTrouvee = 0;
    for (int i = 0; i < b->nbProduits; i++)
    {
        Produit prod = b->produits[i];
        if (prod.quantiteStock > 0 && prod.quantiteStock <= prod.seuilAlerte)
        {
            printf("- %s (%d en stock, seuil : %d)\n", prod.designation, prod.quantiteStock, prod.seuilAlerte);
            alerteTrouvee = 1;
        }
    }
    if (!alerteTrouvee) printf("Aucun produit sous le seuil d'alerte.\n");
}
k