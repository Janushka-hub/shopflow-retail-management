#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "utils.h"

void lireTexte(const char *message, char *texte, int taille)
{
  char tampon[512];
  while (1)
  {
    printf("%s", message);
    if (!fgets(tampon, sizeof(tampon), stdin))
    {
      exit(0);
    }
    size_t n = strlen(tampon);
    if (n && (tampon[n - 1] == '\n'))
    {
      tampon[--n] = '\0';
    }
    else
    {
      int c;
      while (((c = getchar()) != '\n') && (c != EOF))
      {
      }

      if (c != EOF)
      {
        puts("Texte trop long.");
        continue;
      }
    }
    if ((n == 0) || (n >= ((size_t) taille)))
    {
      puts("Texte vide ou trop long.");
      continue;
    }
    strcpy(texte, tampon);
    return;
  }

}

int lireEntier(const char *message, int minimum, int maximum)
{
  char s[100];
  char reste;
  long v;
  while (1)
  {
    printf("%s", message);
    if (!fgets(s, sizeof(s), stdin))
    {
      exit(0);
    }
    if (((sscanf(s, "%ld %c", &v, &reste) == 1) && (v >= minimum)) && (v <= maximum))
    {
      return (int) v;
    }
    puts("Nombre entier invalide.");
  }

}

float lirePrix(const char *message, float minimum)
{
  char s[100];
  char reste;
  float v;
  while (1)
  {
    printf("%s", message);
    if (!fgets(s, sizeof(s), stdin))
    {
      exit(0);
    }
    if (((sscanf(s, "%f %c", &v, &reste) == 1) && (v >= minimum)) && (v < 100000000.0f))
    {
      return v;
    }
    puts("Montant invalide (utiliser un point decimal).");
  }

}

int dateValide(Date d)
{
  int jours[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if ((((d.annee < 2000) || (d.annee > 2100)) || (d.mois < 1)) || (d.mois > 12))
  {
    return 0;
  }
  if ((d.mois == 2) && (((d.annee % 400) == 0) || (((d.annee % 4) == 0) && ((d.annee % 100) != 0))))
  {
    jours[2] = 29;
  }
  return (d.jour >= 1) && (d.jour <= jours[d.mois]);
}

Date lireDate(void)
{
  Date d;
  do
  {
    d.jour = lireEntier("Jour : ", 1, 31);
    d.mois = lireEntier("Mois : ", 1, 12);
    d.annee = lireEntier("Annee : ", 2000, 2100);
    if (!dateValide(d))
    {
      puts("Date impossible.");
    }
  }
  while (!dateValide(d));
  return d;
}

static int chiffres(const char *s, int n)
{
  if (strlen(s) != ((size_t) n))
  {
    return 0;
  }
  for (int i = 0; i < n; i++)
  {
    if (!isdigit((unsigned char) s[i]))
    {
      return 0;
    }
  }

  return 1;
}

int telephoneValide(const char *s)
{
  return chiffres(s, 10);
}

int emailValide(const char *s)
{
  const char *a = strchr(s, '@');
  return (((a && (a != s)) && (!strchr(a + 1, '@'))) && strchr(a + 1, '.')) && strchr(a + 1, '.')[1];
}

void lireAdresse(Adresse *a)
{
  char cp[6];
  lireTexte("Rue : ", a->rue, sizeof(a->rue));
  lireTexte("Ville : ", a->ville, sizeof(a->ville));
  do
  {
    lireTexte("Code postal (5 chiffres) : ", cp, sizeof(cp));
  }
  while (!chiffres(cp, 5));
  strcpy(a->codePostal, cp);
  lireTexte("Pays : ", a->pays, sizeof(a->pays));
}

void afficherAdresse(Adresse a)
{
  printf("%s, %s %s, %s", a.rue, a.codePostal, a.ville, a.pays);
}

int reserver(void **tableau, int *capacite, int nombre, unsigned long taille)
{
  if (nombre <= (*capacite))
  {
    return 1;
  }
  int nouveau = 4;
  if (*capacite > 0) {
    if (*capacite > INT_MAX / 2) return 0;
    nouveau = *capacite * 2;
  }
  while (nouveau < nombre)
  {
    if (nouveau > (INT_MAX / 2))
    {
      return 0;
    }
    nouveau *= 2;
  }

  if (taille && (((size_t) nouveau) > (((size_t) (-1)) / taille)))
  {
    return 0;
  }
  void *p = realloc(*tableau, ((size_t) nouveau) * taille);
  if (!p)
  {
    puts("Erreur : memoire insuffisante.");
    return 0;
  }
  *tableau = p;
  *capacite = nouveau;
  return 1;
}

Produit *chercherProduit(Boutique *b, const char *ref)
{
  for (int i = 0; i < b->nbProduits; i++)
  {
    if (!strcmp(b->produits[i].reference, ref))
    {
      return &b->produits[i];
    }
  }

  return NULL;
}

Client *chercherClient(Boutique *b, int id)
{
  for (int i = 0; i < b->nbClients; i++)
  {
    if (b->clients[i].idClient == id)
    {
      return &b->clients[i];
    }
  }

  return NULL;
}

Commande *chercherCommande(Boutique *b, int numero, Client **proprietaire)
{
  for (int i = 0; i < b->nbClients; i++)
  {
    for (int j = 0; j < b->clients[i].nbCommandes; j++)
    {
      if (b->clients[i].commandes[j].numero == numero)
      {
        if (proprietaire)
        {
          *proprietaire = &b->clients[i];
        }
        return &b->clients[i].commandes[j];
      }
    }

  }

  return NULL;
}


