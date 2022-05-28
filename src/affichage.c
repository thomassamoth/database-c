
#include "../include/main.h"
#include "../include/affichage.h"

// efface la console après le temps défini
void effacer_console(float pause)
{
    sleep(pause);
    // printf("\033[2J\033[1;1H");
    system("clear");
}

/*
Affiche les élèves de la classe demandée sous la forme :
Liste des élèves de la classe {Classe} :
    {Liste}
*/
void afficher_classe(MYSQL *con)
{
    char request[500];
    char request_2[500];
    char color[10]; // couleur d'affichage pour le titre

    printf("Quelle classe voulez-vous afficher ?\n");
    int menu_aff_classe;
    // Verification que l'on a toujours une réponse voulue
    do
    {
        menu_aff_classe = menu_classe();
    } while (menu_aff_classe != 1 && menu_aff_classe != 2 && menu_aff_classe != 3 && menu_aff_classe != 4);

    /* Affichage du titre de la couleur de la classse*/
    if (menu_aff_classe == 1)
    {
        strcpy(color, COLOR_MAGENTA);
    }
    else if (menu_aff_classe == 2)
    {
        strcpy(color, COLOR_CYAN);
    }
    else if (menu_aff_classe == 3)
    {
        strcpy(color, COLOR_WHITE);
    }
    else if (menu_aff_classe == 4)
    {
        strcpy(color, COLOR_GREEN);
    }

    /* Requete n°1*/
    sprintf(request,
            "SELECT concat('Liste des élèves de la classe : ', classe_nom, '\n') \
            FROM Classe WHERE classe_id = '%d'",
            menu_aff_classe);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL)
    {
        exit(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    effacer_console(0);
    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            printf("%s%s\n%s", color, row[i] ? row[i] : "null", COLOR_RESET); // Affichage en magenta
        }
    }
    /* Requete n°1 */
    sprintf(request_2,
            "SELECT concat('\t', user_prenom, ' ', upper(user_nom)) \
            FROM Utilisateurs AS uti \
            INNER JOIN Personne_Classe pc ON uti.user_id = pc.id_personne \
            INNER JOIN Classe AS cla \
            ON cla.classe_id = pc.classe_id \
            WHERE uti.user_statut = 'Eleve' \
            AND cla.classe_id = '%d' \
            ORDER BY user_nom",
            menu_aff_classe); // maxi requète :-)

    if (mysql_query(con, request_2))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
    MYSQL_RES *result2 = mysql_use_result(con);
    if (result2 == NULL)
    {
        exit(1);
    }
    int num_fields2 = mysql_num_fields(result2);
    MYSQL_ROW row2;
    while ((row2 = mysql_fetch_row(result2)))
    {
        for (int i = 0; i < num_fields2; i++)
        {
            printf(COLOR_MAGENTA "%s\n" COLOR_RESET, row2[i] ? row2[i] : "null"); // Affichage en magenta
        }
    }
    printf("\n\n");
}

// Affiche le nombre d'élèves présents dans la bdd
void afficher_nb_eleve(MYSQL *con)
{
    if (mysql_query(con, "SELECT COUNT(user_id) FROM Utilisateurs WHERE user_statut = 'Eleve'"))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL)
    {
        // return(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            printf(COLOR_CYAN "Nombre d'élèves : %d\n\n" COLOR_RESET, atoi(row[i])); // conversion en entier
        }
    }
}

// Affiche le pseudo généré après que l'utilisateur a été ajouté
void afficher_pseudo(MYSQL *con, struct Utilisateur user)
{
    char request[500];
    /* -- Affichage du pseudo -- */
    sprintf(request,
            "SELECT user_pseudo FROM Utilisateurs \
            WHERE user_prenom = '%s' AND user_nom = '%s'",
            user.prenom, user.nom);

    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL)
    {
        // return(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            printf("\tVotre identifiant est %s %s %s\n", COLOR_CYAN,
                   row[i] ? row[i] : "null", COLOR_RESET);
        }
    }
}
