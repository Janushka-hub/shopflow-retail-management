CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic
SOURCES = main.c produit.c client.c commande.c statistique.c utils.c initialisation.c

all: shopflow

shopflow: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o shopflow

clean:
	rm -f shopflow
