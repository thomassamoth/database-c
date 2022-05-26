
#include "../include/menus.h"
#include "../include/affichage.h"
#include "../include/main.h"
#include "../include/utilisateur.h"
#include "../include/bulletin.h"



// Récuperer le verrouillage du bulletin
int get_locked(MYSQL *con, char annee[], int semestre, int matiere)
{
	int locked;
	char request [500];

	sprintf(request, "SELECT bull_locked FROM Bulletin WHERE bull_matiere = %d AND bull_annee ='%s' AND bull_semestre = %d;", matiere, annee, semestre);


    if(mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return 1;
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
            locked = atoi(row[0]); //copie du statut dans la variable retournée
        }
    }

	return locked;
}


// Verrouille les bulletins d'une promo, d'une année précise et d'un semestre précis
void verrouiller_bulletin(MYSQL *con, struct Utilisateur user)
{
    char request [500];
    char annee [15];
    int semestre, promo;
    printf("Quelle promo souhaitez verrouiller ?");
    scanf("%d", &promo);
    printf("Entrer l'année à verrouiller : ");
    scanf("%s", annee);
    printf("Semestre n° :");
    scanf("%d", &semestre);

    sprintf(request, "UPDATE Bulletin INNER JOIN Utilisateurs SET bull_locked = True \
		WHERE bull_annee = '%s' AND bull_semestre = %d AND user_promo = %d; ",
            annee, semestre, promo);

    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //return 1;
    }
    printf(COLOR_WHITE "Bulletins verrouillés pour la promo %d année %s semestre n° %d\n" COLOR_RESET, promo, annee, semestre);
    effacer_console(TIME);
}

void ajout_appreciation (MYSQL *con, struct Utilisateur prof)
{
    char appreciation[1000];
    char request [1200];
    struct Utilisateur eleve;
    char annee[10];
    int matiere, semestre, id_eleve, id_prof;

    //recherche de la matiere de l'enseignant
    id_prof = get_id_via_pseudo(con, prof);
    sprintf(request, "SELECT matiere_id FROM Personne_Matiere WHERE id_personne = %d;", id_prof);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //return 1;
    }
    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL)
    {
        exit(1);
    }
    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            matiere = atoi(row[0]);
        }
    }

    printf ("Entrer l'année scolaire (p.ex 2018-2019) : ");
    scanf("%s", annee);
    printf ("\nEntrer le semestre (1 ou 2) : ");
    scanf("%d", &semestre);

    // Verification verrouillage bulletin
    int locked = get_locked(con, annee, semestre, matiere);

    if(locked == 1)
    {
		printf(COLOR_YELLOW"Le bulletin pour l'année %s, semestre %d a été verrouillé.\n", annee, semestre);
		printf("Il ne peut plus être modifié\n" COLOR_RESET);
		effacer_console(TIME + 1);
    }

    else if(locked == 0) // Les bulletins sont encore modifiables
    {
		printf("Entrer le prénom de l'élève souhaité : ");
		scanf("%s", eleve.prenom);
		printf("Entrer le nom de l'élève souhaité : ");
		scanf("%s", eleve.nom);

		id_eleve = get_id(con, eleve); // on récupère l'id de l'élève

		// warning pour échapper les apostrophes et qu'elles soient prises en compte en SQL
		printf(COLOR_RED
			   "\t/!\\ Penser à mettre deux apostrophes simples (') si l'appréciation en comporte une !" COLOR_RESET);

		printf("\nRentrer l'appréciation : \n");
		__fpurge(stdin);
		fgets(appreciation, 1000, stdin); // prend en compte les espaces

		/* Requete */
		sprintf(request,
				"UPDATE Bulletin SET bull_appreciation = '%s' WHERE bull_eleve = %d AND bull_annee = '%s' AND bull_semestre = %d AND bull_matiere = %d;",
				appreciation, id_eleve, annee, semestre, matiere);

		if (mysql_query(con, request))
		{
			fprintf(stderr, "%s\n", mysql_error(con));
			//return 1;
		}
		printf(COLOR_CYAN "Appréciation ajoutée pour %s %s\n"COLOR_RESET, eleve.prenom, eleve.nom);
		effacer_console(TIME);
    }
}

// Ajout d'une note pour un élève
void ajout_note (MYSQL *con, struct Utilisateur secretariat)
{
    float note;
    char request [1200];
    struct Utilisateur eleve;
    char annee[10];
    int matiere=0, semestre, id_eleve;
    printf ("Entrer l'année scolaire (p.ex 2018-2019) : ");
    scanf("%s", annee);
    printf ("\nEntrer le semestre (1 ou 2) : ");
    scanf("%d", &semestre);

    printf("Entrer le prénom de l'élève souhaité : ");
    scanf("%s", eleve.prenom);
    printf("Entrer le nom de l'élève souhaité : ");
    scanf("%s", eleve.nom);
    effacer_console(0.5);

    printf("Entrer la matière où vous souhaitez saisir une note parmi les choix suivants: ");
    printf ("\n1: 'Algebre' \n2: 'Analyse'\n3: 'Electromagnetisme'\n4: 'Thermodynamique'\n5: 'SI'\n6: 'Informatique'\n");
    printf ("7: 'Algorithmique'\n8: 'Anglais'\n9: 'Communication'\n10: 'Espagnol'\n11: 'Allemand'\n12: 'Francais'\n13: 'Chinois'\n");

    while (matiere<1 || matiere>13)
    {
        fflush(stdin);
        scanf("%d", &matiere);
    }

    id_eleve = get_id(con, eleve);

    printf("\nSaisir la note finale : \n");
    scanf("%f", &note);

    /* Requete */
    sprintf(request,
            "INSERT INTO Bulletin(bull_eleve, bull_annee, bull_semestre, bull_matiere, bull_note) \
			VALUES (%d, '%s', %d, %d, '%f');", id_eleve, annee, semestre, matiere, note);

    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //return 1;
    }
    printf(COLOR_CYAN "Note ajoutée pour %s %s\n"COLOR_RESET, eleve.prenom, eleve.nom);
    effacer_console(TIME);
}

void afficher_bulletin(MYSQL *con)
{
    char request [500];
    char annee [10];
    char affichage [500];
    struct Utilisateur eleve;
    int semestre;

    printf("Entrer le prénom de l'élève souhaité : ");
    scanf("%s", eleve.prenom);
    printf("Entrer le nom de l'élève souhaité : ");
    scanf("%s", eleve.nom);
    printf("Entrer l'année du bulletin : ");
    scanf("%s", annee);
    printf("Entrer le semestre : ");
    scanf("%d", &semestre);

    int id = get_id(con, eleve);

    sprintf(request,
		"SELECT mat_nom, bull_note, bull_appreciation FROM Bulletin \
		INNER JOIN Utilisateurs ON bull_eleve = user_id \
		INNER JOIN Matiere on mat_id = bull_matiere WHERE bull_eleve = %d \
		AND bull_annee = '%s' AND bull_semestre = %d;",
		id, annee, semestre);

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
    effacer_console(1);
    // Concaténation de toutes les infos
    sprintf(affichage, "\t\tBULLETIN de %s %s | %s - semestre %d \n", eleve.prenom, eleve.nom, annee, semestre);
    printf("======================================================================\n");
    printf("%-*s\n",50, affichage);
    printf("======================================================================\n");

    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("%-*s",20, row[i]);
        }
        printf("\n");
    }
    printf("\n\n");
}


void afficher_bulletin_eleve(MYSQL *con,  struct Utilisateur user)
{
    char request [500];
    char annee [10];
    char affichage[500];
    int semestre;

    int id = get_id_via_pseudo(con, user);

    printf("Entrer l'année du bulletin : ");
    scanf("%s", annee);
    printf("Entrer le semestre : ");
    scanf("%d", &semestre);

    sprintf(request,
			"SELECT mat_nom, bull_note,bull_appreciation FROM Bulletin \
			INNER JOIN Utilisateurs ON bull_eleve = user_id \
			INNER JOIN Matiere on mat_id = bull_matiere WHERE bull_eleve = %d AND bull_annee = '%s' AND bull_semestre = %d;",
			id, annee, semestre);

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
    effacer_console(1);
    sprintf(affichage, "\t\tBULLETIN  | %s - semestre %d \n", annee, semestre); // Création ligne à afficher
    printf("======================================================================\n");
    printf("%-*s\n",50, affichage);
    printf("======================================================================\n");


    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("%-*s",20, row[i]);
        }
        printf("\n");
    }
    printf("\n\n");
}
