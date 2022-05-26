# 🎓 Bulletins - C | MySQL

### Projet de gestion des bulletins 📜
Écrire en C un programme permettant au secrétariat du CPII de gérer les
bulletins.  
Le secrétariat doit pouvoir saisir les informations suivantes :
- Les élèves (nom, prénom, groupe,...),
- Les enseignants (nom, prénom, discipline, ...)  
 
Le logiciel permettra à un enseignant d'écrire une appréciation pour chaque
élève des groupes dont il est responsable, uniquement dans la matière où il
enseigne.   
#### Fonctionnalités qui doivent être proposées aux utilisateurs :
- [X] Un enseignant peut consulter les appréciations écrites par les autres enseignants,
- [X] Un enseignant peut consulter les anciens bulletins (s'il y en a),
- [X] Un enseignant peut effectuer la recherche d'un élève (nom et prénom)
et afficher son bulletin,
- [X] Tant que les bulletins n'ont pas été validés par le secrétariat, il est
possible à un enseignant de modifier ses appréciations, ...

### 📝 Autres fonctionnalités 
- [X] Un élève peut consulter ses bulletins (année par année / semestre par semestre)
- [X] Un utilisateur peut se connecter et accéder aux fonctionnalités disponibles pour son statut
- [X] Une personne du secrétariat peut créer un compte  

### Utilisation  
 - Executer le script SQL avec `source esigelec.sql`  
 - Compiler le projet : `make`
