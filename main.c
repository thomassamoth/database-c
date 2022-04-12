#include <mysql/mysql.h>
#include<stdlib.h>
#include <stdio.h>
#include<string.h>

//char host[15] = "localhost";
//char user[15] = "esigelec";
//char password[15] = "esigelec";

int menu_type_user();
struct Eleve
{
    char nom [30] ;
    char prenom [30] ;
    char promo [4] ;
    char classe [10] ;
};

struct Utilisateur
{
    char nom [30];
    char prenom[30];
    char pseudo[30];
    char password[30];
    char statut[30]; // permissions
};

void verif_enseignant(struct Utilisateur user, char confirm_enseignant)
{

}


void connexion_utilisateur(MYSQL *con)
{
	//struct Utilisateur user;
	char pseudo [30];
	char request [100];
	//char resultat[50];
	printf("== Connexion == \n");
	printf("\tEntrer votre pseudo : ");
	scanf("%s", pseudo);
	sprintf(request, "SELECT user_password FROM Utilisateurs WHERE user_pseudo = '%s'", pseudo);
	//printf(request);
	//resultat = mysql(con, request);
	//printf("%s", resultat);
	if(mysql_query(con, request))
	{
        fprintf(stderr, "%s\n", mysql_error(con));
        return 1;
    }

    //int num_fields = mysql_num_fields(con);
    MYSQL_RES *result = mysql_store_result(con);
    printf("Result : %s\n ", result);
    if (result == NULL)
    {
        return(1);
    }
    //on récupère le nombre de champs
    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("%s ", row[i] ? row[i] : "null");
        }
        printf("\n");
    }

    mysql_free_result(result);
    //return(mysql_query(con, request));
}


struct Utilisateur ajouter_utilisateur() //OK
{
    char password [30] = "0";
    char confirm_enseignant [10] = "code_prof";
    char confirm_secretariat [10] = "code_sec";
    struct Utilisateur user;

    printf(" == CREATION USER ==");
    printf("\n\tEcrivez le nom: ");
    scanf("%s", user.nom);
    printf ("\tEcrivez le prenom: ");
    scanf("%s", user.prenom);
    printf("\tEntrer votre pseudo : ");
    scanf("%s", user.pseudo);
    printf("\tEntrer un mot de passe : ");
    scanf("%s", user.password);

    while(strcmp(user.password, password) != 0) // confirmer mot de passe
    {
        printf("\tConfirmer le mot de passe : ");
        scanf("%s", password);
    }
	strcpy(user.password, password);

	// choix des permissions
    int menu_user =  menu_type_user();
    switch(menu_user)
    {
		case 1: // Eleves
			strcpy(user.statut, "Eleve");
			break;

		case 2: // Enseignant
			printf("Entrer le code de validation reçu : ");
			char code[30] = "0";
			scanf("%s", code);
			while(strcmp(code, confirm_enseignant) != 0)
			{
				printf("Entrer le code de validation reçu : ");
				scanf("%s", code);
				strcpy(code, confirm_enseignant);
			}
			strcpy(user.statut, "Enseignant");
			printf("Utilisateur de type : %s", user.statut);
					//verif_enseignant(user, confirm_enseignant);
			break;
		case 3: // Secretariat
			printf("Entrer le code de validation reçu : ");
			char code2[30] = "0";
			scanf("%s", code2);
			while(strcmp(code2, confirm_secretariat) != 0)
			{
				printf("Entrer le code de validation reçu : ");
				scanf("%s", code2);
			}
			strcpy(user.statut, "Secretariat");
			printf("Utilisateur de type : %s", user.statut);

			break;
    }

    return  user;
}


void add_user_database(MYSQL *con)
{
	char request [1000];
    struct Utilisateur utilisateur;
    utilisateur = ajouter_utilisateur();

    sprintf(request, "INSERT INTO Utilisateurs(user_nom, user_prenom,user_statut, user_pseudo, user_password) VALUES ('%s', '%s', '%s', '%s', '%s');", utilisateur.nom, utilisateur.prenom, utilisateur.statut, utilisateur.pseudo, utilisateur.password);

	if (mysql_query(con, request))
	{
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
}


int menu_principal()
{
    int i;
    printf("MENU\n\n");
    printf("1 - Créer un compte\n");
    printf("2 - Supprimer eleves \n");
    printf("3 - Supprimer users\n");
    printf("4 - Connexion user\n");
    printf("Choose: ");
    scanf("%d",&i);
    printf("\n\n");
    return i;
}

int menu_type_user()
{
	int i;
    printf("Quel type d'utilisateur êtes-vous ?\n\n");
    printf("1 - Eleve\n");
    printf("2 - Enseignant\n");
    printf("3 - Secretariat\n");
    printf("Choose: ");
    scanf("%d",&i);
    printf("\n\n");
    return i;
}

int supprimer_eleves(MYSQL *con)  // OK
{
    if (mysql_query(con, "DELETE FROM Eleves WHERE el_id > 1;"))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return(1);
    }
    if(mysql_query(con, "ALTER TABLE Eleves AUTO_INCREMENT = 1;"))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return(1);
    }
    return(1);
}

int supprimer_users(MYSQL *con)  // OK
{
    if (mysql_query(con, "DELETE FROM Utilisateurs WHERE user_id > 1;"))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return(1);
    }
    if(mysql_query(con, "ALTER TABLE Utilisateurs AUTO_INCREMENT = 1;"))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return(1);
    }
    return(1);
}

int main()
{
	MYSQL *con;
	con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return(1);
    }

	if (mysql_real_connect(con, "localhost", "esigelec", "esigelec", "Esigelec", 0, NULL, 0) == NULL) //
	{
		return(1);
	}

    int menu = menu_principal();
    switch(menu)
    {
		case 1 :
			//ajouter_utilisateur();
			add_user_database(con);
			break;

		case 2 :
			supprimer_eleves(con);
			break;
		case 3:
			supprimer_users(con);
			break;

		case 4:
			connexion_utilisateur(con);
			break;
    }

    // Lecture des données
    if (mysql_query(con, "SELECT * FROM Eleves;"))
    {
        return(1);
    }

    MYSQL_RES *result = mysql_store_result(con);
    //printf("Result : %s ", result);

    if (result == NULL)
    {
        return(1);
    }
    //on récupère le nombre de champs
    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("%s ", row[i] ? row[i] : "null");
        }
        printf("\n");
    }

    mysql_free_result(result);
    mysql_close(con);


    return 0;
}
