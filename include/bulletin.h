#ifndef BULLETIN_H
#define BULLETIN_H

//== LIBRAIRIES ==
#include <mysql/mysql.h>
#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include <unistd.h> //pause

// Fichiers
#include "../include/menus.h"
#include "../include/main.h"
//#include "../include/bulletin.h"


int get_locked(MYSQL *con, char annee[], int semestre, int matiere);
void verrouiller_bulletin(MYSQL *con, struct Utilisateur user);
void ajout_appreciation (MYSQL *con, struct Utilisateur prof);
void ajout_note (MYSQL *con, struct Utilisateur secretariat);
void afficher_bulletin(MYSQL *con);
void afficher_bulletin_eleve(MYSQL *con,  struct Utilisateur user);




#endif
