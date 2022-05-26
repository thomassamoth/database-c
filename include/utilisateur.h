#ifndef UTILISATEURS_H
#define UTILISATEURS_H

// Fichiers
#include "../include/menus.h"
#include "../include/main.h"
#include "../include/bulletin.h"


int get_id(MYSQL *con, struct Utilisateur user);
int get_id_via_pseudo(MYSQL *con, struct Utilisateur user);

char * get_status(MYSQL *con, struct Utilisateur user);
char * get_password(MYSQL *con, struct Utilisateur user);

void ajouter_promo(MYSQL *con, struct Utilisateur user);
void ajouter_classe(MYSQL *con, struct Utilisateur user);

void assignation_classe (MYSQL *con, struct Utilisateur user);
void assignation_matiere (MYSQL *con, struct Utilisateur user);

struct Utilisateur ajouter_utilisateur(MYSQL *con);
void add_user_database(MYSQL *con);
void connexion_utilisateur(MYSQL *con);

void modifier_password(MYSQL *con, struct Utilisateur user);
void modifier_groupe(MYSQL *con, struct Utilisateur user);
int supprimer_users(MYSQL *con);
void modifier_pseudo(MYSQL *con, struct Utilisateur user);


#endif
