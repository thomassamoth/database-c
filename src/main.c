
// imports librairies dans main.h

#include "../include/main.h"
#include "../include/menus.h"
#include "../include/affichage.h"
#include "../include/utilisateur.h"
#include "../include/bulletin.h"


/* == FONCTIONS == */

int main()
{
    /*  == Initialisation Database ==*/
    MYSQL *con = mysql_init(NULL);
    if(con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return(1);
    }

    if(mysql_real_connect(con, "localhost", "esigelec", "esigelec", "Esigelec", 0, NULL, 0) == NULL)
    {
        return(1);
    }

    /* == MENU PRINCIPAL == */
    int main_menu;
    do
    {
        main_menu = menu_principal();
        switch(main_menu)
        {
        case 1:
            effacer_console(0);
            connexion_utilisateur(con);
            break;

        case 0:
            printf("Au revoir !!\n");
            break;
        default:
            printf("Erreur lors de votre choix ! Veuillez retenter.\n");
            effacer_console(TIME);
        }
    }
    while(main_menu != 0);
    mysql_close(con);
    return 0;
}
