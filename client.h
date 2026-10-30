#ifndef CLIENT_H
#define CLIENT_H

#include "types.h"

// Afficher la liste des clients
void afficherClients(const Boutique *b);

// Ajouter un nouveau client
void ajouterClient(Boutique *b);

// Rechercher un client par son identifiant
void rechercherClient(const Boutique *b);

// Modifier les informations d'un client
void modifierClient(Boutique *b);

// Desactiver un client (suppression logique)
void supprimerClient(Boutique *b);

#endif // CLIENT_H
