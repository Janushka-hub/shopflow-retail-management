#include <stdio.h>
#include "types.h"
#include "utils.h"
#include "produit.h"
#include "client.h"
#include "commande.h"
#include "statistique.h"
#include "initialisation.h"

int main(void)
{
  Boutique boutique;
  initialiserDonneesTest(&boutique);
  int choix;
  do
  {
    puts("\n========== SHOPFLOW ==========\n1. Gestion des produits\n2. Gestion des clients\n3. Gestion des commandes\n4. Statistiques\n0. Quitter");
    choix = lireEntier("Votre choix : ", 0, 4);
    switch (choix)
    {
      case 1:
        menuProduits(&boutique);
        break;

      case 2:
        menuClients(&boutique);
        break;

      case 3:
        menuCommandes(&boutique);
        break;

      case 4:
        menuStatistiques(&boutique);
        break;

      default:
        break;

    }

  }
  while (choix);
  detruireBoutique(&boutique);
  puts("Au revoir !");
  return 0;
}


