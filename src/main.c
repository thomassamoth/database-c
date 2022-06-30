#include "../include/main.h"
#include "../include/utilisateur.h"
#include "../include/menus.h"
#include "../include/affichage.h"
#include "../include/bulletin.h"

// Get the info to connect to the database server
char *get_db_info(char info_to_get[])
{
    char * info = malloc(1024);
    char buffer[1024];
	struct json_object *parsed_json;
	struct json_object *database_info;

    // Read the JSON FILE
	FILE *fichier = fopen("credentials.json","r");
	fread(buffer, 1024, 1, fichier);
	fclose(fichier);

    parsed_json = json_tokener_parse(buffer);
    // Get the associated values
	json_object_object_get_ex(parsed_json, info_to_get, &database_info);

    strcpy(info,json_object_get_string(database_info)); // copy into final vari
    return info; 
    free(info);
}


int main()
{
    // Get the credentials to connect to the database server
    char *username_database = get_db_info("username_database");
    char *password_database = get_db_info("password_database");
    char *name_database = get_db_info("database_name");


    /*  == Initialisation Database ==*/
    MYSQL *con = mysql_init(NULL);
    if(con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return(1);
    }

    if(mysql_real_connect(con, "localhost", username_database, password_database, name_database, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return(1);
    }
    else

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
