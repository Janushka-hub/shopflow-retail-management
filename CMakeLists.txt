cmake_minimum_required(VERSION 3.25)
project(MiniprojetShopflow C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(MiniprojetShopflow
        main.c
        commande.c
        initialisation.c
        produit.c
        statistique.c
        utils.c
        types.h
        commande.h
        initialisation.h
        produit.h
        statistique.h
        utils.h
)
