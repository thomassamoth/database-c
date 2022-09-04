# 🎓 Bulletins - C | MySQL

![GitHub release (latest by date)](https://img.shields.io/github/v/release/thomassamoth/database-c?label=version)  
Écrire en C un programme permettant au secrétariat du CPII de gérer les
bulletins.  
Le secrétariat doit pouvoir saisir les informations suivantes :

- Les élèves (nom, prénom, groupe,...)
- Les enseignants (nom, prénom, discipline, ...)  

Le logiciel permettra à un enseignant d'écrire une appréciation pour chaque
élève des groupes dont il est responsable, uniquement dans la matière où il
enseigne.

## Fonctionnalités qui doivent être proposées aux utilisateurs

- [X] Un enseignant peut consulter les appréciations écrites par les autres enseignants.
- [X] Un enseignant peut consulter les anciens bulletins (s'il y en a).
- [X] Un enseignant peut effectuer la recherche d'un élève (nom et prénom)
et afficher son bulletin.
- [X] Tant que les bulletins n'ont pas été validés par le secrétariat, il est
possible à un enseignant de modifier ses appréciations, ...

## 📝 Autres fonctionnalités

- [X] Un élève peut consulter ses bulletins (année par année / semestre par semestre).
- [X] Un utilisateur peut se connecter et accéder aux fonctionnalités disponibles pour son statut.
- [X] Une personne du secrétariat peut créer un compte.

## :computer: Utilisation

### Prérequis

- Installer la librairie MySQL avec :

```bash
sudo apt install default-libmysqlclient-dev
```

- Installer la librairie [JSON-C](https://github.com/json-c/json-c) : 

```bash
sudo apt install libjson-c-dev
```

- Après connexion au serveur SQL, **exécuter** le script SQL :

``` sql
source esigelec.sql
```  

- Renommer `credentials-template.json` en `credentials.json` et modifier les identifiants.  

### :rocket: Compilation

- Pour compiler le projet, il suffit d'exécuter :

``` bash
make
```
