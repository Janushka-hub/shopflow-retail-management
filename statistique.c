#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statistique.h"
#include "utils.h"

static int valable(const Commande *c)
{
  return strcmp(c->statut, "ANNULEE") != 0;
}

static void chiffreAffaires(const Boutique *b, int mois, int annee)
{
  double ht = 0;
  double ttc = 0;
  int n = 0;
  for (int i = 0; i < b->nbClients; i++)
  {
    for (int j = 0; j < b->clients[i].nbCommandes; j++)
    {
      const Commande *c = &b->clients[i].commandes[j];
      if (((!valable(c)) || (annee && (c->dateCommande.annee != annee))) || (mois && (c->dateCommande.mois != mois)))
      {
        continue;
      }
      ht += c->montantHT;
      ttc += c->montantTTC;
      n++;
    }

  }

  printf("Commandes non annulees : %d | CA HT : %.2f EUR | CA TTC : %.2f EUR\n", n, ht, ttc);
}

static void topProduits(const Boutique *b)
{
  int *q = calloc((size_t) b->nbProduits, sizeof(int));
  int *ordre = malloc(((size_t) b->nbProduits) * (sizeof(int)));
  if ((!q) || (!ordre))
  {
    puts("Memoire insuffisante.");
    free(q);
    free(ordre);
    return;
  }
  for (int i = 0; i < b->nbProduits; i++)
  {
    ordre[i] = i;
  }

  for (int i = 0; i < b->nbClients; i++)
  {
    for (int j = 0; j < b->clients[i].nbCommandes; j++)
    {
      const Commande *c = &b->clients[i].commandes[j];
      if (!valable(c))
      {
        continue;
      }
      for (int k = 0; k < c->nbLignes; k++)
      {
        for (int p = 0; p < b->nbProduits; p++)
        {
          if (!strcmp(c->lignes[k].refProduit, b->produits[p].reference))
          {
            q[p] += c->lignes[k].quantite;
          }
        }

      }

    }

  }

  for (int i = 0; i < b->nbProduits; i++)
  {
    for (int j = i + 1; j < b->nbProduits; j++)
    {
      if (q[ordre[j]] > q[ordre[i]])
      {
        int t = ordre[i];
        ordre[i] = ordre[j];
        ordre[j] = t;
      }
    }

  }

  puts("TOP 5 PRODUITS (quantites vendues)");
  for (int i = 0; (i < b->nbProduits) && (i < 5); i++)
  {
    printf("%d. %s (%s) : %d unites\n", i + 1, b->produits[ordre[i]].designation, b->produits[ordre[i]].reference, q[ordre[i]]);
  }

  free(q);
  free(ordre);
}

static void topClients(const Boutique *b)
{
  double *ca = calloc((size_t) b->nbClients, sizeof(double));
  int *ordre = malloc(((size_t) b->nbClients) * (sizeof(int)));
  if ((!ca) || (!ordre))
  {
    puts("Memoire insuffisante.");
    free(ca);
    free(ordre);
    return;
  }
  for (int i = 0; i < b->nbClients; i++)
  {
    ordre[i] = i;
    for (int j = 0; j < b->clients[i].nbCommandes; j++)
    {
      const Commande *c = &b->clients[i].commandes[j];
      if (valable(c))
      {
        ca[i] += c->montantTTC;
      }
    }

  }

  for (int i = 0; i < b->nbClients; i++)
  {
    for (int j = i + 1; j < b->nbClients; j++)
    {
      if (ca[ordre[j]] > ca[ordre[i]])
      {
        int t = ordre[i];
        ordre[i] = ordre[j];
        ordre[j] = t;
      }
    }

  }

  puts("CLASSEMENT CLIENTS (CA TTC)");
  for (int i = 0; i < b->nbClients; i++)
  {
    const Client *c = &b->clients[ordre[i]];
    printf("%d. %s %s (ID %d) : %.2f EUR\n", i + 1, c->prenom, c->nom, c->idClient, ca[ordre[i]]);
  }

  free(ca);
  free(ordre);
}

static void categories(const Boutique *b)
{
  puts("STOCK PAR CATEGORIE");
  for (int i = 0; i < b->nbProduits; i++)
  {
    const Produit *p = &b->produits[i];
    if (!p->actif)
    {
      continue;
    }
    int deja = 0;
    for (int j = 0; j < i; j++)
    {
      if (b->produits[j].actif && (!strcmp(b->produits[j].categorie, p->categorie)))
      {
        deja = 1;
      }
    }

    if (deja)
    {
      continue;
    }
    int quantite = 0;
    double valeur = 0;
    for (int j = 0; j < b->nbProduits; j++)
    {
      if (b->produits[j].actif && (!strcmp(b->produits[j].categorie, p->categorie)))
      {
        quantite += b->produits[j].quantiteStock;
        valeur += b->produits[j].quantiteStock * b->produits[j].prixUnitaireHT;
      }
    }

    printf("%-20s %d unites, valeur HT %.2f EUR\n", p->categorie, quantite, valeur);
  }

}

void menuStatistiques(const Boutique *b)
{
  int choix;
  do
  {
    puts("\n--- STATISTIQUES ---\n1 CA total\n2 CA par mois\n3 CA par annee\n4 Top 5 produits\n5 Classement clients\n6 Stock par categorie (bonus)\n0 Retour");
    choix = lireEntier("Choix : ", 0, 6);
    switch (choix)
    {
      case 1:
        chiffreAffaires(b, 0, 0);
        break;

      case 2:
      {
        int an = lireEntier("Annee : ", 2000, 2100);
        int m = lireEntier("Mois : ", 1, 12);
        chiffreAffaires(b, m, an);
        break;
      }

      case 3:
        chiffreAffaires(b, 0, lireEntier("Annee : ", 2000, 2100));
        break;

      case 4:
        topProduits(b);
        break;

      case 5:
        topClients(b);
        break;

      case 6:
        categories(b);
        break;

      default:
        break;

    }

  }
  while (choix);
}


