#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include "commande.h"

static Client *trouverClient(Boutique *b, int idClient) {
    if (b == NULL) {
        return NULL;
    }
    for (int i = 0; i < b->nbClients; i++) {

        if (b->clients[i].idClient == idClient &&
            b->clients[i].actif == 1) {

            return &b->clients[i];
        }
    }
    return NULL;
}

static Produit *trouverProduit(Boutique *b, const char *reference) {
    if (b == NULL || reference == NULL) {
        return NULL;
    }
    for (int i = 0; i < b->nbProduits; i++) {

        if (strcmp(b->produits[i].reference, reference) == 0) {
            return &b->produits[i];
        }
    }
    return NULL;
}


static Commande *trouverCommande(Boutique *b, int numeroCommande) {
    if (b == NULL) {
        return NULL;
    }
    for (int i = 0; i < b->nbClients; i++)
    {
        Client *client = &b->clients[i];

        for (int j = 0; j < client->nbCommandes; j++) {

            if (client->commandes[j].numero == numeroCommande) {
                return &client->commandes[j];
            }
        }
    }
    return NULL;
}

static Client *trouverClientCommande(Boutique *b, int idClient) {
    if (b == NULL) {
        return NULL;
    }
    for (int i = 0; i < b->nbClients; i++) {

        if (b->clients[i].idClient == idClient) {
            return &b->clients[i];
        }
    }
    return NULL;
}

static void mettreAJourMontants(Commande *commande) {
    if (commande == NULL) {
        return;
    }
    float montantBrutHT = 0.0f;
    for (int i = 0; i < commande->nbLignes; i++) {
        montantBrutHT += commande->lignes[i].sousTotalHT;
    }
    float tauxRemise;
    if (montantBrutHT < 100.0f) {
        tauxRemise = 0.0f;
    }
    else if (montantBrutHT < 300.0f) {
        tauxRemise = 5.0f;
    }
    else if (montantBrutHT < 600.0f) {
        tauxRemise = 10.0f;
    }
    else {
        tauxRemise = 15.0f;
    }
    float montantRemise = montantBrutHT * tauxRemise / 100.0f;
    float montantHT = montantBrutHT - montantRemise;
    float montantTVA = montantHT * 0.20f;
    float fraisPort = 0.0f;

    if (commande->nbLignes > 0 && montantBrutHT < 60.0f) {
        fraisPort = 5.90f;
    }
    commande->montantBrutHT = montantBrutHT;
    commande->tauxRemise = tauxRemise;
    commande->montantRemise = montantRemise;
    commande->montantHT = montantHT;
    commande->montantTVA = montantTVA;
    commande->fraisPort = fraisPort;
    commande->montantTTC = montantHT + montantTVA + fraisPort;
}

void creerCommande(Boutique *b) {
    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }
    if (b->nbClients <= 0 || b->clients == NULL) {
        printf("Erreur : aucun client disponible.\n");
        return;
    }
    if (b->numeroSuivant <= 0 ||
        b->numeroSuivant == INT_MAX) {

        printf("Erreur : numero de commande invalide.\n");
        return;
    }

    int idClient;
    printf("\n===== CREER UNE COMMANDE =====\n");
    printf("Identifiant du client : ");

    if (scanf("%d", &idClient) != 1) {
        printf("Erreur : identifiant invalide.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return;
    }

    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }

    Client *client = trouverClient(b, idClient);
    if (client == NULL) {
        printf("Erreur : client inexistant ou inactif.\n");
        return;
    }
    if (client->nbCommandes < 0 ||
        client->capaciteCommandes < 0 ||
        client->nbCommandes > client->capaciteCommandes ||
        (client->capaciteCommandes > 0 &&
         client->commandes == NULL)) {

        printf("Erreur : tableau des commandes invalide.\n");
        return;
    }
    Commande nouvelle = {0};
    nouvelle.numero = b->numeroSuivant;
    nouvelle.idClient = client->idClient;
    time_t maintenant = time(NULL);
    struct tm *date = localtime(&maintenant);

    if (date == NULL) {
        printf("Erreur : date indisponible.\n");
        return;
    }

    nouvelle.dateCommande.jour = date->tm_mday;
    nouvelle.dateCommande.mois = date->tm_mon + 1;
    nouvelle.dateCommande.annee = date->tm_year + 1900;

    strcpy(nouvelle.statut, "EN_COURS");

    nouvelle.lignes = NULL;
    nouvelle.nbLignes = 0;
    nouvelle.capaciteLignes = 0;

    nouvelle.adresseLivraison = client->adresse;

    if (client->nbCommandes == client->capaciteCommandes) {
        size_t nouvelleCapacite;
        if (client->capaciteCommandes == 0) {
            nouvelleCapacite = 4;
        }
        else {
            nouvelleCapacite =
                (size_t)client->capaciteCommandes * 2;
        }
        if (nouvelleCapacite > INT_MAX ||
            nouvelleCapacite > SIZE_MAX / sizeof(Commande)) {

            printf("Erreur : capacite maximale atteinte.\n");
            return;
        }
        Commande *temp = realloc(
            client->commandes,
            nouvelleCapacite * sizeof(Commande)
        );
        if (temp == NULL) {
            printf("Erreur : allocation memoire impossible.\n");
            return;
        }

        client->commandes = temp;
        client->capaciteCommandes = (int)nouvelleCapacite;
    }

    client->commandes[client->nbCommandes] = nouvelle;
    client->nbCommandes++;

    b->numeroSuivant++;

    printf("\nCommande creee avec succes !\n");
    printf("Numero : %d\n", nouvelle.numero);

    printf("Client : %s %s\n",
           client->prenom,
           client->nom);

    printf("Date : %02d/%02d/%04d\n",
           nouvelle.dateCommande.jour,
           nouvelle.dateCommande.mois,
           nouvelle.dateCommande.annee);

    printf("Statut : %s\n", nouvelle.statut);
}

void ajouterProduitCommande(Boutique *b) {
    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }

    int numeroCommande;
    printf("\n===== AJOUTER UN PRODUIT A UNE COMMANDE =====\n");
    printf("Numero de commande : ");

    if (scanf("%d", &numeroCommande) != 1) {
        printf("Erreur : numero invalide.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
    Commande *commande = trouverCommande(b, numeroCommande);

    if (commande == NULL) {
        printf("Erreur : commande introuvable.\n");
        return;
    }
    if (strcmp(commande->statut, "EN_COURS") != 0) {
        printf("Erreur : cette commande ne peut plus etre modifiee.\n");
        return;
    }
    char reference[12];
    printf("Reference du produit : ");

    if (fgets(reference, sizeof(reference), stdin) == NULL) {
        printf("Erreur de lecture.\n");
        return;
    }

    if (strchr(reference, '\n') == NULL && !feof(stdin)) {
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        printf("Erreur : reference trop longue.\n");
        return;
    }

    reference[strcspn(reference, "\n")] = '\0';
    Produit *produit = trouverProduit(b, reference);

    if (produit == NULL) {
        printf("Erreur : produit introuvable.\n");
        return;
    }
    if (produit->actif != 1) {
        printf("Erreur : produit inactif.\n");
        return;
    }

    int quantite;
    printf("Quantite souhaitee : ");

    if (scanf("%d", &quantite) != 1) {
        printf("Erreur : quantite invalide.\n");
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return;
    }
    while ((c = getchar()) != '\n' && c != EOF) {
    }
    if (quantite <= 0) {
        printf("Erreur : la quantite doit etre positive.\n");
        return;
    }

    if (commande->nbLignes < 0 ||
        commande->capaciteLignes < 0 ||
        commande->nbLignes > commande->capaciteLignes ||
        (commande->capaciteLignes > 0 &&
         commande->lignes == NULL)) {
        printf("Erreur : tableau des lignes invalide.\n");
        return;
    }

    for (int i = 0; i < commande->nbLignes; i++) {
        LigneCommande *ligne = &commande->lignes[i];
        if (strcmp(ligne->refProduit, reference) == 0) {
            if (quantite > produit->quantiteStock) {
                printf("Erreur : stock insuffisant.\n");
                return;
            }
            if (quantite > INT_MAX - ligne->quantite) {
                printf("Erreur : quantite trop grande.\n");
                return;
            }
            int anciensPoints = (int)(commande->montantTTC / 10.0f);
            ligne->quantite += quantite;
            ligne->sousTotalHT =
                ligne->quantite * ligne->prixUnitaireHT;
            produit->quantiteStock -= quantite;
            mettreAJourMontants(commande);
            int nouveauxPoints = (int)(commande->montantTTC / 10.0f);
            Client *client = trouverClientCommande(b, commande->idClient);
            if (client != NULL) {
                client->pointsFidelite += nouveauxPoints - anciensPoints;
            }

            printf("\nQuantite du produit mise a jour !\n");
            printf("Produit : %s\n", ligne->designation);
            printf("Nouvelle quantite : %d\n", ligne->quantite);
            printf("Stock restant : %d\n", produit->quantiteStock);
            printf("Montant TTC : %.2f EUR\n", commande->montantTTC);

            return;
        }
    }

    if (quantite > produit->quantiteStock) {

        printf("Erreur : stock insuffisant.\n");
        return;
    }
    if (commande->nbLignes == commande->capaciteLignes) {
        size_t nouvelleCapacite =
            commande->capaciteLignes == 0
                ? 4u
                : (size_t)commande->capaciteLignes * 2u;
        if (nouvelleCapacite > INT_MAX ||
            nouvelleCapacite > SIZE_MAX / sizeof(LigneCommande)) {
            printf("Erreur : capacite maximale atteinte.\n");
            return;
        }
        LigneCommande *temp = realloc(
            commande->lignes,
            nouvelleCapacite * sizeof(LigneCommande)
        );
        if (temp == NULL) {
            printf("Erreur : allocation memoire impossible.\n");
            return;
        }
        commande->lignes = temp;
        commande->capaciteLignes = (int)nouvelleCapacite;
    }

    LigneCommande ligne = {0};
    strcpy(ligne.refProduit, produit->reference);
    strcpy(ligne.designation, produit->designation);
    ligne.quantite = quantite;
    ligne.prixUnitaireHT = produit->prixUnitaireHT;
    ligne.sousTotalHT = quantite * produit->prixUnitaireHT;
    commande->lignes[commande->nbLignes] = ligne;
    commande->nbLignes++;
    produit->quantiteStock -= quantite;
    int anciensPoints = (int)(commande->montantTTC / 10.0f);
    mettreAJourMontants(commande);
    int nouveauxPoints = (int)(commande->montantTTC / 10.0f);
    Client *client = trouverClientCommande(b, commande->idClient);

    if (client != NULL) {
        client->pointsFidelite += nouveauxPoints - anciensPoints;
    }

    printf("\nProduit ajoute a la commande !\n");
    printf("Produit : %s\n", ligne.designation);
    printf("Quantite : %d\n", ligne.quantite);
    printf("Prix unitaire HT : %.2f EUR\n", ligne.prixUnitaireHT);
    printf("Sous-total HT : %.2f EUR\n", ligne.sousTotalHT);
    printf("Stock restant : %d\n", produit->quantiteStock);
    printf("Montant TTC de la commande : %.2f EUR\n",
           commande->montantTTC);

    if (client != NULL) {
        printf("Points de fidelite du client : %d\n",
               client->pointsFidelite);
    }
}

void calculerMontantCommande(Boutique *b) {
    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }
    int numeroCommande;
    printf("\n===== CALCULER LE MONTANT D'UNE COMMANDE =====\n");
    printf("Numero de commande : ");
    if (scanf("%d", &numeroCommande) != 1) {
        printf("Erreur : numero invalide.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }

    Commande *commande = trouverCommande(b, numeroCommande);

    if (commande == NULL) {
        printf("Erreur : commande introuvable.\n");
        return;
    }
    if (commande->nbLignes <= 0 || commande->lignes == NULL) {

        printf("Erreur : cette commande ne contient aucun produit.\n");
        return;
    }

    mettreAJourMontants(commande);
    printf("\n===== RECAPITULATIF COMMANDE N%d =====\n",
           commande->numero);
    printf("Montant brut HT : %.2f EUR\n",
           commande->montantBrutHT);
    printf("Taux de remise : %.0f %%\n",
           commande->tauxRemise);
    printf("Montant remise : %.2f EUR\n",
           commande->montantRemise);
    printf("Montant HT : %.2f EUR\n",
           commande->montantHT);
    printf("TVA (20 %%) : %.2f EUR\n",
           commande->montantTVA);
    printf("Frais de port : %.2f EUR\n",
           commande->fraisPort);
    printf("Montant TTC : %.2f EUR\n",
           commande->montantTTC);
}

void changerStatutCommande(Boutique *b) {
    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }
    int numeroCommande;
    printf("\n===== CHANGER LE STATUT D'UNE COMMANDE =====\n");
    printf("Numero de commande : ");

    if (scanf("%d", &numeroCommande) != 1) {
        printf("Erreur : numero invalide.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }

    Commande *commande = trouverCommande(b, numeroCommande);

    if (commande == NULL) {
        printf("Erreur : commande introuvable.\n");
        return;
    }

    printf("Statut actuel : %s\n", commande->statut);
    printf("\nChoisir le nouveau statut :\n");
    printf("1. EXPEDIEE\n");
    printf("2. LIVREE\n");
    printf("3. ANNULEE\n");
    printf("Votre choix : ");
    int choix;

    if (scanf("%d", &choix) != 1) {
        printf("Erreur : choix invalide.\n");
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return;
    }
    while ((c = getchar()) != '\n' && c != EOF) {
    }
    if (choix == 1) {
        if (strcmp(commande->statut, "EN_COURS") != 0) {
            printf("Erreur : transition de statut interdite.\n");
            return;
        }
        if (commande->nbLignes <= 0 || commande->lignes == NULL) {
            printf("Erreur : impossible d'expedier une commande vide.\n");
            return;
        }
        strcpy(commande->statut, "EXPEDIEE");
        printf("Commande N%d expediee avec succes.\n",
               commande->numero);
    }

    else if (choix == 2) {
        if (strcmp(commande->statut, "EXPEDIEE") != 0) {
            printf("Erreur : transition de statut interdite.\n");
            return;
        }
        strcpy(commande->statut, "LIVREE");
        printf("Commande N%d livree avec succes.\n",
               commande->numero);
    }

    else if (choix == 3) {
        if (strcmp(commande->statut, "EN_COURS") != 0) {
            printf("Erreur : seule une commande EN_COURS peut etre annulee.\n");
            return;
        }
        for (int i = 0; i < commande->nbLignes; i++) {
            LigneCommande *ligne = &commande->lignes[i];
            Produit *produit = trouverProduit(b, ligne->refProduit);
            if (produit == NULL) {
                printf("Erreur : produit %s introuvable.\n",
                       ligne->refProduit);
                return;
            }
            if (ligne->quantite <= 0 ||
                produit->quantiteStock > INT_MAX - ligne->quantite) {
                printf("Erreur : restauration du stock impossible.\n");
                return;
            }
        }
        for (int i = 0; i < commande->nbLignes; i++) {
            LigneCommande *ligne = &commande->lignes[i];
            Produit *produit = trouverProduit(b, ligne->refProduit);
            produit->quantiteStock += ligne->quantite;
        }
        Client *client = trouverClientCommande(b, commande->idClient);

        if (client != NULL) {
            int pointsCommande =
                (int)(commande->montantTTC / 10.0f);
            client->pointsFidelite -= pointsCommande;
            printf("Points de fidelite retires : %d\n",
                   pointsCommande);
            printf("Nouveau solde de points : %d\n",
                   client->pointsFidelite);
        }
        strcpy(commande->statut, "ANNULEE");
        printf("Commande N%d annulee avec succes.\n",
               commande->numero);
        printf("Les produits ont ete remis en stock.\n");
    }
    else {
        printf("Erreur : choix invalide.\n");
    }
}

void afficherCommande(Boutique *b) {
    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }
    int numeroCommande;
    printf("\n===== AFFICHER UNE COMMANDE =====\n");
    printf("Numero de commande : ");

    if (scanf("%d", &numeroCommande) != 1) {
        printf("Erreur : numero invalide.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }

    Commande *commande = trouverCommande(b, numeroCommande);

    if (commande == NULL) {
        printf("Erreur : commande introuvable.\n");
        return;
    }

    Client *client = trouverClientCommande(b, commande->idClient);
    printf("\n========== COMMANDE N%d ==========\n", commande->numero);
    printf("Date : %02d/%02d/%04d\n",
           commande->dateCommande.jour,
           commande->dateCommande.mois,
           commande->dateCommande.annee);
    printf("Statut : %s\n", commande->statut);

    if (client != NULL) {
        printf("Client : %s %s (ID : %d)\n",
               client->prenom,
               client->nom,
               client->idClient);
    }
    printf("\nAdresse de livraison :\n");
    printf("%s\n", commande->adresseLivraison.rue);
    printf("%s %s\n",
           commande->adresseLivraison.codePostal,
           commande->adresseLivraison.ville);
    printf("%s\n", commande->adresseLivraison.pays);
    printf("\n----- PRODUITS -----\n");

    if (commande->nbLignes == 0) {
        printf("Aucun produit dans cette commande.\n");
    } else {
        for (int i = 0; i < commande->nbLignes; i++) {
            LigneCommande *ligne = &commande->lignes[i];
            printf("\nProduit %d\n", i + 1);
            printf("Reference : %s\n", ligne->refProduit);
            printf("Designation : %s\n", ligne->designation);
            printf("Quantite : %d\n", ligne->quantite);
            printf("Prix unitaire HT : %.2f EUR\n",
                   ligne->prixUnitaireHT);
            printf("Sous-total HT : %.2f EUR\n",
                   ligne->sousTotalHT);
        }
    }

    printf("\n----- MONTANTS -----\n");
    printf("Montant brut HT : %.2f EUR\n",
           commande->montantBrutHT);
    printf("Remise : %.0f %%\n",
           commande->tauxRemise);
    printf("Montant remise : %.2f EUR\n",
           commande->montantRemise);
    printf("Montant HT : %.2f EUR\n",
           commande->montantHT);
    printf("TVA : %.2f EUR\n",
           commande->montantTVA);
    printf("Frais de port : %.2f EUR\n",
           commande->fraisPort);
    printf("Montant TTC : %.2f EUR\n",
           commande->montantTTC);
    printf("=================================\n");
}

void retirerProduitCommande(Boutique *b) {
    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }

    int numeroCommande;
    printf("\n===== RETIRER UN PRODUIT D'UNE COMMANDE =====\n");
    printf("Numero de commande : ");

    if (scanf("%d", &numeroCommande) != 1) {
        printf("Erreur : numero invalide.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }

    Commande *commande = trouverCommande(b, numeroCommande);
    if (commande == NULL) {
        printf("Erreur : commande introuvable.\n");
        return;
    }
    if (strcmp(commande->statut, "EN_COURS") != 0) {
        printf("Erreur : cette commande ne peut plus etre modifiee.\n");
        return;
    }
    if (commande->nbLignes <= 0 || commande->lignes == NULL) {
        printf("Erreur : la commande est vide.\n");
        return;
    }

    char reference[12];
    printf("Reference du produit a retirer : ");

    if (fgets(reference, sizeof(reference), stdin) == NULL) {
        printf("Erreur de lecture.\n");
        return;
    }
    if (strchr(reference, '\n') == NULL && !feof(stdin)) {
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        printf("Erreur : reference trop longue.\n");
        return;
    }

    reference[strcspn(reference, "\n")] = '\0';
    int indice = -1;

    for (int i = 0; i < commande->nbLignes; i++) {
        if (strcmp(commande->lignes[i].refProduit, reference) == 0) {
            indice = i;
            break;
        }
    }
    if (indice == -1) {
        printf("Erreur : produit absent de la commande.\n");
        return;
    }

    LigneCommande *ligne = &commande->lignes[indice];
    Produit *produit = trouverProduit(b, ligne->refProduit);

    if (produit == NULL) {
        printf("Erreur : produit introuvable dans le stock.\n");
        return;
    }
    if (ligne->quantite <= 0 ||
        produit->quantiteStock > INT_MAX - ligne->quantite) {

        printf("Erreur : restauration du stock impossible.\n");
        return;
    }

    int anciensPoints = (int)(commande->montantTTC / 10.0f);
    produit->quantiteStock += ligne->quantite;

    for (int i = indice; i < commande->nbLignes - 1; i++) {
        commande->lignes[i] = commande->lignes[i + 1];
    }
    commande->nbLignes--;
    mettreAJourMontants(commande);
    int nouveauxPoints = (int)(commande->montantTTC / 10.0f);
    Client *client = trouverClientCommande(b, commande->idClient);

    if (client != NULL) {
        client->pointsFidelite += nouveauxPoints - anciensPoints;
    }

    printf("\nProduit retire avec succes !\n");
    printf("Reference : %s\n", reference);
    printf("Stock actuel : %d\n", produit->quantiteStock);
    printf("Nouveau montant TTC : %.2f EUR\n",
           commande->montantTTC);

    if (client != NULL) {
        printf("Points de fidelite : %d\n",
               client->pointsFidelite);
    }
}
