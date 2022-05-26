
#include "../include/menus.h"
#include "../include/affichage.h"
#include "../include/main.h"
#include "../include/utilisateur.h"
#include "../include/bulletin.h"


//efface la console après le temps défini
void effacer_console(float pause)
{
    sleep(pause);
    printf("\033[2J\033[1;1H");
}


/*
Affiche les élèves de la classe demandée sour la forme :
	"Liste des élèves de la classe {Classe} : "
		{Liste}
*/
void afficher_classe(MYSQL *con)
{
    char request [500];
    printf("Quelle classe voulez-vous afficher ?\n");
    int menu_aff_classe;
    // Verification que l'on a toujours une réponse voulue
    do
    {
        menu_aff_classe = menu_classe();
    }
    while(menu_aff_classe != 1 && menu_aff_classe != 2 && menu_aff_classe != 3 && menu_aff_classe != 4);

    sprintf(request,
            "SELECT concat('Liste des eleves de la classe : ', classe_nom, '\n') FROM Classe WHERE classe_id = '%d' UNION \
			SELECT concat('\t', user_prenom, ' ', upper(user_nom))FROM Utilisateurs AS uti \
			INNER JOIN Personne_Classe pc ON uti.user_id = pc.id_personne \
			INNER JOIN Classe AS cla \
			ON cla.classe_id = pc.classe_id WHERE uti.user_statut = 'Eleve' AND cla.classe_id = '%d' GROUP BY user_nom ASC;", menu_aff_classe, menu_aff_classe); //maxi requète :-)
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
        for(int i = 0; i < num_fields; i++)
        {
            printf(COLOR_MAGENTA "%s\n" COLOR_RESET, row[i] ? row[i] : "null"); // Affichage en magenta
        }
    }
    printf("\n\n");
}


//Affiche le nombre d'élèves présents dans la bdd
void afficher_nb_eleve(MYSQL *con)
{
    if(mysql_query(con, "SELECT COUNT(user_id) FROM Utilisateurs WHERE user_statut = 'Eleve'"))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL)
    {
        //return(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf(COLOR_CYAN"Nombre d'élèves : %d\n\n" COLOR_RESET, atoi(row[i])); // conversion en entier
        }
    }
}


// Affiche le pseudo généré après que l'utilisateur ait été ajouté
void afficher_pseudo(MYSQL *con, struct Utilisateur user)
{
    char request[500];
    /* -- Affichage du pseudo -- */
    sprintf(request, "SELECT user_pseudo FROM Utilisateurs WHERE user_prenom = '%s' AND \
			user_nom = '%s' ",user.prenom, user.nom);

    if(mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL)
    {
        //return(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("\tVotre identifiant est " COLOR_CYAN "%s\n" COLOR_RESET,
                   row[i] ? row[i] : "null"); // affichage en couleur cyan :-)
        }
    }
}

