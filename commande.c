#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "commande.h"
#include "utils.h"

void calculCommande(Commande *c)
{
  c->montantBrutHT = 0;
  for (int i = 0; i < c->nbLignes; i++)
  {
    c->lignes[i].sousTotalHT = c->lignes[i].prixUnitaireHT * c->lignes[i].quantite;
    c->montantBrutHT += c->lignes[i].sousTotalHT;
  }

  float brut = c->montantBrutHT;
  if (brut >= 600) {
    c->tauxRemise = 15;
  } else if (brut >= 300) {
    c->tauxRemise = 10;
  } else if (brut >= 100) {
    c->tauxRemise = 5;
  } else {
    c->tauxRemise = 0;
  }
  c->montantRemise = (brut * c->tauxRemise) / 100;
  c->montantHT = brut - c->montantRemise;
  c->montantTVA = c->montantHT * 0.20f;
  if (brut >= 60) {
    c->fraisPort = 0;
  } else {
    c->fraisPort = 5.90f;
  }
  c->montantTTC = (c->montantHT + c->montantTVA) + c->fraisPort;
}

static int points(const Commande *c)
{
  return (int) ((c->montantTTC / 10.0f) + 0.00001f);
}

static void recalculerPoints(Client *client)
{
  int total = 0;
  for (int i = 0; i < client->nbCommandes; i++)
  {
    if (strcmp(client->commandes[i].statut, "ANNULEE"))
    {
      total += points(&client->commandes[i]);
    }
  }

  client->pointsFidelite = total;
}

int ajouterLigne(Boutique *b, Commande *c, const char *reference, int quantite)
{
  if (strcmp(c->statut, "EN_COURS") || (quantite <= 0))
  {
    puts("Modification interdite.");
    return 0;
  }
  Produit *p = chercherProduit(b, reference);
  if ((!p) || (!p->actif))
  {
    puts("Produit actif introuvable.");
    return 0;
  }
  if (quantite > p->quantiteStock)
  {
    puts("Stock insuffisant.");
    return 0;
  }
  for (int i = 0; i < c->nbLignes; i++)
  {
    if (!strcmp(c->lignes[i].refProduit, reference))
    {
      puts("Produit deja present : supprimer la ligne avant de la ressaisir.");
      return 0;
    }
  }

  if (!reserver((void **) (&c->lignes), &c->capaciteLignes, c->nbLignes + 1, sizeof(LigneCommande)))
  {
    return 0;
  }
  LigneCommande *l = &c->lignes[c->nbLignes++];
  strcpy(l->refProduit, p->reference);
  strcpy(l->designation, p->designation);
  l->quantite = quantite;
  l->prixUnitaireHT = p->prixUnitaireHT;
  p->quantiteStock -= quantite;
  calculCommande(c);
  return 1;
}

int supprimerLigne(Boutique *b, Commande *c, const char *reference)
{
  if (strcmp(c->statut, "EN_COURS"))
  {
    puts("Modification interdite.");
    return 0;
  }
  if (c->nbLignes <= 1)
  {
    puts("Une commande doit conserver au moins une ligne. Annulez-la si necessaire.");
    return 0;
  }
  for (int i = 0; i < c->nbLignes; i++)
  {
    if (!strcmp(c->lignes[i].refProduit, reference))
    {
      Produit *p = chercherProduit(b, reference);
      if (p)
      {
        p->quantiteStock += c->lignes[i].quantite;
      }
      for (int j = i; j < (c->nbLignes - 1); j++)
      {
        c->lignes[j] = c->lignes[j + 1];
      }

      c->nbLignes--;
      calculCommande(c);
      return 1;
    }
  }

  puts("Ligne introuvable.");
  return 0;
}

void annulerCommande(Boutique *b, Commande *c, Client *client)
{
  if (strcmp(c->statut, "EN_COURS"))
  {
    puts("Annulation possible uniquement en cours.");
    return;
  }
  for (int i = 0; i < c->nbLignes; i++)
  {
    Produit *p = chercherProduit(b, c->lignes[i].refProduit);
    if (p)
    {
      p->quantiteStock += c->lignes[i].quantite;
    }
  }

  strcpy(c->statut, "ANNULEE");
  recalculerPoints(client);
  puts("Commande annulee. Stock et points mis a jour.");
}

void afficherCommande(const Commande *c)
{
  printf("\nCommande %d | Client %d | %02d/%02d/%04d | %s\n", c->numero, c->idClient, c->dateCommande.jour, c->dateCommande.mois, c->dateCommande.annee, c->statut);
  printf("Livraison : ");
  afficherAdresse(c->adresseLivraison);
  puts("");
  for (int i = 0; i < c->nbLignes; i++)
  {
    const LigneCommande *l = &c->lignes[i];
    printf("  %-12s %-24s x%d a %.2f = %.2f HT\n", l->refProduit, l->designation, l->quantite, l->prixUnitaireHT, l->sousTotalHT);
  }

  printf("Brut HT %.2f | Remise %.0f%% : %.2f | Net HT %.2f\nTVA %.2f | Port %.2f | TTC %.2f EUR\n", c->montantBrutHT, c->tauxRemise, c->montantRemise, c->montantHT, c->montantTVA, c->fraisPort, c->montantTTC);
}

static void creer(Boutique *b)
{
  if (b->numeroSuivant == INT_MAX)
  {
    puts("Limite de numeros atteinte.");
    return;
  }
  int id = lireEntier("ID client : ", 1, INT_MAX);
  Client *client = chercherClient(b, id);
  if ((!client) || (!client->actif))
  {
    puts("Client actif introuvable.");
    return;
  }
  Commande c = {0};
  c.idClient = id;
  c.dateCommande = lireDate();
  strcpy(c.statut, "EN_COURS");
  c.adresseLivraison = client->adresse;
  int n = lireEntier("Nombre de lignes (1 a 50) : ", 1, 50);
  for (int i = 0; i < n;)
  {
    char ref[12];
    lireTexte("Reference produit : ", ref, sizeof(ref));
    int q = lireEntier("Quantite : ", 1, INT_MAX);
    if (ajouterLigne(b, &c, ref, q))
    {
      i++;
    }
  }

  if (!reserver((void **) (&client->commandes), &client->capaciteCommandes, client->nbCommandes + 1, sizeof(Commande)))
  {
    for (int i = 0; i < c.nbLignes; i++)
    {
      Produit *p = chercherProduit(b, c.lignes[i].refProduit);
      if (p)
      {
        p->quantiteStock += c.lignes[i].quantite;
      }
    }

    free(c.lignes);
    return;
  }
  c.numero = b->numeroSuivant++;
  client->commandes[client->nbCommandes++] = c;
  recalculerPoints(client);
  printf("Commande creee : %d\n", c.numero);
  afficherCommande(&c);
}

static void modifierLigne(Boutique *b, int ajout)
{
  Client *client = NULL;
  int numero = lireEntier("Numero commande : ", 1, INT_MAX);
  Commande *c = chercherCommande(b, numero, &client);
  if (!c)
  {
    puts("Commande introuvable.");
    return;
  }
  char ref[12];
  lireTexte("Reference produit : ", ref, sizeof(ref));
  int ok;
  if (ajout)
  {
    int q = lireEntier("Quantite : ", 1, INT_MAX);
    ok = ajouterLigne(b, c, ref, q);
  }
  else
  {
    ok = supprimerLigne(b, c, ref);
  }
  if (ok)
  {
    recalculerPoints(client);
    puts("Commande mise a jour.");
    afficherCommande(c);
  }
}

static void statut(Boutique *b)
{
  int numero = lireEntier("Numero commande : ", 1, INT_MAX);
  Commande *c = chercherCommande(b, numero, NULL);
  if (!c)
  {
    puts("Commande introuvable.");
    return;
  }
  if (!strcmp(c->statut, "EN_COURS"))
  {
    strcpy(c->statut, "EXPEDIEE");
  }
  else
    if (!strcmp(c->statut, "EXPEDIEE"))
  {
    strcpy(c->statut, "LIVREE");
  }
  else
  {
    puts("Aucune transition autorisee.");
    return;
  }
  printf("Nouveau statut : %s\n", c->statut);
}

static void annuler(Boutique *b)
{
  Client *client = NULL;
  int numero = lireEntier("Numero commande : ", 1, INT_MAX);
  Commande *c = chercherCommande(b, numero, &client);
  if (c)
  {
    annulerCommande(b, c, client);
  }
  else
  {
    puts("Commande introuvable.");
  }
}

void menuCommandes(Boutique *b)
{
  int choix;
  do
  {
    puts("\n--- COMMANDES ---\n1 Creer\n2 Ajouter ligne\n3 Supprimer ligne\n4 Statut suivant\n5 Annuler\n6 Afficher\n0 Retour");
    choix = lireEntier("Choix : ", 0, 6);
    switch (choix)
    {
      case 1:
        creer(b);
        break;

      case 2:
        modifierLigne(b, 1);
        break;

      case 3:
        modifierLigne(b, 0);
        break;

      case 4:
        statut(b);
        break;

      case 5:
        annuler(b);
        break;

      case 6:
      {
        int numero = lireEntier("Numero commande : ", 1, INT_MAX);
        Commande *c = chercherCommande(b, numero, NULL);
        if (c)
        {
          afficherCommande(c);
        }
        else
        {
          puts("Commande introuvable.");
        }
        break;
      }

      default:
        break;

    }

  }
  while (choix);
}


