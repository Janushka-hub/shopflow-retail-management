#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "initialisation.h"
#include "commande.h"
#include "utils.h"

void detruireBoutique(Boutique *b)
{
  for (int i = 0; i < b->nbClients; i++)
  {
    for (int j = 0; j < b->clients[i].nbCommandes; j++)
    {
      free(b->clients[i].commandes[j].lignes);
    }

    free(b->clients[i].commandes);
  }

  free(b->clients);
  free(b->produits);
  b->clients = NULL;
  b->produits = NULL;
  b->nbClients = (b->nbProduits = 0);
  b->capaciteClients = (b->capaciteProduits = 0);
}

static int commandeDemo(Boutique *b, int id, Date date, const char *statut, const char *refs[], const int quantites[], int n)
{
  Client *cl = chercherClient(b, id);
  if ((!cl) || (!reserver((void **) (&cl->commandes), &cl->capaciteCommandes, cl->nbCommandes + 1, sizeof(Commande))))
  {
    return 0;
  }
  Commande c = {0};
  c.numero = b->numeroSuivant;
  c.idClient = id;
  c.dateCommande = date;
  c.adresseLivraison = cl->adresse;
  strcpy(c.statut, "EN_COURS");
  for (int i = 0; i < n; i++)
  {
    if (!ajouterLigne(b, &c, refs[i], quantites[i]))
    {
      for (int j = 0; j < c.nbLignes; j++)
      {
        Produit *p = chercherProduit(b, c.lignes[j].refProduit);
        if (p)
        {
          p->quantiteStock += c.lignes[j].quantite;
        }
      }

      free(c.lignes);
      return 0;
    }
  }

  strcpy(c.statut, statut);
  cl->commandes[cl->nbCommandes++] = c;
  b->numeroSuivant++;
  if (strcmp(statut, "ANNULEE"))
  {
    cl->pointsFidelite += (int) ((c.montantTTC / 10.0f) + 0.00001f);
  }
  else
  {
    for (int i = 0; i < c.nbLignes; i++)
    {
      Produit *p = chercherProduit(b, c.lignes[i].refProduit);
      if (p)
      {
        p->quantiteStock += c.lignes[i].quantite;
      }
    }

  }
  return 1;
}

void initialiserDonneesTest(Boutique *b)
{
  memset(b, 0, sizeof(*b));
  strcpy(b->nom, "ShopFlow");
  b->numeroSuivant = 1001;
  b->idClientSuivant = 1;
  Produit produits[] = {{"PRD-0001", "Clavier mecanique", "Informatique", 79.90f, 42, 10, 1},
    {"PRD-0002", "Souris sans fil", "Informatique", 24.50f, 8, 15, 1},
    {"PRD-0003", "Ecran 27 pouces", "Informatique", 199.00f, 16, 5, 1},
    {"PRD-0004", "Casque audio", "Audio", 89.90f, 25, 10, 1},
    {"PRD-0005", "Enceinte Bluetooth", "Audio", 59.90f, 12, 8, 1},
    {"PRD-0006", "Cable HDMI 2m", "Accessoires", 9.90f, 120, 30, 1},
    {"PRD-0007", "Hub USB-C", "Accessoires", 49.90f, 4, 10, 1},
    {"PRD-0008", "Disque SSD 1To", "Stockage", 109.00f, 18, 6, 1}};
  if (!reserver((void **) (&b->produits), &b->capaciteProduits, 8, sizeof(Produit)))
  {
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < 8; i++)
  {
    b->produits[b->nbProduits++] = produits[i];
  }

  Client clients[] = {{.nom = "Bernard", .prenom = "Julie", .email = "julie.bernard@mail.fr", .telephone = "0612345678", .adresse = {"12 rue de la Paix", "Paris", "75002", "France"}},
    {.nom = "Martin", .prenom = "Paul", .email = "paul.martin@mail.fr", .telephone = "0623456789", .adresse = {"8 rue Victor Hugo", "Lyon", "69002", "France"}},
    {.nom = "Petit", .prenom = "Emma", .email = "emma.petit@mail.fr", .telephone = "0634567890", .adresse = {"3 rue Nationale", "Lille", "59000", "France"}},
    {.nom = "Dubois", .prenom = "Lucas", .email = "lucas.dubois@mail.fr", .telephone = "0645678901", .adresse = {"15 rue du Port", "Nantes", "44000", "France"}},
    {.nom = "Moreau", .prenom = "Lea", .email = "lea.moreau@mail.fr", .telephone = "0656789012", .adresse = {"22 rue des Fleurs", "Bordeaux", "33000", "France"}}};
  if (!reserver((void **) (&b->clients), &b->capaciteClients, 5, sizeof(Client)))
  {
    detruireBoutique(b);
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < 5; i++)
  {
    clients[i].idClient = b->idClientSuivant++;
    clients[i].actif = 1;
    b->clients[b->nbClients++] = clients[i];
  }

  const char *r1[] = {"PRD-0001", "PRD-0003", "PRD-0008"};
  int q1[] = {2, 2, 2};
  const char *r2[] = {"PRD-0002", "PRD-0004", "PRD-0006"};
  int q2[] = {2, 1, 3};
  const char *r3[] = {"PRD-0005", "PRD-0007"};
  int q3[] = {1, 1};
  const char *r4[] = {"PRD-0003"};
  int q4[] = {1};
  const char *r5[] = {"PRD-0008", "PRD-0006"};
  int q5[] = {1, 2};
  const char *r6[] = {"PRD-0001", "PRD-0005"};
  int q6[] = {1, 1};
  int succes = 1;
  if (!commandeDemo(b, 1, (Date){12, 3, 2026}, "LIVREE", r1, q1, 3)) succes = 0;
  if (succes && !commandeDemo(b, 2, (Date){15, 3, 2026}, "EXPEDIEE", r2, q2, 3)) succes = 0;
  if (succes && !commandeDemo(b, 3, (Date){2, 4, 2026}, "EN_COURS", r3, q3, 2)) succes = 0;
  if (succes && !commandeDemo(b, 4, (Date){9, 4, 2026}, "ANNULEE", r4, q4, 1)) succes = 0;
  if (succes && !commandeDemo(b, 5, (Date){21, 5, 2026}, "LIVREE", r5, q5, 2)) succes = 0;
  if (succes && !commandeDemo(b, 1, (Date){1, 6, 2026}, "EN_COURS", r6, q6, 2)) succes = 0;
  if (!succes)
  {
    puts("Erreur d'initialisation des commandes.");
    detruireBoutique(b);
    exit(EXIT_FAILURE);
  }
}


