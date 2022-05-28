#ifndef __MAIN_H__
#define __MAIN_H__

#include <mysql/mysql.h>
#include<stdlib.h>
#include <stdio.h>
#include<string.h>
#include <unistd.h> //pause
#include <time.h>
#include <stdio_ext.h> //fpurge


// Enregistrements
struct Utilisateur
{
    char nom [30];
    char prenom[30];
    char pseudo[30];
    char password[30];
    char statut[30]; // permissions
    int groupe;
};

//== COLORS ==
#define COLOR_RED     "\x1b[91m"
#define COLOR_CYAN    "\x1b[96m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_MAGENTA "\x1b[95m"
#define COLOR_WHITE   "\x1B[97m"
#define COLOR_YELLOW  "\x1B[93m"
#define COLOR_GREEN   "\x1B[92m"

#define TIME 2


#endif
