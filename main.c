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

int menu_classe()
{
    int i;
    printf("En quelle classe êtes-vous ?\n");
    printf("\t1 - Mauve\n");
    printf("\t2 - Bleue\n");
    printf("\t3 - Ivoire\n");
    printf("\t4 - Verte\n");
    printf("Choix : ");
    scanf("%d",&i);
    printf("\n\n");
    return i;
}

void ajouter_classe(MYSQL *con, int menu, struct Utilisateur user) // ajouter classe dans la bdd
{
	char *request[100];

	//on récupère l'id de l'utilisateur
	sprintf(request, "SELECT user_id FROM Utilisateurs WHERE user_nom = '%s' AND user_prenom = '%s';", user.nom, user.prenom);
	if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL){
        return(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("Votre id est %d ", row[i] ? row[i] : "null");
        }
        printf("\n");
    }

}

int connexion_utilisateur(MYSQL *con)
{
    struct Utilisateur user;
    char request [100];

    printf("== Connexion == \n");
    printf("\tEntrer votre pseudo : ");
    scanf("%s", user.pseudo);
    sprintf(request, "SELECT user_password FROM Utilisateurs WHERE user_pseudo = '%s'", user.pseudo);

    if(mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return 1;
    }

    MYSQL_RES *result = mysql_use_result(con);

    //printf("Pointer : %s", result);
    //printf("Result : %p\n ", &result); //affichage du mot de passe
    if (result == NULL)
    {
        return(1);
    }
    //printf("Recup result %s ", result);
    //on récupère le nombre de champs
    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("%s ", row[i] ? row[i] : "null");
        }
        //printf("\n");
    }
    /* == VERIF MOT DE PASSE ==
    //printf("%s", result);
    while(strcmp(result, password) != 0)
    {
    	printf("\tConfirmer le mot de passe : ");
        scanf("%s", password);
    }
    */
    mysql_free_result(result);
}

void modifier_pseudo(MYSQL *con, struct Utilisateur user)
{
    char *request [500];
    char *aff_pseudo[100]; // requete pour afficher le pseudo
    sprintf(request, "UPDATE Utilisateurs SET user_pseudo = concat(lower(user_prenom),'.',lower(user_nom)) WHERE user_nom = '%s' AND user_prenom ='%s';", user.nom, user.prenom);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }

    /* Affichage du pseudo */
    sprintf(aff_pseudo, "SELECT user_pseudo FROM Utilisateurs WHERE user_prenom = '%s' AND user_nom = '%s' ",user.prenom, user.nom);
    if(mysql_query(con, aff_pseudo))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL){
        return(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("Votre identifiant est %s \n", row[i] ? row[i] : "null");
        }
        printf("\n");
    }
}

struct Utilisateur ajouter_utilisateur(MYSQL *con) //OK
{
    char password [30] = "0";
    char pseudo [30];
    char confirm_enseignant [10] = "code_prof";
    char confirm_secretariat [10] = "code_sec";
    struct Utilisateur user;

    printf(" == CREATION USER ==\n");
    printf ("\tEcrivez le prenom: ");
    scanf("%s", user.prenom);
    printf("\tEcrivez le nom: ");
    scanf("%s", user.nom);
    printf("\tEntrer un mot de passe : ");
    scanf("%s", user.password);

	/* -- Confirmation mot de passe --*/
    while(strcmp(user.password, password) != 0){
        printf("\tConfirmer le mot de passe : ");
        scanf("%s", password);
    }
    strcpy(user.password, password);

    /* -- Choix des permissions --*/
    int menu_user =  menu_type_user();
    switch(menu_user)
    {
		case 1: // Eleves
			strcpy(user.statut, "Eleve");
			int classe = menu_classe();
			break;

		case 2: // Enseignant
			printf("Entrer le code de validation reçu : ");
			char code[30] = "0";
			scanf("%s", code);
			/* Vérification code enseignant */
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
			/* Vérification code secretariat */
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


int get_id(MYSQL *con, struct Utilisateur user){ //OK
	/* Retourne l'id de l'utilisateur souhaité */
	char request [200];
	int id;
	sprintf(request, "SELECT user_id FROM Utilisateurs WHERE user_prenom = '%s' AND user_nom = '%s';", user.prenom, user.nom);
	if(mysql_query(con, request)){
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }

    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL){
        return(1);
    }
	int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))){
        for(int i = 0; i < num_fields; i++){
			id = atoi(row[i]); //conversion en entier
        }
    }
    return id;
}

void add_user_database(MYSQL *con)
{
    char request [1000];
    int id;
    struct Utilisateur utilisateur;
    utilisateur = ajouter_utilisateur(con);

	/* -- Ajout à la bdd -- */
    sprintf(request, "INSERT INTO Utilisateurs(user_nom, user_prenom, user_statut, user_password) VALUES ('%s', '%s', '%s', '%s');", utilisateur.nom, utilisateur.prenom, utilisateur.statut, utilisateur.password);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
    /* -- Creation pseudo -- */
    modifier_pseudo(con, utilisateur);

    if(strcmp(utilisateur.statut, "Eleve") == 0)
    {
		 id = get_id(con, utilisateur);
		 sprintf(request, "INSERT INTO ")

    }
    printf("%s %s a bien été ajouté à la base de données !\n", utilisateur.prenom, utilisateur.nom);
}

/*void modifier_password(MYSQL *con)
{
    char request[500];
    char new_password[30];
    printf("Entrer votre nouveau mot de passe : ");
    scanf("%s", new_password);
    sprintf(request, "UPDATE Utilisateurs SET user_password = '%s' WHERE user_pseudo = 'bruce'", new_password);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
    printf("Le mot de passe a été mis à jour avec succès !");
}
*/

int menu_principal()
{
    int i;
    printf("====== MENU ======\n\n");
    printf("1 - Créer un compte\n");
    printf("2 - Supprimer users\n");
    printf("4 - Connexion user\n");
    printf("0 - QUITTER\n");
    printf("Choix : ");
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
    printf("Choix : ");
    scanf("%d",&i);
    printf("\n\n");
    return i;
}

int supprimer_users(MYSQL *con)  // OK
{
    int indice;
    char request [50];
    printf("A partir de quel id voulez-vous supprimer les élèves ?");
    scanf("%d", &indice);
    sprintf(request, "DELETE FROM Utilisateurs WHERE user_id > %d", indice);
    if (mysql_query(con, request)){
        fprintf(stderr, "%s\n", mysql_error(con));
        return(1);
    }
    // Reinitialisation des user_id au nombre voulu
    sprintf(request,"ALTER TABLE Utilisateurs AUTO_INCREMENT = %d", indice);
    if(mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return(1);
    }
    printf("Utilisateurs supprimés\n");
    return(1);
}

int main()
{
    MYSQL *con = mysql_init(NULL);
    int main_menu;

    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return(1);
    }

    if (mysql_real_connect(con, "localhost", "esigelec", "esigelec", "Esigelec", 0, NULL, 0) == NULL) //
    {
        return(1);
    }
	/* == MAIN MENU == */
	do
	{
		main_menu = menu_principal();
		switch(main_menu)
		{
		case 1 :
			add_user_database(con);
			break;

		case 2:
			supprimer_users(con);
			break;

		case 4:
			connexion_utilisateur(con);
			break;
		case 0:
			printf("Goodbye!");
			break;
		default:
			printf("Erreur nombre !\n");
		}
    }while(main_menu != 0);
    mysql_close(con);
    return 0;
}
