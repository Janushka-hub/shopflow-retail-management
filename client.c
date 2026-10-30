#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include "client.h"

static void lireChaineClient(const char *message,
                            char *destination,
                            size_t taille) {
    char buffer[256];
    while (1) {
        printf("%s", message);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\nErreur de lecture.\n");
            exit(EXIT_FAILURE);
        }
        size_t longueur = strlen(buffer);
        if (longueur > 0 && buffer[longueur - 1] == '\n') {
            buffer[--longueur] = '\0';
        } else if (!feof(stdin)) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            printf("Erreur : saisie trop longue.\n");
            continue;
        }
        if (longueur == 0 || longueur >= taille) {
            printf("Erreur : texte vide ou trop long.\n");
            continue;
        }
        strcpy(destination, buffer);
        return;
    }
}
// Verifier qu'un code postal contient exactement 5 chiffres
static int codePostalValide(const char *code) {
    if (strlen(code) != 5) {
        return 0;
    }

    for (int i = 0; i < 5; i++) {
        if (!isdigit((unsigned char)code[i])) {
            return 0;
        }
    }

    return 1;
}
// Verification simple du format de l'email
static int emailValide(const char *email) {
    const char *arobase = strchr(email, '@');

    if (arobase == NULL || arobase == email) {
        return 0;
    }
    const char *point = strrchr(arobase + 1, '.');
    return point != NULL &&
           point > arobase + 1 &&
           point[1] != '\0' &&
           strchr(arobase + 1, '@') == NULL;
}
// Verifier si l'email est deja utilise
static int emailExiste(const Boutique *b, const char *email) {
    for (int i = 0; i < b->nbClients; i++) {
        if (strcmp(b->clients[i].email, email) == 0) {
            return 1;
        }
    }

    return 0;
}
void ajouterClient(Boutique *b) {

    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }
    // Verifier la coherence du tableau des clients
    if (b->nbClients < 0 ||
        b->capaciteClients < 0 ||
        b->nbClients > b->capaciteClients ||
        (b->capaciteClients > 0 && b->clients == NULL)) {
        printf("Erreur : tableau des clients invalide.\n");
        return;
    }
    // Verifier le prochain identifiant
    if (b->idClientSuivant <= 0 ||
        b->idClientSuivant == INT_MAX) {
        printf("Erreur : identifiant client invalide.\n");
        return;
    }
    Client nouveau = {0};

    // Attribuer un identifiant unique
    nouveau.idClient = b->idClientSuivant;
    printf("\n===== AJOUTER UN CLIENT =====\n");
    lireChaineClient("Nom : ",
                     nouveau.nom,
                     sizeof(nouveau.nom));
    lireChaineClient("Prenom : ",
                     nouveau.prenom,
                     sizeof(nouveau.prenom));
    while (1) {
        lireChaineClient("Email : ",
                         nouveau.email,
                         sizeof(nouveau.email));

        if (!emailValide(nouveau.email)) {
            printf("Erreur : email invalide.\n");
        }
        else if (emailExiste(b, nouveau.email)) {
            printf("Erreur : cet email existe deja.\n");
        }
        else {
            break;
        }
    }

    lireChaineClient("Telephone : ",
                     nouveau.telephone,
                     sizeof(nouveau.telephone));

    // Adresse de livraison
    lireChaineClient("Rue : ",
                     nouveau.adresse.rue,
                     sizeof(nouveau.adresse.rue));

    lireChaineClient("Ville : ",
                     nouveau.adresse.ville,
                     sizeof(nouveau.adresse.ville));
    while (1) {
        lireChaineClient("Code postal : ",
                         nouveau.adresse.codePostal,
                         sizeof(nouveau.adresse.codePostal));

        if (codePostalValide(nouveau.adresse.codePostal)) {
            break;
        }
        printf("Erreur : le code postal doit contenir 5 chiffres.\n");
    }

    lireChaineClient("Pays : ",
                     nouveau.adresse.pays,
                     sizeof(nouveau.adresse.pays));

    //Initialisation des informations du client
    nouveau.pointsFidelite = 0;
    nouveau.nbCommandes = 0;
    nouveau.capaciteCommandes = 0;
    nouveau.commandes = NULL;
    nouveau.actif = 1;

    if (b->nbClients == b->capaciteClients) {
        size_t nouvelleCapacite;
        if (b->capaciteClients == 0) {
            nouvelleCapacite = 4;
        }
        else {
            nouvelleCapacite = (size_t)b->capaciteClients * 2;
        }

        if (nouvelleCapacite > INT_MAX ||
            nouvelleCapacite > SIZE_MAX / sizeof(Client)) {
            printf("Erreur : capacite maximale atteinte.\n");
            return;
        }
        Client *temp = realloc(
            b->clients,
            nouvelleCapacite * sizeof(Client)
        );
        if (temp == NULL) {
            printf("Erreur : allocation memoire impossible.\n");
            return;
        }
        b->clients = temp;
        b->capaciteClients = (int)nouvelleCapacite;
    }

    //Ajouter le client au tableau
    b->clients[b->nbClients] = nouveau;
    b->nbClients++;

    //Incrementer l'identifiant uniquement apres l'ajout
    b->idClientSuivant++;
    printf("\nClient ajoute avec succes !\n");
    printf("Identifiant du client : %d\n", nouveau.idClient);
}

//Afficher la liste des clients
void afficherClients(const Boutique *b) {
    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }
    if (b->nbClients <= 0) {
        printf("Aucun client dans la boutique.\n");
        return;
    }
    if (b->clients == NULL) {
        printf("Erreur : tableau des clients inexistant.\n");
        return;
    }
    printf("\n===== LISTE DES CLIENTS =====\n");
    int nbActifs = 0;
    for (int i = 0; i < b->nbClients; i++) {
        const Client *c = &b->clients[i];
        if (c->actif != 1) {
            continue;
        }
        nbActifs++;
        printf("\n-----------------------------\n");
        printf("Identifiant : %d\n", c->idClient);
        printf("Nom : %s\n", c->nom);
        printf("Prenom : %s\n", c->prenom);
        printf("Email : %s\n", c->email);
        printf("Telephone : %s\n", c->telephone);
        printf("Adresse : %s, %s %s, %s\n",
               c->adresse.rue,
               c->adresse.codePostal,
               c->adresse.ville,
               c->adresse.pays);
        printf("Points de fidelite : %d\n", c->pointsFidelite);
        printf("Nombre de commandes : %d\n", c->nbCommandes);
    }
    if (nbActifs == 0) {
        printf("Aucun client actif dans la boutique.\n");
    }
    printf("\n=============================\n");
}

void rechercherClient(const Boutique *b) {
    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }
    if (b->nbClients <= 0 || b->clients == NULL) {
        printf("Aucun client dans la boutique.\n");
        return;
    }
    int idRecherche;
    printf("\n===== RECHERCHER UN CLIENT =====\n");
    printf("Identifiant du client : ");
    if (scanf("%d", &idRecherche) != 1) {
        printf("Erreur : identifiant invalide.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return;
    }
    for (int i = 0; i < b->nbClients; i++) {
        const Client *c = &b->clients[i];
        if (c->idClient == idRecherche && c->actif == 1) {
            printf("\n===== CLIENT TROUVE =====\n");
            printf("Identifiant : %d\n", c->idClient);
            printf("Nom : %s\n", c->nom);
            printf("Prenom : %s\n", c->prenom);
            printf("Email : %s\n", c->email);
            printf("Telephone : %s\n", c->telephone);
            printf("Adresse : %s, %s %s, %s\n",
                   c->adresse.rue,
                   c->adresse.codePostal,
                   c->adresse.ville,
                   c->adresse.pays);
            printf("Points de fidelite : %d\n", c->pointsFidelite);
            printf("Nombre de commandes : %d\n", c->nbCommandes);

            return;
        }
    }

    printf("Aucun client actif trouve avec l'identifiant %d.\n",
           idRecherche);
}
void modifierClient(Boutique *b) {
    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }
    if (b->nbClients <= 0 || b->clients == NULL) {
        printf("Aucun client dans la boutique.\n");
        return;
    }
    int idRecherche;
    printf("\n===== MODIFIER UN CLIENT =====\n");
    printf("Identifiant du client : ");
    if (scanf("%d", &idRecherche) != 1) {
        printf("Erreur : identifiant invalide.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return;
    }

    //Vider le reste de la ligne apres scanf
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }

    // Rechercher le client
    for (int i = 0; i < b->nbClients; i++) {
        Client *c = &b->clients[i];
        if (c->idClient == idRecherche && c->actif == 1) {
            printf("\nClient trouve : %s %s\n", c->prenom, c->nom);
            printf("\nSaisir les nouvelles informations :\n");
            lireChaineClient("Nouveau nom : ",
                             c->nom,
                             sizeof(c->nom));
            lireChaineClient("Nouveau prenom : ",
                             c->prenom,
                             sizeof(c->prenom));
            while (1) {
                char nouvelEmail[80];
                lireChaineClient("Nouvel email : ",
                                 nouvelEmail,
                                 sizeof(nouvelEmail));
                if (!emailValide(nouvelEmail)) {
                    printf("Erreur : email invalide.\n");
                    continue;
                }
                int dejaUtilise = 0;
                for (int j = 0; j < b->nbClients; j++) {
                    if (j != i &&
                        strcmp(b->clients[j].email, nouvelEmail) == 0) {
                        dejaUtilise = 1;
                        break;
                    }
                }

                if (dejaUtilise) {
                    printf("Erreur : cet email existe deja.\n");
                    continue;
                }
                strcpy(c->email, nouvelEmail);
                break;
            }
            lireChaineClient("Nouveau telephone : ",
                             c->telephone,
                             sizeof(c->telephone));
            lireChaineClient("Nouvelle rue : ",
                             c->adresse.rue,
                             sizeof(c->adresse.rue));
            lireChaineClient("Nouvelle ville : ",
                             c->adresse.ville,
                             sizeof(c->adresse.ville));
            while (1) {
                lireChaineClient("Nouveau code postal : ",
                                 c->adresse.codePostal,
                                 sizeof(c->adresse.codePostal));
                if (codePostalValide(c->adresse.codePostal)) {
                    break;
                }
                printf("Erreur : le code postal doit contenir 5 chiffres.\n");
            }
            lireChaineClient("Nouveau pays : ",
                             c->adresse.pays,
                             sizeof(c->adresse.pays));
            printf("\nClient modifie avec succes !\n");
            return;
        }
    }
    printf("Aucun client actif trouve avec l'identifiant %d.\n",
           idRecherche);
}

//Desactiver un client sans supprimer son historique
void supprimerClient(Boutique *b) {
    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }
    if (b->nbClients <= 0 || b->clients == NULL) {
        printf("Aucun client dans la boutique.\n");
        return;
    }
    int idRecherche;
    printf("\n===== SUPPRIMER UN CLIENT =====\n");
    printf("Identifiant du client : ");
    if (scanf("%d", &idRecherche) != 1) {
        printf("Erreur : identifiant invalide.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return;
    }
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }

    for (int i = 0; i < b->nbClients; i++) {

        Client *c = &b->clients[i];

        if (c->idClient == idRecherche) {

            if (c->actif == 0) {
                printf("Ce client est deja desactive.\n");
                return;
            }

            printf("\nClient trouve : %s %s\n",
                   c->prenom, c->nom);
            char confirmation[10];
            printf("Confirmer la suppression (o/n) : ");
            if (fgets(confirmation, sizeof(confirmation), stdin) == NULL) {
                printf("Erreur de lecture.\n");
                return;
            }
            if (confirmation[0] == 'o' || confirmation[0] == 'O') {
                c->actif = 0;
                printf("\nClient desactive avec succes !\n");
            }
            else {
                printf("\nSuppression annulee.\n");
            }
            return;
        }
    }
    printf("Aucun client trouve avec l'identifiant %d.\n",
           idRecherche);
}