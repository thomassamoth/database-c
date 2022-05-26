#ifndef AFFICHAGE_H
#define AFFICHAGE_H

//== LIBRAIRIES ==
#include <mysql/mysql.h>
#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include <unistd.h> //pause

// Fichiers
#include "../include/menus.h"
#include "../include/main.h"
#include "../include/utilisateur.h"
#include "../include/bulletin.h"


// Fonctions
void effacer_console(float pause);
void afficher_classe(MYSQL *con);
void afficher_nb_eleve(MYSQL *con);
void afficher_pseudo(MYSQL *con, struct Utilisateur user);





#endif
