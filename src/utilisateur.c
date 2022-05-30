
#include "../include/utilisateur.h"

// Retourne l'id de l'utilisateur souhaité, à partir de son prénom & nom
int get_id(MYSQL *con, struct Utilisateur user)
{

    char request[200];
    int id;
    sprintf(request,
            "SELECT user_id FROM Utilisateurs WHERE user_prenom = '%s' AND user_nom = '%s'", user.prenom, user.nom);

    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return 1;
    }

    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL)
    {
        return (1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            id = atoi(row[i]); // conversion en entier
        }
    }
    return id;
}

// Retourne l'id de l'utilisateur souhaité, à partir de son pseudo
int get_id_via_pseudo(MYSQL *con, struct Utilisateur user)
{

    char request[200];
    int id = 0;
    sprintf(request,
            "SELECT user_id FROM Utilisateurs WHERE user_pseudo = '%s';", user.pseudo);

    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return 1;
    }

    MYSQL_RES *result = mysql_use_result(con);
    if (result == NULL)
    {
        return (1);
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            id = atoi(row[i]); // conversion en entier
        }
    }
    return id;
}

// Retourne le statut de l'utilisateur à partir de son pseudo
char *get_status(MYSQL *con, struct Utilisateur user)
{
    char request[200];
    char *statut = malloc(30);
    sprintf(request,
            "SELECT user_statut FROM Utilisateurs WHERE user_pseudo ='%s';", user.pseudo);

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
            strcpy(statut, row[i]); // copie du statut dans la variable retournée
        }
    }
    return statut;
    free(statut); // libération mémoire
}

// Récupère le password de l'utilisateur à partir de son pseudo
char *get_password(MYSQL *con, struct Utilisateur user)
{
    char request[200];
    char *password = malloc(30);
    sprintf(request,
            "SELECT user_password FROM Utilisateurs WHERE user_pseudo ='%s';",
            user.pseudo);

    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        // return 1;
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
            strcpy(password, row[i]); // récupération password dans la bdd -> var finale
        }
    }
    return password;
    free(password);
}

// Ajoute la promo à l'utilisateur
void ajouter_promo(MYSQL *con, struct Utilisateur user)
{
    char request[100];
    int promo;
    printf("Entrer la promo : ");
    scanf("%d", &promo);
    int id = get_id(con, user); // Récupération de l'id

    /* -- Requete -- */
    sprintf(request, "UPDATE Utilisateurs SET user_promo = %d WHERE user_id = %d;", promo, id);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
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
    int classe = menu_classe(); // Affichage menu classes
    int id = get_id(con, user); // Récupération de l'id

    /* -- Requete -- */
    sprintf(request, "INSERT INTO Personne_Classe VALUES(%d, %d);", id, classe);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
}

// Ajoute les classes à l'utilisateur de type Enseignant
void assignation_classe(MYSQL *con, struct Utilisateur user)
{
    char request[500];
    int choix;
    int classe = 0;
    printf("\nEntrer la classe assignée : \n");
    do
    {
        classe = menu_classe(); // affichage menu

    } while (classe != 1 && classe != 2 && classe != 3 && classe != 4);

    int id = get_id(con, user); // Récupération de l'id

    /* -- Requete -- */
    sprintf(request, "INSERT INTO Personne_Classe VALUES(%d, %d);", id, classe);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        // return 1;
    }

    while (choix != 1 || choix != 2)
    {
        effacer_console(0);
        printf("Ce professeur est il/elle en charge d'une autre classe ?\n");
        printf("1. Oui \n2. Non\n");
        scanf("%d", &choix);
        if (choix == 1)
        {
            assignation_classe(con, user);
        }
        else if (choix == 2)
        {
            goto jump;
            break;
        }
    jump:
        break;
    }
}

// Assigne la matière que le prof enseigne
void assignation_matiere(MYSQL *con, struct Utilisateur user)
{
    int matiere = 0;
    char request[500];
    menu_matieres();
    printf("\tQuelle matière le professeur enseigne-t-il ?\n");
    while (matiere < 1 || matiere > 13)
    {
        fflush(stdin);
        scanf("%d", &matiere);
    }
    int id = get_id(con, user); // Récupération de l'id
    /* -- Requete -- */
    sprintf(request, "INSERT INTO Personne_Matiere VALUES(%d, %d);", id, matiere);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
}

// Création de l'utilisateur
// Retourne un utilisateur pour la suite
struct Utilisateur ajouter_utilisateur(MYSQL *con)
{
    char password[30] = "0"; // on initialise un mot de passe pour rentrer dans la boucle while
    struct Utilisateur user;
    int menu_user;

    printf(" == CREATION UTILISATEUR ==\n");
    printf("\tEcrivez le prenom: ");
    scanf("%s", user.prenom);
    printf("\tEcrivez le nom: ");
    scanf("%s", user.nom);
    printf("\tEntrer un mot de passe : ");
    scanf("%s", user.password);

    /* -- Confirmation mot de passe -- */
    while (strcmp(user.password, password) != 0)
    {
        printf("\tConfirmer le mot de passe : ");
        scanf("%s", password);
    }
    strcpy(user.password, password);

    /* -- Choix du statut -- */
    do
    {
        menu_user = menu_type_user(); // affichage menu
        switch (menu_user)
        {
        case 1: // Eleves
            strcpy(user.statut, "Eleve");
            return user;
            break;

        case 2: // Enseignant
            strcpy(user.statut, "Enseignant");
            return user;
            break;

        case 3: // Secretariat
            strcpy(user.statut, "Secretariat");
            return user;
            break;

        default:
            printf("Erreur dans le choix de type de statut\n");
            break;
        }
    } while (menu_user != ' '); // toujours vrai
    return user;
}

// Modification du mot de passe d'un utilisateur
void modifier_password(MYSQL *con, struct Utilisateur user)
{
    char request[500];
    char nouveau_pwd[30];
    char *pwd_precedent = get_password(con, user); // on récupère le mot de passe dans la bdd
    char ancien_pwd[30];

    /* Confirmation ancien mot de passe */
    while (strcmp(pwd_precedent, ancien_pwd) != 0)
    {
        printf("Entrer votre %sancien%s mot de passe : ", COLOR_MAGENTA, COLOR_RESET);
        scanf("%s", ancien_pwd);
    }
    printf("\n");

    /* -- Nouveau mot de passe -- */
    printf("Entrer votre %snouveau%s mot de passe : ", COLOR_GREEN, COLOR_MAGENTA);
    scanf("%s", nouveau_pwd);
    sprintf(request,
            "UPDATE Utilisateurs SET user_password = '%s' \
            WHERE user_pseudo = '%s'", \
            nouveau_pwd, user.pseudo);

    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
    printf("%sLe mot de passe a été mis à jour avec succès !\n%s", COLOR_CYAN, COLOR_RESET);
    effacer_console(TIME);
}

// Compte le nombre de personnes retournées correspondant au couple pseudo/password rentré
// Si retourne 1 : identifiants corrects
// Si retourne 0 : mauvais identifiants
// Sinon erreur
void connexion_utilisateur(MYSQL *con)
{
    struct Utilisateur user;
    char request[500];
    char mot_de_passe[60];
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
			user_password = BINARY '%s';",
            user.pseudo, mot_de_passe);

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
            correspondance = atoi(row[i]); // conversion entier
        }
    }
    /* -- Conditions -- */
    if (correspondance == 0)
    {
        printf("%sInformations de connexion erronées. Veuillez réessayer !\n%s", COLOR_RED, COLOR_RESET);
        effacer_console(TIME);
        connexion_utilisateur(con);
    }
    else if (correspondance == 1)
    {
        printf("%sConnexion réussie\n%s", COLOR_YELLOW, COLOR_RESET);
        effacer_console(TIME);
        char *statut = get_status(con, user); // on récupère le statut de l'utilisateur
        menus_connexion(statut, con, user);   // on affiche le menu correspondant à son statut
    }
    else
    {
        printf("Erreur : plus d'une personne correspond à ces critères.\n");
    }
    mysql_free_result(result);
}

// Ajoute un utilisateur dans la base de donnée après avoir crée son profil
void add_user_database(MYSQL *con)
{
    char request[1000];
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
    if (strcmp(utilisateur.statut, "Eleve") == 0)
    {
        ajouter_classe(con, utilisateur);
        modifier_groupe(con, utilisateur);
        ajouter_promo(con, utilisateur);
    }

    else if (strcmp(utilisateur.statut, "Enseignant") == 0)
    {
        effacer_console(0);
        assignation_matiere(con, utilisateur);
        assignation_classe(con, utilisateur);
    }

    effacer_console(0);
    afficher_pseudo(con, utilisateur);
    printf("\n%s %s %s a bien été ajouté.e à la base de données !\n\n%s", COLOR_YELLOW, utilisateur.prenom, utilisateur.nom, COLOR_RESET);
}

// Modifie le groupe si utilisateur est un élève
void modifier_groupe(MYSQL *con, struct Utilisateur user)
{
    char request[500];
    int id;
    printf("Entrer le groupe : ");
    scanf("%d", &user.groupe);

    id = get_id(con, user);

    /* -- Ajout pseudo à la bdd -- */
    sprintf(request, "UPDATE Utilisateurs SET user_groupe = %d WHERE user_id = %d", user.groupe, id);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        // return;
    }
}

// Supprime les utilisateurs de la bdd à partir de l'id fourni
int supprimer_users(MYSQL *con)
{
    int indice;
    char request[50];
    printf("A partir de quel id voulez-vous supprimer les élèves ?");
    scanf("%d", &indice);
    sprintf(request, "DELETE FROM Utilisateurs WHERE user_id > %d", indice);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return (1);
    }

    /* Reinitialisation des user_id au nombre voulu */
    sprintf(request, "ALTER TABLE Utilisateurs AUTO_INCREMENT = %d", indice);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        return (1);
    }
    printf("Utilisateurs supprimés\n");
    return (1);
}

// Modifie le pseudo généré par défault en SQL (" ") par celui sous la forme {prenom.nom}
void modifier_pseudo(MYSQL *con, struct Utilisateur user)
{
    char request[500];

    /* -- Ajout pseudo à la bdd -- */
    sprintf(request, "UPDATE Utilisateurs SET user_pseudo = concat(lower(user_prenom),'.', \
			lower(user_nom)) WHERE user_nom = '%s' AND user_prenom ='%s';",
            user.nom, user.prenom);
    if (mysql_query(con, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
}
