
#include "../include/menus.h"


/* === FONCTIONS AFFICHAGE MENUS === */

int menu_classe()
{
    int i;
    printf("%s\t1 - Mauve\n%s", COLOR_MAGENTA, COLOR_RESET);
    printf("%s\t2 - Bleue\n%s", COLOR_CYAN, COLOR_RESET);
    printf("%s\t3 - Ivoire\n%s", COLOR_WHITE, COLOR_RESET);
    printf("%s\t4 - Verte\n%s", COLOR_GREEN, COLOR_RESET);
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
    printf("\t1. Modifier votre mot de passe\n");
    printf("\t2. Afficher bulletin\n");
    printf("\t0. Déconnexion\n");
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
    printf("\t1. Modifier votre mot de passe\n");
    printf("\t2. Ecrire une appreciation à un eleve\n");
    printf("\t3. Afficher un bulletin\n");
    printf("\t0. Déconnexion\n");
    printf("Choix : ");
    scanf("%d", &i);
    return i;
}


/* Menu affiché lors de la création d'un utilisateur par le secretariat */
int menu_type_user()
{
    int i;
    printf("De quel type est l'utilisateur ?\n");
    printf("\t1 - Eleve\n");
    printf("\t2 - Enseignant\n");
    printf("\t3 - Secretariat\n");
    printf("Choix : ");
    scanf("%d",&i);
    printf("\n\n");
    return i;
}


// Affiche les menus en fonction du type d'utilisateurs.
void menus_connexion(char * statut, MYSQL *con, struct Utilisateur user)
{
    int menu_sec, menu_el, menu_ens;

    /* ---- MENUS SPECIFIQUES ---- */
    /* -- Secretariat -- */
    if(strcmp(statut, "Secretariat") == 0)
    {
        do
        {
            menu_sec = menu_secretariat(); // MENU
            switch(menu_sec)
            {
            case 1:
                effacer_console(0);
                add_user_database(con); //ajout d'un utilisateur à la bdd
                break;
            case 2:
                afficher_classe(con);
                break;
            case 3:
                effacer_console(0);
                afficher_nb_eleve(con);
                break;
            case 4:
                verrouiller_bulletin(con, user);
                break;
            case 5:
                afficher_bulletin(con);
                break;
            case 6:
                ajout_note(con, user);
                break;

            case 0:
                printf("%sDéconnexion réussie\n%s",COLOR_YELLOW, COLOR_RESET);
                effacer_console(1);
                break;
            default :
                printf("Erreur !\n");
                break;
            }

        }
        while(menu_sec != 0);

    }
    /* -- Eleve -- */
    else if(strcmp(statut, "Eleve") == 0)
    {
        do
        {
            menu_el = menu_eleve(); // affichage menu
            switch(menu_el)
            {
            case 1:
                modifier_password(con, user);
                break;

            case 2:
                afficher_bulletin_eleve (con, user);
                break;

            case 0:
                printf("%sDéconnexion réussie\n%s", COLOR_YELLOW, COLOR_RESET);
                effacer_console(1);
                break;
            default:
                printf("Erreur !\n");
                break;
            }

        }
        while(menu_el != 0);

    }
    /* -- Enseignant -- */
    else if(strcmp(statut, "Enseignant") == 0)
    {
        do
        {
            menu_ens = menu_enseignant();
            switch(menu_ens)
            {
            case 0:
                printf("%sDéconnexion réussie\n%s", COLOR_YELLOW, COLOR_RESET);
                effacer_console(1);
                break;
            case 1:
                modifier_password(con, user);
                break;
            case 2:
                ajout_appreciation (con, user);
                break;
            case 3:
                afficher_bulletin(con);
                break;
            default:
                printf("Erreur !");
                break;
            }

        }
        while(menu_ens != 0);

    }
    free(statut); // on libère la mémoire de statut
}

void menu_matieres()
{
    printf("\t1: Algebre\n");
    printf("\t2: Analyse\n");
    printf("\t3: Electromagnetisme\n");
    printf("\t4: Thermodynamique\n");
    printf("\t5: SI\n");
    printf("\t6: Informatique\n");
    printf("\t7: Algorithmique\n");
    printf("\t8: Anglais\n");
    printf("\t9: Communication\n");
    printf("\t10: Espagnol\n");
    printf("\t11: Allemand\n");
    printf("\t12: Francais\n");
    printf("\t13: Chinois\n");
}
