#ifndef AFFICHAGE_H
#define AFFICHAGE_H

//== LIBRAIRIES ==
#include <mysql/mysql.h>
#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include <unistd.h> //pause

// Fichiers
#include "main.h"
#include "menus.h"


// Fonctions
void effacer_console(float pause);
void afficher_classe(MYSQL *con);
void afficher_nb_eleve(MYSQL *con);
void afficher_pseudo(MYSQL *con, struct Utilisateur user);





#endif
