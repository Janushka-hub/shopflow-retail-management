#ifndef MINIPROJETSHOPFLOW_TYPES_H
#define MINIPROJETSHOPFLOW_TYPES_H

// 1. Structure Date
typedef struct {
    int jour;   // 1 à 31
    int mois;   // 1 à 12
    int annee;  // 2000 à 2100
} Date;

// 2. Structure Adresse
typedef struct {
    char rue[80];       // Numéro et libellé de voie
    char ville[50];     // Ville de livraison
    char codePostal[6]; // Code postal (5 chiffres)
    char pays[50];      // Pays de livraison
} Adresse;

// 3. Structure Produit
typedef struct {
    char reference[12];   // Référence commerciale (SKU)
    char designation[60]; // Libellé du produit
    char categorie[30];   // Famille de produit
    float prixUnitaireHT; // Prix unitaire hors taxes
    int quantiteStock;    // Stock disponible
    int seuilAlerte;      // Seuil de réapprovisionnement
    int actif;            // Statut du produit (suppression logique)
} Produit;

// 4. Structure Ligne de Commande (Doit être avant Commande)
typedef struct {
    char refProduit[12];   // Référence du produit commandé
    char designation[60];  // Libellé figé au moment de la vente
    int quantite;          // Quantité commandée
    float prixUnitaireHT;  // Prix figé au moment de la vente
    float sousTotalHT;     // Quantité x prixUnitaireHT
} LigneCommande;

// 5. Structure Commande (DOIT ÊTRE PLACÉE AVANT CLIENT)
typedef struct {
    int numero;            // Numéro de commande
    int idClient;          // Référence du client propriétaire
    Date dateCommande;     // Date de validation
    char statut[20];       // EN_COURS, EXPEDIEE, LIVREE, ANNULEE
    int nbLignes;          // Nombre de lignes réellement saisies
    int capaciteLignes;    // Taille du tableau alloué
    LigneCommande *lignes; // Tableau de lignes de commande

    float montantBrutHT;   // Somme des sous-totaux, avant remise
    float tauxRemise;      // 0, 5, 10 ou 15 %
    float montantRemise;   // Montant de la remise
    float montantHT;       // Montant HT après remise
    float montantTVA;      // 20% du montant HT
    float montantTTC;      // HT + TVA + frais de port
    float fraisPort;       // 0 ou 5.90 €

    Adresse adresseLivraison; // Instantané de l'adresse
} Commande;

// 6. Structure Client (Placee après Commande, donc elle connaît sa structure)
typedef struct {
    int idClient;         // Identifiant du client
    char nom[50];         // Nom de famille
    char prenom[50];      // Prénom
    char email[80];       // Adresse électronique
    char telephone[15];   // Téléphone de contact
    Adresse adresse;      // Adresse de livraison
    int pointsFidelite;   // Points de fidélité cumulés
    int nbCommandes;      // Nombre de commandes
    int capaciteCommandes;// Taille du tableau alloué
    Commande *commandes;  // Historique des commandes (plus besoin de struct devant)
    int actif;            // Statut du client
} Client;

// 7. Structure Boutique (Racine)
typedef struct {
    char nom[50];          // Nom commercial de la boutique

    Produit *produits;     // Catalogue des produits
    int nbProduits;        // Nombre de produits
    int capaciteProduits;  // Capacité allouée

    Client *clients;       // Répertoire des clients
    int nbClients;         // Nombre de clients
    int capaciteClients;   // Capacité allouée

    int numeroSuivant;     // Compteur des numéros de commande
    int idClientSuivant;   // Compteur des identifiants clients
} Boutique;

#endif //MINIPROJETSHOPFLOW_TYPES_H

k

