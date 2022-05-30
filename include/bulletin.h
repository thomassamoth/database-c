#ifndef BULLETIN_H
#define BULLETIN_H

// Fichiers
#include "main.h"




int get_locked(MYSQL *con, char annee[], int semestre, int matiere);
void verrouiller_bulletin(MYSQL *con, struct Utilisateur user);
void ajout_appreciation (MYSQL *con, struct Utilisateur prof);
void ajout_note (MYSQL *con, struct Utilisateur secretariat);
void afficher_bulletin(MYSQL *con);
void afficher_bulletin_eleve(MYSQL *con,  struct Utilisateur user);




#endif
