
#include "../include/main.h"
#include "../include/utilisateur.h"
#include "../include/menus.h"
#include "../include/affichage.h"
#include "../include/bulletin.h"

/* == FONCTIONS == */

int main()
{
    // Get the credentials to connect to the database server
    char buffer[1024];
	struct json_object *parsed_json;
	struct json_object *database_username;
    struct json_object *database_password;

    // Read the JSON FILE
	FILE *fichier = fopen("credentials.json","r");
	fread(buffer, 1024, 1, fichier);
	fclose(fichier);

    parsed_json = json_tokener_parse(buffer);

    // Get the associated values
	json_object_object_get_ex(parsed_json, "username_database", &database_username);
	json_object_object_get_ex(parsed_json, "password_database", &database_password);

    /*  == Initialisation Database ==*/
    MYSQL *con = mysql_init(NULL);
    if(con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //fprintf(stderr, "mysql_init() failed\n");
        return(1);
    }

    if(mysql_real_connect(con, "localhost", json_object_get_string(database_username), json_object_get_string(database_password), "Esigelec", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return(1);
    }

    /* == MENU PRINCIPAL == */
    effacer_console(0);
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
            printf("%sAu revoir !!\n%s", COLOR_YELLOW, COLOR_RESET);
            break;
        default:
            printf("%sErreur lors de votre choix ! Veuillez retenter.\n%s", COLOR_YELLOW, COLOR_RESET);
            effacer_console(TIME);
        }
    }
    while(main_menu != 0);
    mysql_close(con);
    return 0;
}
