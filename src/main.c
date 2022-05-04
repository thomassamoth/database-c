#include <mysql/mysql.h>
#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include <unistd.h> //pause

#define COLOR_RED     "\x1b[31m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_WHITE   "\x1B[97m"

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

int get_id(MYSQL *con, struct Utilisateur user)
{
    /* Paramètres :
    	connexion MYSQL, Utilisateur
      Retourne:
    	int : id de l'utilisateur souhaité, à partir de son prénom & nom */

    char request [200];
    int id;
    sprintf(request,
            "SELECT user_id FROM Utilisateurs WHERE user_prenom = '%s' AND \
			user_nom = '%s';", user.prenom, user.nom);

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


char * get_status(MYSQL *con, struct Utilisateur user)
{
    /* Paramètres :
    	connexion, Utilisateur */
    /* Retourne:
    	char: statut de l'utilisateur entré, à partir de son pseudo */

    char request [200];
    char *statut = malloc(30);
    sprintf(request,
            "SELECT user_statut FROM Utilisateurs WHERE user_pseudo ='%s';",
            user.pseudo);

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

char * get_password(MYSQL *con, struct Utilisateur user)
{
    /* Paramètres :
    	connexion, Utilisateur */
    /* Retourne:
    	char : mot de passe de l'utilisateur, récupéré à partir de son pseudo */

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

void rechercher_eleve(MYSQL *con, struct Utilisateur user)
{
    /* Paramètres :
    	con, Utilisateur */
    /* Retourne: None */

    printf("Entrer le prénom de l'élève souhaité : ");
    scanf("%s", user.prenom);
    printf("Entrer le nom de l'élève souhaité : ");
    scanf("%s", user.nom);

    int id = get_id(con, user);
    printf("%d", id);
}

void afficher_classe(MYSQL *con)
{
    /* Paramètres :
    	connnexion */
    /* Retourne: None

    Affiche les élèves de la classe demandée sour la forme :
    	"Liste des élèves de la classe {Classe} : "
    		{Liste} */

    char request [500];
    printf("Quelle classe voulez-vous voir ?\n");
    int menu_aff_classe = menu_classe();
    sprintf(request,
            "SELECT concat('Liste des eleves de la classe : ', classe_nom, '\n') FROM Classe WHERE classe_id = '%d' UNION \
			SELECT concat('\t', user_prenom, ' ', upper(user_nom))FROM Utilisateurs AS uti  \
			INNER JOIN Personne_Classe pc ON uti.user_id = pc.id_personne \
			INNER JOIN Classe AS cla \
			ON cla.classe_id = pc.classe_id WHERE cla.classe_id = '%d' GROUP BY user_nom ASC;", menu_aff_classe, menu_aff_classe); //maxi requète :-)

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


void afficher_nb_eleve(MYSQL *con)
{
    /* Paramètres :
    	connexion */
    /* Retourne:  None

    Affiche le nombre d'élèves présents dans la bdd */

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
            printf("Nombre d'élèves : %d\n", atoi(row[i])); // conversion en entier
        }
    }
}


void ajouter_classe(MYSQL *con, struct Utilisateur user)
{
    /* Paramètres :
    	con, Utilisateur */

    /* Ajoute les correspondances entre les Utilisateurs et les classes à partir de leur id dans la table Personne_Classe
    	- Si utilisateur est élève : ajout de sa classe
    	- Si utilisateur est enseignant : ajout des classes dont il est responsable
    */

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

/*
void assignation_classe ()
{
    int i=0;
    char request [500];
    printf ("Dans quelle matiere est le prof?");
    printf ("1: 'Algebre' \n2: 'Analyse'\n3: 'Electromagnetisme'\n4: 'Thermodynamique'\n5: 'SI'\n6: 'Informatique'\n7: 'Algorithmique'\n8: 'Anglais'\n9: 'Communication'\n10: 'Espagnol'\n11: 'Allemand'\n12: 'Francais'\n13: 'Chinois'");
    while (i==0)
    {
        fflush(stdin);
        scanf ("%d", &i);
    }
    sprintf(request, "INSERT INTO Matiere(mat_prof_nom, mat_prof_prenom, %s, %s);", utilisateur.nom, Utilisateurprenom;
}
*/

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

struct Utilisateur ajouter_utilisateur(MYSQL *con)
{
    // Paramètre : connexion
    // Retourne : struct Utilisateur : Utilisateur


    char password [30] = "0"; // on initialise un mot de passe pour rentrer dans la boucle while
    struct Utilisateur user;

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
    int menu_user =  menu_type_user();
    switch(menu_user)
    {
    case 1: // Eleves
        strcpy(user.statut, "Eleve");
        break;

    case 2: // Enseignant
        verif_enseignant();
        strcpy(user.statut, "Enseignant");
        break;

    case 3: // Secretariat
        verif_secretariat();
        strcpy(user.statut, "Secretariat");
        break;

    default:
        printf("Erreur dans le choix de type de statut");
        break;
    }
    return  user;
}

// Modifie le pseudo généré par défault en SQL ("") par celui sous la forme {prenom.nom}
void modifier_pseudo(MYSQL *con, struct Utilisateur user) //OK
{
    /* Paramètres :
    	connexion,  Utilisateur */
    /* Retourne : None */

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
    sprintf(request, "INSERT INTO Utilisateurs(user_nom, user_prenom, user_statut, user_password) \
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
    }

    /*else if(strcmp(utilisateur.statut, "Enseignant") == 0))
    {
    	continue;
    }
    else if(strcmp(utilisateur.statut, "Secretariat") == 0))
    {
    	continue;
    } */
    effacer_console(TIME);
    afficher_pseudo(con, utilisateur);
    printf(COLOR_MAGENTA "\t\n%s %s a bien été ajouté.e à la base de données !\n\n" COLOR_RESET,
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

//affiche les menus en fonction du type d'utilisateurs.
void menus_connexion(char * statut, MYSQL *con, struct Utilisateur user)
{
    int menu_sec, menu_el, menu_ens;

    /* ---- MENUS SPECIFIQUES ---- */
    /* -- Secretariat -- */
    if(strcmp(statut, "Secretariat") == 0)
    {
        do
        {
            menu_sec = menu_secretariat();
            switch(menu_sec)
            {
            case 1:
                //effacer_console();
                add_user_database(con); //ajout d'un utilisateur à la bdd
                break;
            case 2:
                afficher_classe(con);
                break;
            case 3:
                afficher_nb_eleve(con);
                break;
            case 0:
                printf("Déconnexion réussie\n");
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
            menu_el = menu_eleve();
            switch(menu_el)
            {
            case 1:
                modifier_password(con, user);
                break;
            case 0:
                printf("Déconnexion réussie\n");
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
                printf("Déconnexion réussie\n");
                effacer_console(1);
                break;
            case 1:
                modifier_password(con, user);
                break;
            case 2:
                printf("== Recherche élève ==\n");
                //rechercher_eleve(con, user);
                break;
            default:
                printf("Erreur !");
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
    else
    {
        printf("Connexion établie\n");
        effacer_console(TIME);
        char * statut = get_status(con, user); // on récupère le statut de l'utilisateur
        menus_connexion(statut, con, user);
    }
    mysql_free_result(result);
}

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
