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


struct Utilisateur
{
    char nom [30];
    char prenom[30];
    char pseudo[30];
    char password[30];
    char statut[30]; // permissions
};

/* === FONCTIONS MENUS === */

int menu_classe()
{
    int i;
    printf("\t1 - Mauve\n");
    printf("\t2 - Bleue\n");
    printf("\t3 - Ivoire\n");
    printf("\t4 - Verte\n");
    printf("Choix : ");
    scanf("%d",&i);
    printf("\n");
    return i;
}

int menu_principal()
{
    int i;
    printf("======== MENU ========\n");
    printf("1 - Connexion utilisateur\n");
    printf("0 - QUITTER\n");
    printf("Choix : ");
    scanf("%d",&i);
    printf("\n");
    return i;
}

int menu_eleve()
{
    int i;
    printf("== Eleve ==\n");
    printf("1. Modifier votre mot de passe\n");
    printf("0. Déconnexion\n");
    printf("Choix : ");
    scanf("%d", &i);
    return i;
}

int menu_secretariat()
{
    int i;
    printf("== Secretariat ==\n");
    printf("1. Ajouter utilisateur\n");
    printf("2. Afficher les classes\n");
    printf("3. Afficher le nombre d'élèves\n");
    printf("0. Déconnexion\n");
    printf("Choix : ");
    scanf("%d", &i);
    return i;
}

int menu_enseignant()
{
    int i;
    printf("== Enseignant ==\n");
    printf("This is not the menu you're looking for\n");
    printf("1. Modifier votre mot de passe\n");
    printf("0. Déconnexion\n");
    printf("Choix : ");
    scanf("%d", &i);
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

/* =============================== */

/* == FONCTIONS == */

void effacer_console()
{
    printf("\033[2J\033[1;1H");
    //printf("\n");
}


int get_id(MYSQL *con, struct Utilisateur user)  //OK
{
    /* Retourne l'id de l'utilisateur souhaité */
    char request [200];
    int id;
    sprintf(request, "SELECT user_id FROM Utilisateurs WHERE user_prenom = '%s' AND user_nom = '%s';", user.prenom, user.nom);
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
            id = atoi(row[i]); //conversion en entier
        }
    }
    return id;
}


char * get_status(MYSQL *con, struct Utilisateur user) //récuperer le statut de l'utilisateur
{
    char request [200];
    char *statut = malloc(30);
    sprintf(request, "SELECT user_statut FROM Utilisateurs WHERE user_pseudo ='%s';", user.pseudo);
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
            strcpy(statut, row[i]);
        }
    }
    return statut;
    free(statut);
}

char * get_password(MYSQL *con, struct Utilisateur user) //on récupère le mot de passe de l'utilisateur
{
    char request [200];
    char *password = malloc(30);
    sprintf(request, "SELECT user_password FROM Utilisateurs WHERE user_pseudo ='%s';", user.pseudo);
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
            strcpy(password, row[i]);
        }
    }
    return password;
    free(password);
}

void afficher_classe(MYSQL *con)
{
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
        return;
    }
    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL)
    {
        //return(1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    effacer_console();
    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf(COLOR_MAGENTA "%s\n" COLOR_RESET, row[i] ? row[i] : "null");
        }
    }
    printf("\n\n");
}


void afficher_nb_eleve(MYSQL *con)
{
    /* Affiche le nombre d'élèves -- */
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
            printf("Nombre d'élèves : %d\n", atoi(row[i]));
        }
    }
}


void ajouter_classe(MYSQL *con, struct Utilisateur user) // ajouter classe dans la bdd
{
    char request[100];
    printf("En quelle classe êtes-vous ?\n");
    int classe = menu_classe(); // on affiche le menu des classes
    int id = get_id(con, user); //on récupère l'id

    /* -- Requete -- */
    sprintf(request, "INSERT INTO Personne_Classe VALUES(%d, %d);", id, classe);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        //return 1;
    }
}

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

struct Utilisateur ajouter_utilisateur(MYSQL *con) //OK
{
    char password [30] = "0"; // on initialise un password pour rentrer dans la boucle while
    struct Utilisateur user;

    printf(" == CREATION UTILISATEUR ==\n");
    printf ("\tEcrivez le prenom: ");
    scanf("%s", user.prenom);
    printf("\tEcrivez le nom: ");
    scanf("%s", user.nom);
    printf("\tEntrer un mot de passe : ");
    scanf("%s", user.password);

    /* -- Confirmation mot de passe --*/
    while(strcmp(user.password, password) != 0)
    {
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

void modifier_pseudo(MYSQL *con, struct Utilisateur user) //OK
{
    char request [500];

    /* -- Ajout pseudo à la bdd -- */
    sprintf(request, "UPDATE Utilisateurs SET user_pseudo = concat(lower(user_prenom),'.',lower(user_nom)) WHERE user_nom = '%s' AND user_prenom ='%s';", user.nom, user.prenom);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
}

void afficher_pseudo(MYSQL *con, struct Utilisateur user)
{
    char aff_pseudo[500];
    /* -- Affichage du pseudo -- */
    sprintf(aff_pseudo, "SELECT user_pseudo FROM Utilisateurs WHERE user_prenom = '%s' AND user_nom = '%s' ",user.prenom, user.nom);
    if(mysql_query(con, aff_pseudo))
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
            printf("\tVotre identifiant est " COLOR_CYAN "%s\n" COLOR_RESET, row[i] ? row[i] : "null"); // affichage en couleur cyan :-)
        }
    }
}

void add_user_database(MYSQL *con)
{
    char request [1000];
    struct Utilisateur utilisateur;
    utilisateur = ajouter_utilisateur(con);

    /* -- Ajout à la bdd -- */
    sprintf(request, "INSERT INTO Utilisateurs(user_nom, user_prenom, user_statut, user_password) VALUES ('%s', '%s', '%s', '%s');", utilisateur.nom, utilisateur.prenom, utilisateur.statut, utilisateur.password);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
    /* -- Creation pseudo -- */
    modifier_pseudo(con, utilisateur);

    if(strcmp(utilisateur.statut, "Eleve") == 0)
    {
        ajouter_classe(con, utilisateur);
    }
    effacer_console();
    afficher_pseudo(con, utilisateur);
    printf(COLOR_MAGENTA " \t\n%s %s a bien été ajouté.e à la base de données !\n\n" COLOR_RESET, utilisateur.prenom, utilisateur.nom);
}


void modifier_password(MYSQL *con, struct Utilisateur user)
{
    char request[500];
    char nouveau_pwd[30];
    char * previous_password = get_password(con, user); // on récupère le mot de passe dans la bdd
    char ancien_pwd [30];

    /* Confirmation ancien mot de passe */
    while(strcmp(previous_password, ancien_pwd) != 0)
    {
        printf("Entrer votre ancien mot de passe : ");
        scanf("%s", ancien_pwd);
    }
    printf("\n");

    /* -- Nouveau mot de passe -- */
    printf("Entrer votre nouveau mot de passe : ");
    scanf("%s", nouveau_pwd);
    sprintf(request, "UPDATE Utilisateurs SET user_password = '%s' WHERE user_pseudo = '%s'", nouveau_pwd, user.pseudo);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return;
    }
    printf(COLOR_CYAN "Le mot de passe a été mis à jour avec succès !\n" COLOR_RESET);
}


void menus_connexion(char * statut, MYSQL *con, struct Utilisateur user) //affiche les menus en fonction du type d'utilisateurs
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
                sleep(TIME);
                effacer_console();
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
                sleep(TIME);
                effacer_console();
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
                sleep(TIME);
                effacer_console();
                break;
            default:
                printf("Erreur !");
            }

        }
        while(menu_ens != 0);

    }
    free(statut); // on libère la mémoire de statut
}

void connexion_utilisateur(MYSQL *con)
{
    struct Utilisateur user;
    char request [500];
    char mot_de_passe [60];
    int correspondance;

    printf("== Connexion == \n");
    printf("\tEntrer votre pseudo : ");
    scanf("%s", user.pseudo);
    printf("\tEntrer votre mot de passe : ");
    scanf("%s", mot_de_passe);
    // on compte le nombre de personnes retournées :
    //si 1: qqn est associé, sinon personne n'est associé et il y a une erreur
    sprintf(request, "SELECT count(user_id) FROM Utilisateurs WHERE  user_pseudo  =  BINARY '%s' AND user_password =  BINARY '%s';", user.pseudo, mot_de_passe);

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
        sleep(1);
        effacer_console();
        connexion_utilisateur(con);
    }
    else
    {
        printf("Connexion établie\n");
        sleep(TIME-1.5);
        effacer_console();
        char * statut = get_status(con, user); // on récupère le statut de l'utilisateur
        menus_connexion(statut, con, user);
    }
    mysql_free_result(result);
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
    /*  == Initialisation Database ==*/
    MYSQL *con = mysql_init(NULL);
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
    int main_menu;
    do
    {
        main_menu = menu_principal();
        switch(main_menu)
        {
        case 1:
            effacer_console();
            connexion_utilisateur(con);
            break;

        case 0:
            printf("Au revoir !!\n");
            break;
        default:
            printf("Erreur lors de votre choix ! Veuillez retenter.\n");
            sleep(TIME);
            effacer_console();
        }
    }
    while(main_menu != 0);
    mysql_close(con);
    return 0;
}
