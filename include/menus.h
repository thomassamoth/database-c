#ifndef MENUS_H
#define MENUS_H

//== LIBRAIRIES ==
#include <mysql/mysql.h>
#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include <unistd.h> //pause

// Fichiers
#include "../include/menus.h"

// Enregistrements
struct Utilisateur
{
    char nom [30];
    char prenom[30];
    char pseudo[30];
    char password[30];
    char statut[30]; // permissions
};


// Entêtes des fonctions d'affichage des menus avec les paramètres
int menu_classe();
int menu_principal();
int menu_eleve();
int menu_secretariat();
int menu_enseignant();
int menu_type_user();

#endif
