#include <mysql/mysql.h>
#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include <unistd.h> //pause
#include <time.h>
#include <stdio_ext.h> //fpurge

#define COLOR_RED     "\x1b[31m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_WHITE   "\x1B[97m"
#define COLOR_YELLOW  "\x1B[93m"

#define TIME 2

#include "../include/menus.h"

// struct Utilisateur est maintenant dans menus.h


/* == FONCTIONS == */

//efface la console après le temps défini
void effacer_console(float pause)
{
    sleep(pause);
    printf("\033[2J\033[1;1H");
    //printf("\n");
}

// Retourne l'id de l'utilisateur souhaité, à partir de son prénom & nom
int get_id(MYSQL *con, struct Utilisateur user)
{

    char request [200];
    int id;
    sprintf(request,
            "SELECT user_id FROM Utilisateurs WHERE user_prenom = '%s' AND user_nom = '%s'", user.prenom, user.nom);

    if(mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        printf ("get_id erreur \n");
        return 1;
    }

    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL)
    {
        return(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            id = atoi(row[i]); 	//conversion en entier

        }
    }
    return id;
}


// Retourne l'id de l'utilisateur souhaité, à partir de son pseudo
int get_id_via_pseudo(MYSQL *con, struct Utilisateur user)
{

    char request [200];
    int id=0;
    sprintf(request,
            "SELECT user_id FROM Utilisateurs WHERE user_pseudo = '%s';", user.pseudo);

    if(mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return 1;
    }

    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL)
    {
        return(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            id = atoi(row[i]); 	//conversion en entier

        }
    }
    return id;
}

// Récuperer le verrouillage du bulletin
/*int get_locked(MYSQL *con)
{
	int locked;

	sprintf(request, "SELECT bull_locked FROM Bulletin WHERE

    if(mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        printf ("get_id erreur \n");
        return 1;
    }

	return locked
}*/

// Retourne le statut de l'utilisateur à partir de son pseudo
char * get_status(MYSQL *con, struct Utilisateur user)
{
    char request [200];
    char *statut = malloc(30);
    sprintf(request,
            "SELECT user_statut FROM Utilisateurs WHERE user_pseudo ='%s';", user.pseudo);

    if(mysql_query(con, request))
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
            strcpy(statut, row[i]); //copie du statut dans la variable retournée
        }
    }
    return statut;
    free(statut); 	//libération mémoire
}

// Récupère le password de l'utilisateur à partir de son pseudo
char * get_password(MYSQL *con, struct Utilisateur user)
{
    char request [200];
    char *password = malloc(30);
    sprintf(request,
            "SELECT user_password FROM Utilisateurs WHERE user_pseudo ='%s';",
            user.pseudo);

    if(mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //return 1;
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
            strcpy(password, row[i]); //récupération password dans la bdd -> var finale
        }
    }
    return password;
    free(password);
}


/* Affiche les élèves de la classe demandée sour la forme :
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
            printf("Nombre d'élèves : %d\n\n", atoi(row[i])); // conversion en entier
        }
    }
}



/*
Ajoute les correspondances entre les Utilisateurs et les classes à partir de leur id dans la table Personne_Classe
    	- Si utilisateur est élève : ajout de sa classe
    	- Si utilisateur est enseignant : ajout des classes dont il est responsable
*/
void ajouter_classe(MYSQL *con, struct Utilisateur user)
{
    char request[100];
    printf("En quelle classe l'eleve est-il ?\n");
    int classe = menu_classe(); 	// Affichage menu classes
    int id = get_id(con, user); 	// Récupération de l'id

    /* -- Requete -- */
    sprintf(request, "INSERT INTO Personne_Classe VALUES(%d, %d);", id, classe);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //return 1;
    }
}


// Ajoute la promo à l'utilisateur
void ajouter_promo(MYSQL *con, struct Utilisateur user)
{
    char request[100];
    int promo;
    printf("Quelle est la promo ?\n");
    scanf("%d", &promo);
    int id = get_id(con, user); 	// Récupération de l'id

    /* -- Requete -- */
    sprintf(request, "UPDATE Utilisateurs SET user_promo = %d WHERE user_id = %d;", promo, id);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //return 1;
    }
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


/* Vérifie si l'utilisateur entré est bien un enseignant */
void verif_enseignant()
{
    printf("Entrer le code de validation reçu : ");
    char code[30] = "0";
    char confirm_enseignant [10] = "code_prof";
    scanf("%s", code);
    /* Vérification code enseignant */
    while(strcmp(code, confirm_enseignant) != 0)
    {
        printf("Entrer le code de validation reçu : ");
        scanf("%s", code);
        strcpy(code, confirm_enseignant);
    }
}


// Ajoute les classes à l'utilisateur de type Enseignant
void assignation_classe (MYSQL *con, struct Utilisateur user)
{
    char request [500];
    int choix;
    int classe = 0;
    printf("\nEntrer la classe assignee : \n");
    do
    {
        classe = menu_classe();

    }
    while(classe != 1 && classe != 2 && classe != 3 && classe != 4 );
    int id = get_id(con, user); 	// Récupération de l'id

    /* -- Requete -- */
    sprintf(request, "INSERT INTO Personne_Classe VALUES(%d, %d);", id, classe);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //return 1;
    }

    while(choix != 1 || choix != 2)
    {
        effacer_console(0);
        printf("Ce professeur est il/elle en charge d'une autre classe ?\n");
        printf("1. Oui \n2. Non\n");
        scanf("%d", &choix);
        if(choix == 1)
        {
            assignation_classe(con, user);
        }
        else if(choix == 2)
        {
            goto jump;
            break;
        }
jump:
        break;
    }
}


// Assigne la matière que le prof enseigne
void assignation_matiere (MYSQL *con, struct Utilisateur user)
{
    int i=0;
    char request [500];
	printf ("\n1: 'Algebre' \n2: 'Analyse'\n3: 'Electromagnetisme'\n");
	printf("4: 'Thermodynamique'\n5: 'SI'\n6: 'Informatique'\n");
    printf ("7: 'Algorithmique'\n8: 'Anglais'\n9: 'Communication'\n");
    printf("10: 'Espagnol'\n11: 'Allemand'\n12: 'Francais'\n13: 'Chinois'\n");


    printf ("\tDans quelle matiere est le prof?\n");
    while (i<1 || i>13)
    {
        fflush(stdin);
        scanf ("%d", &i);
    }
    int id = get_id(con, user); 	// Récupération de l'id
    /* -- Requete -- */
    sprintf(request, "INSERT INTO Personne_Matiere VALUES(%d, %d);", id, i);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //return 1;
    }
}




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
    printf("Entrer la matière où vous souhaitez saisir une note parmiles choix suivants: ");
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

    /*
    time_t timestamp;
    struct tm *t;
    timestamp = time(NULL);
    t = localtime(&timestamp);
    sprintf (annee, "%04u",1900+t->tm_year);
    printf("Annee : %s\n", annee);
    */

    printf ("Entrer l'année scolaire (p.ex 2018-2019) : ");
    scanf("%s", annee);
    printf ("\nEntrer le semestre (1 ou 2) : ");
    scanf("%d", &semestre);

    printf("Entrer le prénom de l'élève souhaité : ");
    scanf("%s", eleve.prenom);
    printf("Entrer le nom de l'élève souhaité : ");
    scanf("%s", eleve.nom);

    id_eleve = get_id(con, eleve);

    // warning pour échapper les apostrophes et qu'elles soient prises en compte en SQL
	printf(COLOR_RED
		"/!\\ Penser à mettre deux apostrophes simples (') si l'appréciation en comporte une !" COLOR_RESET);

    printf("\nDonner l'appreciation : \n");
	__fpurge(stdin);
    fgets(appreciation, 1000, stdin); // prend en cpte les espaces

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


void afficher_bulletin(MYSQL *con)
{
    char request [500];
    char annee [10];
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

	//sprintf(request,"SELECT bull_appreciation FROM Bulletin WHERE bull_eleve = %d;", id);
    sprintf(request, "SELECT mat_nom, bull_note, bull_appreciation FROM Bulletin \
    INNER JOIN Utilisateurs ON bull_eleve = user_id \
    INNER JOIN Matiere on mat_id = bull_matiere WHERE bull_eleve = %d \
    AND bull_annee = '%s' AND bull_semestre = %d;", id, annee, semestre);
	printf("%s", request);
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
	printf("======================================================================\n");
    printf("==                           BULLETIN                               ==\n");
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
    int semestre;

    int id = get_id_via_pseudo(con, user);

    printf("Entrer l'année du bulletin : ");
    scanf("%s", annee);
    printf("Entrer le semestre : ");
    scanf("%d", &semestre);

	//sprintf(request,"SELECT bull_appreciation FROM Bulletin WHERE bull_eleve = %d;", id);
    sprintf(request, "SELECT mat_nom, bull_note,bull_appreciation FROM Bulletin \
    INNER JOIN Utilisateurs ON bull_eleve = user_id \
    INNER JOIN Matiere on mat_id = bull_matiere WHERE bull_eleve = %d AND bull_annee = '%s' AND bull_semestre = %d;", id, annee, semestre);

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
	printf("======================================================================\n");
    printf("==                           BULLETIN                               ==\n");
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


/* Vérifie si l'utilisateur entré est bien un secrétaire */
void verif_secretariat()
{
    printf("Entrer le code de validation reçu : ");
    char code[30] = "0";
    char confirm_secretariat [10] = "code_sec";
    scanf("%s", code);
    /* Vérification code secretariat */
    while(strcmp(code, confirm_secretariat) != 0)
    {
        printf("Entrer le code de validation reçu : ");
        scanf("%s", code);
        strcpy(code, confirm_secretariat);
    }
}

// Création de l'utilisateur
// Retourne un utilisateur pour la suite
struct Utilisateur ajouter_utilisateur(MYSQL *con)
{
    char password [30] = "0"; // on initialise un mot de passe pour rentrer dans la boucle while
    struct Utilisateur user;
    int menu_user;

    printf(" == CREATION UTILISATEUR ==\n");
    printf ("\tEcrivez le prenom: ");
    scanf("%s", user.prenom);
    printf("\tEcrivez le nom: ");
    scanf("%s", user.nom);
    printf("\tEntrer un mot de passe : ");
    scanf("%s", user.password);

    /* -- Confirmation mot de passe -- */
    while(strcmp(user.password, password) != 0)
    {
        printf("\tConfirmer le mot de passe : ");
        scanf("%s", password);
    }
    strcpy(user.password, password);

    /* -- Choix du statut -- */
    do
    {
        menu_user =  menu_type_user(); // affichage menu
        switch(menu_user)
        {
        case 1: // Eleves
            strcpy(user.statut, "Eleve");
            return  user;
            break;

        case 2: // Enseignant
            //verif_enseignant();
            strcpy(user.statut, "Enseignant");
            return  user;
            break;

        case 3: // Secretariat
            //verif_secretariat();
            strcpy(user.statut, "Secretariat");
            return  user;
            break;

        default:
            printf("Erreur dans le choix de type de statut\n");
            break;
        }
    }
    while(menu_user != ' '); // toujours vrai
    return user;
}


// Modifie le pseudo généré par défault en SQL ("") par celui sous la forme {prenom.nom}
void modifier_pseudo(MYSQL *con, struct Utilisateur user) //OK
{
    char request [500];

    /* -- Ajout pseudo à la bdd -- */
    sprintf(request, "UPDATE Utilisateurs SET user_pseudo = concat(lower(user_prenom),'.', \
			lower(user_nom)) WHERE user_nom = '%s' AND user_prenom ='%s';", user.nom, user.prenom);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //return;
    }
}

// Modifie le groupe si utilisateur est un élève
void modifier_groupe(MYSQL *con, struct Utilisateur user)
{
	char request [500];
	int id;
	printf("Entrer le groupe : ");
	scanf("%d", &user.groupe);

	id = get_id(con, user);

    /* -- Ajout pseudo à la bdd -- */
    sprintf(request, "UPDATE Utilisateurs SET user_groupe = %d WHERE user_id = %d", user.groupe, id);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //return;
    }
}

// Supprime les utilisateurs de la bdd à partir de l'id fourni
int supprimer_users(MYSQL *con)
{
    int indice;
    char request [50];
    printf("A partir de quel id voulez-vous supprimer les élèves ?");
    scanf("%d", &indice);
    sprintf(request, "DELETE FROM Utilisateurs WHERE user_id > %d", indice);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return(1);
    }

    /* Reinitialisation des user_id au nombre voulu */
    sprintf(request,"ALTER TABLE Utilisateurs AUTO_INCREMENT = %d", indice);
    if(mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return(1);
    }
    printf("Utilisateurs supprimés\n");
    return(1);
}


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


// Ajoute un utilisateur dans la base de donnée après avoir crée son profil
void add_user_database(MYSQL *con)
{
    char request [1000];
    struct Utilisateur utilisateur;

    utilisateur = ajouter_utilisateur(con); // création de l'utilisateur

    /* -- Ajout à la bdd -- */
    sprintf(request,
            "INSERT INTO Utilisateurs(user_nom, user_prenom, user_statut, user_password) \
		VALUES ('%s', '%s', '%s', '%s');",
            utilisateur.nom, utilisateur.prenom, utilisateur.statut, utilisateur.password);

    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
    /* -- Creation pseudo -- */
    modifier_pseudo(con, utilisateur);

    /* Fonctions spécifiques pour chaque type d'utilisateurs */
    if(strcmp(utilisateur.statut, "Eleve") == 0)
    {
        ajouter_classe(con, utilisateur);
        modifier_groupe(con, utilisateur);
        ajouter_promo(con, utilisateur);
    }

    else if(strcmp(utilisateur.statut, "Enseignant") == 0)
    {
        effacer_console(0);
        assignation_matiere(con, utilisateur);
        assignation_classe(con, utilisateur);
    }
    /*
    else if(strcmp(utilisateur.statut, "Secretariat") == 0))
    {
    	break;
    }
    */
    effacer_console(0);
    afficher_pseudo(con, utilisateur);
    printf(COLOR_MAGENTA "\n%s %s a bien été ajouté.e à la base de données !\n\n" COLOR_RESET,
           utilisateur.prenom, utilisateur.nom);
}


void modifier_password(MYSQL *con, struct Utilisateur user)
{
    char request[500];
    char nouveau_pwd[30];
    char * pwd_precedent = get_password(con, user); // on récupère le mot de passe dans la bdd
    char ancien_pwd [30];

    /* Confirmation ancien mot de passe */
    while(strcmp(pwd_precedent, ancien_pwd) != 0)
    {
        printf("Entrer votre ancien mot de passe : ");
        scanf("%s", ancien_pwd);
    }
    printf("\n");

    /* -- Nouveau mot de passe -- */
    printf("Entrer votre nouveau mot de passe : ");
    scanf("%s", nouveau_pwd);
    sprintf(request, "UPDATE Utilisateurs SET user_password = '%s' WHERE user_pseudo = '%s'",
            nouveau_pwd, user.pseudo);

    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
    printf(COLOR_CYAN "Le mot de passe a été mis à jour avec succès !\n" COLOR_RESET);
    effacer_console(TIME);
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
                printf(COLOR_YELLOW "Déconnexion réussie\n" COLOR_RESET);
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
                printf(COLOR_YELLOW "Déconnexion réussie\n" COLOR_RESET);
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
                printf(COLOR_YELLOW "Déconnexion réussie\n" COLOR_RESET);
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


// Compte le nombre de personnes retournées correspondant au couple pseudo/password rentré
// Si retourne 1 : identifiants corrects
// Si retourne 0 : mauvais identifiants
// Sinon erreur
void connexion_utilisateur(MYSQL *con)
{
    struct Utilisateur user;
    char request [500];
    char mot_de_passe [60];
    int correspondance;

    /* -- Affichage menu -- */
    printf("== Connexion == \n");
    printf("\tEntrer votre pseudo : ");
    scanf("%s", user.pseudo);
    printf("\tEntrer votre mot de passe : ");
    scanf("%s", mot_de_passe);

    /* Requete */
    sprintf(request,
            "SELECT count(user_id) FROM Utilisateurs WHERE user_pseudo = BINARY '%s' AND  \
			user_password = BINARY '%s';", user.pseudo, mot_de_passe);

    if(mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //return 1;
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
            correspondance = atoi(row[i]); //conversion entier
        }
    }
    /* -- Conditions -- */
    if(correspondance == 0)
    {
        printf(COLOR_RED "Informations de connexion erronées. Veuillez réessayer !\n" COLOR_RESET);
        effacer_console(TIME);
        connexion_utilisateur(con);
    }
    else if(correspondance == 1)
    {
        printf(COLOR_YELLOW "Connexion réussie\n" COLOR_RESET);
        effacer_console(TIME);
        char * statut = get_status(con, user); // on récupère le statut de l'utilisateur
        menus_connexion(statut, con, user); // on affiche le menu correspondant à son statut
    }
    else
    {
        printf("Erreur : plus d'une personne correspond à ces critères.\n");
    }
    mysql_free_result(result);
}


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

// end
