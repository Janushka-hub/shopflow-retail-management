#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "produit.h"
#include "utils.h"

static void afficherProduit(const Produit *p)
{
  printf("%-11s %-24s %-15s %8.2f EUR  stock:%4d  seuil:%3d  %s\n", p->reference, p->designation, p->categorie, p->prixUnitaireHT, p->quantiteStock, p->seuilAlerte, (p->actif) ? ("ACTIF") : ("INACTIF"));
}

static void ajouter(Boutique *b)
{
  Produit p = {0};
  lireTexte("Reference : ", p.reference, sizeof(p.reference));
  if (chercherProduit(b, p.reference))
  {
    puts("Reference deja utilisee (meme inactive).");
    return;
  }
  lireTexte("Designation : ", p.designation, sizeof(p.designation));
  lireTexte("Categorie : ", p.categorie, sizeof(p.categorie));
  p.prixUnitaireHT = lirePrix("Prix HT : ", 0.01f);
  p.quantiteStock = lireEntier("Stock : ", 0, INT_MAX);
  p.seuilAlerte = lireEntier("Seuil d'alerte : ", 0, INT_MAX);
  p.actif = 1;
  if (!reserver((void **) (&b->produits), &b->capaciteProduits, b->nbProduits + 1, sizeof(Produit)))
  {
    return;
  }
  b->produits[b->nbProduits++] = p;
  puts("Produit ajoute.");
}

static void modifier(Boutique *b)
{
  char ref[12];
  lireTexte("Reference : ", ref, sizeof(ref));
  Produit *p = chercherProduit(b, ref);
  if ((!p) || (!p->actif))
  {
    puts("Produit actif introuvable.");
    return;
  }
  p->prixUnitaireHT = lirePrix("Nouveau prix HT : ", 0.01f);
  p->quantiteStock = lireEntier("Nouveau stock : ", 0, INT_MAX);
  p->seuilAlerte = lireEntier("Nouveau seuil : ", 0, INT_MAX);
  puts("Produit modifie.");
}

static void supprimer(Boutique *b)
{
  char ref[12];
  lireTexte("Reference : ", ref, sizeof(ref));
  Produit *p = chercherProduit(b, ref);
  if ((!p) || (!p->actif))
  {
    puts("Produit actif introuvable.");
    return;
  }
  for (int i = 0; i < b->nbClients; i++)
  {
    for (int j = 0; j < b->clients[i].nbCommandes; j++)
    {
      Commande *c = &b->clients[i].commandes[j];
      if (!strcmp(c->statut, "ANNULEE"))
      {
        continue;
      }
      for (int k = 0; k < c->nbLignes; k++)
      {
        if (!strcmp(c->lignes[k].refProduit, ref))
        {
          puts("Suppression refusee : produit utilise dans une commande non annulee.");
          return;
        }
      }

    }

  }

  p->actif = 0;
  puts("Produit desactive.");
}

static void catalogue(const Boutique *b)
{
  double total = 0;
  for (int i = 0; i < b->nbProduits; i++)
  {
    afficherProduit(&b->produits[i]);
    if (b->produits[i].actif)
    {
      total += b->produits[i].prixUnitaireHT * b->produits[i].quantiteStock;
    }
  }

  printf("Valeur du stock actif HT : %.2f EUR\n", total);
}

void alertesStock(const Boutique *b)
{
  int n = 0;
  for (int i = 0; i < b->nbProduits; i++)
  {
    if (b->produits[i].actif && (b->produits[i].quantiteStock <= b->produits[i].seuilAlerte))
    {
      afficherProduit(&b->produits[i]);
      n++;
    }
  }

  if (!n)
  {
    puts("Aucune alerte.");
  }
}

static void rechercher(Boutique *b)
{
  int choix = lireEntier("1. Reference exacte  2. Mot-cle : ", 1, 2);
  char s[60];
  lireTexte("Recherche : ", s, sizeof(s));
  int n = 0;
  for (int i = 0; i < b->nbProduits; i++)
  {
    Produit *p = &b->produits[i];
    if (((choix == 1) && (!strcmp(p->reference, s))) || ((choix == 2) && strstr(p->designation, s)))
    {
      afficherProduit(p);
      n++;
    }
  }

  if (!n)
  {
    puts("Aucun produit trouve.");
  }
}

static void rechercheAvancee(Boutique *b)
{
  char categorie[30];
  lireTexte("Categorie : ", categorie, sizeof(categorie));
  float max = lirePrix("Prix HT maximum : ", 0.01f);
  int n = 0;
  for (int i = 0; i < b->nbProduits; i++)
  {
    Produit *p = &b->produits[i];
    if ((p->actif && strstr(p->categorie, categorie)) && (p->prixUnitaireHT <= max))
    {
      afficherProduit(p);
      n++;
    }
  }

  if (!n)
  {
    puts("Aucun resultat.");
  }
}

void menuProduits(Boutique *b)
{
  int c;
  do
  {
    puts("\n--- PRODUITS ---\n1 Ajouter\n2 Modifier\n3 Supprimer\n4 Catalogue\n5 Rechercher\n6 Alertes stock\n7 Recherche avancee (bonus)\n0 Retour");
    c = lireEntier("Choix : ", 0, 7);
    switch (c)
    {
      case 1:
        ajouter(b);
        break;

      case 2:
        modifier(b);
        break;

      case 3:
        supprimer(b);
        break;

      case 4:
        catalogue(b);
        break;

      case 5:
        rechercher(b);
        break;

      case 6:
        alertesStock(b);
        break;

      case 7:
        rechercheAvancee(b);
        break;

      default:
        break;

    }

  }
  while (c);
}


