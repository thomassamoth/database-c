#include <mysql/mysql.h>
#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include <ctype.h> // lowercase - uppercase

//char host[15] = "localhost";
//char user[15] = "esigelec";
//char password[15] = "esigelec";

int menu_type_user();
//void modifier_pseudo(MYSQL *con, struct Utilisateur user);

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

char *toLower(char *str, size_t len) //lowercase
{
    char *str_l = calloc(len+1, sizeof(char));

    for (size_t i = 0; i < len; ++i)
    {
        str_l[i] = tolower((unsigned char)str[i]);
    }
    return str_l;
}




int connexion_utilisateur(MYSQL *con)
{
    struct Utilisateur user;
    //char password[30] = "0";
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
    //return(mysql_query(con, request));
}
char modifier_pseudo(MYSQL *con, struct Utilisateur user)
{
    char request [500], concat [100];
    sprintf(concat,"concat(lower(user_prenom),'.',lower(user_nom))"); //concatenation
    printf("%s", concat);
    sprintf(request, "UPDATE Utilisateurs SET user_pseudo = %s WHERE user_prenom = '%s' AND user_prenom ='%s'", concat, user.prenom, user.nom);
    printf("Final request : %s", request);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
    printf("Le pseudo a été mis à jour avec succès !");
	return user.pseudo;
}

struct Utilisateur ajouter_utilisateur(MYSQL *con) //OK
{
    char password [30] = "0";
    char confirm_enseignant [10] = "code_prof";
    char confirm_secretariat [10] = "code_sec";
    struct Utilisateur user;

    printf(" == CREATION USER ==\n");

    printf ("\tEcrivez le prenom: ");
    scanf("%s", user.prenom);
    printf("\tEcrivez le nom: ");
    scanf("%s", user.nom);
    /* UPDATE Utilisateurs SET user_pseudo = concat(lower(user_prenom),'.',lower(user_nom)) WHERE user_id = 3; */

    //printf("\tEntrer un pseudo : ");
    //scanf("%s", user.pseudo);
    //printf("\tVotre pseudo est %s", user.pseudo);
    printf("\tEntrer un mot de passe : ");
    scanf("%s", user.password);

    while(strcmp(user.password, password) != 0) // confirmation mot de passe
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
    modifier_pseudo(con, user);
    return  user;
}


void add_user_database(MYSQL *con)
{
    char request [1000];
    struct Utilisateur utilisateur;
    utilisateur = ajouter_utilisateur(con);

    sprintf(request, "INSERT INTO Utilisateurs(user_nom, user_prenom, user_statut, user_pseudo, user_password) VALUES ('%s', '%s', '%s', '%s', '%s');", utilisateur.nom, utilisateur.prenom, utilisateur.statut, utilisateur.pseudo, utilisateur.password);

    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
}

void modifier_champ(MYSQL *con)
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
int menu_principal()
{
    int i;
    printf("MENU\n\n");
    printf("1 - Créer un compte\n");
    printf("2 - Supprimer users\n");
    printf("4 - Connexion user\n");
    printf("5 - Changer pseudo\n");
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


int supprimer_users(MYSQL *con)  // OK
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
    sprintf(request,"ALTER TABLE Utilisateurs AUTO_INCREMENT = %d", indice);
    if(mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return(1);
    }
    printf("Utilisateurs supprimés");
    return(1);
}

int main()
{
	struct Utilisateur user;
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

    case 2:
        supprimer_users(con);
        break;

    case 4:
        connexion_utilisateur(con);
        break;
    case 5:
        modifier_pseudo(con, user);
        break;
    }

    // Lecture des données
    /*
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

    while ((row = mysql_fetch_row(result))){
        for(int i = 0; i < num_fields; i++)
        {
            printf("%s ", row[i] ? row[i] : "null");
        }
        printf("\n");
    }
	mysql_free_result(result);
	*/
    mysql_close(con);

    return 0;
}
