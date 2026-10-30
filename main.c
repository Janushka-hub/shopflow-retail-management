#include <stdio.h>
#include "types.h"
#include "initialisation.h"

int main() {
    Boutique maBoutique;

    // 1. Tester l'initialisation
    if (initialiserBoutique(&maBoutique, "ShopFlow")) {
        printf("Succes : La boutique est prete a l'emploi !\n");
        printf("Nom de la boutique : %s\n", maBoutique.nom);
        printf("Capacite produits : %d\n", maBoutique.capaciteProduits);
    } else {
        printf("Erreur lors de l'initialisation de la boutique.\n");
    }

    // 2. Tester le nettoyage de la memoire avant de quitter
    detruireBoutique(&maBoutique);

    return 0;
}