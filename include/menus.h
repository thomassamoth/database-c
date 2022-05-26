#ifndef MENUS_H
#define MENUS_H


// Fichiers
#include "../include/menus.h"
#include "../include/main.h"
#include "../include/utilisateur.h"
#include "../include/bulletin.h"


// Fonctions
int menu_classe();
int menu_principal();
int menu_eleve();
int menu_secretariat();
int menu_enseignant();
int menu_type_user();
void menus_connexion(char * statut, MYSQL *con, struct Utilisateur user);


#endif
