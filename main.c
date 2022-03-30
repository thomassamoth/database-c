#include <mysql/mysql.h>
#include<stdlib.h>
#include <stdio.h>
char host[15] = "localhost";
char user[15] = "esigelec";
char password[15] = "esigelec";


struct Eleve
{
    char nom [30] ;
    char prenom [30] ;
    char promo [4] ;
    char classe [10] ;
};

struct Eleve saisir_eleve() //fonction permettant de saisir un nouveau joueur
{
    struct Eleve student;
    fflush(stdin);
    printf("Veuillez entrer le nom de \n");
    fgets(student.nom, 30, stdin);

    return student;
}

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

static MYSQL *con;
int main()
{
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

    //saisir_eleve();

    /*if (mysql_query(con, "INSERT INTO Eleves(el_nom,el_prenom,el_promo, el_classe) VALUES('tata','tata', '2025', 'Mauve')"))
    {
		fprintf(stderr, "%s\n", mysql_error(con));
		return(1);
    }*/

    if (mysql_query(con, "DELETE FROM Eleves WHERE el_prenom = 'tata';"))
    {
		fprintf(stderr, "%s\n", mysql_error(con));
		return(1);
    }

    // Lecture des données
    if (mysql_query(con, "SELECT * FROM Eleves;"))
    {
        return(1);
    }

    MYSQL_RES *result = mysql_store_result(con);
    printf("Result : %s ",result);

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
