#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "client.h"
#include "utils.h"

static double caClient(const Client *c)
{
  double total = 0;
  for (int j = 0; j < c->nbCommandes; j++)
  {
    if (strcmp(c->commandes[j].statut, "ANNULEE"))
    {
      total += c->commandes[j].montantTTC;
    }
  }

  return total;
}

static int emailDisponible(Boutique *b, const char *email, int idIgnore)
{
  for (int i = 0; i < b->nbClients; i++)
  {
    if ((b->clients[i].idClient != idIgnore) && (!strcmp(b->clients[i].email, email)))
    {
      return 0;
    }
  }

  return 1;
}

static void saisirEmail(Boutique *b, char *email, int idIgnore)
{
  do
  {
    lireTexte("Email : ", email, 80);
    if (emailValide(email) && emailDisponible(b, email, idIgnore))
    {
      return;
    }
    puts("Email invalide ou deja utilise.");
  }
  while (1);
}

static void saisirTelephone(char *telephone)
{
  do
  {
    lireTexte("Telephone (10 chiffres) : ", telephone, 15);
    if (telephoneValide(telephone))
    {
      return;
    }
    puts("Telephone invalide.");
  }
  while (1);
}

static void ajouter(Boutique *b)
{
  if (b->idClientSuivant == INT_MAX)
  {
    puts("Limite d'identifiants atteinte.");
    return;
  }
  Client c = {0};
  lireTexte("Nom : ", c.nom, sizeof(c.nom));
  lireTexte("Prenom : ", c.prenom, sizeof(c.prenom));
  saisirEmail(b, c.email, -1);
  saisirTelephone(c.telephone);
  lireAdresse(&c.adresse);
  if (!reserver((void **) (&b->clients), &b->capaciteClients, b->nbClients + 1, sizeof(Client)))
  {
    return;
  }
  c.idClient = b->idClientSuivant++;
  c.actif = 1;
  b->clients[b->nbClients++] = c;
  printf("Client ajoute : ID %d\n", c.idClient);
}

static void modifier(Boutique *b)
{
  int id = lireEntier("ID client : ", 1, INT_MAX);
  Client *c = chercherClient(b, id);
  if ((!c) || (!c->actif))
  {
    puts("Client actif introuvable.");
    return;
  }
  lireTexte("Nouveau nom : ", c->nom, sizeof(c->nom));
  lireTexte("Nouveau prenom : ", c->prenom, sizeof(c->prenom));
  saisirEmail(b, c->email, id);
  saisirTelephone(c->telephone);
  lireAdresse(&c->adresse);
  puts("Client modifie. Les adresses des anciennes commandes restent inchangees.");
}

static void supprimer(Boutique *b)
{
  int id = lireEntier("ID client : ", 1, INT_MAX);
  Client *c = chercherClient(b, id);
  if ((!c) || (!c->actif))
  {
    puts("Client actif introuvable.");
    return;
  }
  for (int i = 0; i < c->nbCommandes; i++)
  {
    if (strcmp(c->commandes[i].statut, "ANNULEE"))
    {
      puts("Suppression refusee : commande non annulee existante.");
      return;
    }
  }

  c->actif = 0;
  puts("Client desactive.");
}

void afficherFicheClient(const Client *c)
{
  printf("\nClient %d : %s %s [%s]\nEmail : %s  Tel : %s\nAdresse : ", c->idClient, c->prenom, c->nom, (c->actif) ? ("ACTIF") : ("INACTIF"), c->email, c->telephone);
  afficherAdresse(c->adresse);
  printf("\nPoints : %d  Commandes : %d  CA TTC : %.2f EUR\n", c->pointsFidelite, c->nbCommandes, caClient(c));
  for (int i = 0; i < c->nbCommandes; i++)
  {
    const Commande *o = &c->commandes[i];
    printf("  Commande %d | %02d/%02d/%04d | %s | %.2f EUR TTC | %d ligne(s)\n", o->numero, o->dateCommande.jour, o->dateCommande.mois, o->dateCommande.annee, o->statut, o->montantTTC, o->nbLignes);
    for (int j = 0; j < o->nbLignes; j++)
    {
      printf("    %s x%d : %.2f EUR HT\n", o->lignes[j].designation, o->lignes[j].quantite, o->lignes[j].sousTotalHT);
    }

  }

}

static void lister(Boutique *b)
{
  for (int i = 0; i < b->nbClients; i++)
  {
    Client *c = &b->clients[i];
    printf("%4d %-16s %-16s %s | %d commandes | CA TTC %.2f EUR | %s\n", c->idClient, c->nom, c->prenom, c->email, c->nbCommandes, caClient(c), (c->actif) ? ("ACTIF") : ("INACTIF"));
  }

}

void menuClients(Boutique *b)
{
  int choix;
  do
  {
    puts("\n--- CLIENTS ---\n1 Ajouter\n2 Modifier\n3 Supprimer\n4 Liste\n5 Fiche et historique\n0 Retour");
    choix = lireEntier("Choix : ", 0, 5);
    switch (choix)
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
        lister(b);
        break;

      case 5:
      {
        int id = lireEntier("ID client : ", 1, INT_MAX);
        Client *c = chercherClient(b, id);
        if (c)
        {
          afficherFicheClient(c);
        }
        else
        {
          puts("Introuvable.");
        }
        break;
      }

      default:
        break;

    }

  }
  while (choix);
}


