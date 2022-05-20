#include <mysql/mysql.h>
#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include <unistd.h> //pause


#include "../include/menus.h"


/* === FONCTIONS AFFICHAGE MENUS === */

int menu_classe()
{
    int i;
    printf("\t1 - Mauve\n");
    printf("\t2 - Bleue\n");
    printf("\t3 - Ivoire\n");
    printf("\t4 - Verte\n");
    printf("Choix : ");
    scanf("%d",&i);
    printf("\n");
    return i;
}


int menu_principal()
{
    int i;
    printf("========== MENU ==========\n");
    printf("1 - Connexion utilisateur\n");
    printf("0 - QUITTER\n");
    printf("Choix : ");
    scanf("%d",&i);
    printf("\n");
    return i;
}


/* Menu affiché lorsque l'utilisateur connecté est de type 'Eleve' */
int menu_eleve()
{

    int i;
    printf("== Eleve ==\n");
    printf("1. Modifier votre mot de passe\n");
    printf("2. Afficher bulletin\n");
    printf("0. Déconnexion\n");
    printf("Choix : ");
    scanf("%d", &i);
    return i;
}


/* Menu affiché lorsque l'utilisateur connecté est de type 'Secrectariat' */
int menu_secretariat()
{
    int i;
    printf("== Secretariat ==\n");
    printf("\t1. Ajouter un utilisateur\n");
    printf("\t2. Afficher les classes\n");
    printf("\t3. Afficher le nombre d'élèves\n");
    printf("\t4. Verrouiller les bulletins\n");
    printf("\t5. Afficher un bulletin\n");
    printf("\t6. Ajouter une note\n");
    printf("\t0. Déconnexion\n");
    printf("Choix : ");
    scanf("%d", &i);
    return i;
}


/* Menu affiché lorsque l'utilisateur connecté est de type 'Enseignant' */
int menu_enseignant()
{
    int i;
    printf("== Enseignant ==\n");
    printf("1. Modifier votre mot de passe\n");
    printf("2. Ecrire une appreciation à un eleve\n");
    printf("3. Afficher un bulletin\n");
    printf("0. Déconnexion\n");
    printf("Choix : ");
    scanf("%d", &i);
    return i;
}


/* Menu affiché lors de la création d'un utilisateur par le secretariat */
int menu_type_user()
{
    int i;
    printf("De quel type est l'utilisateur ?\n\n");
    printf("1 - Eleve\n");
    printf("2 - Enseignant\n");
    printf("3 - Secretariat\n");
    printf("Choix : ");
    scanf("%d",&i);
    printf("\n\n");
    return i;
}
