--
-- Création de la base de données :
--
DROP DATABASE IF EXISTS Esigelec;

CREATE DATABASE Esigelec CHARACTER SET 'utf8';

USE Esigelec;

--
-- Création des tables :
--

CREATE TABLE Matiere (
    mat_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
    mat_nom VARCHAR(30) NOT NULL,
    
    PRIMARY KEY(mat_id)
)
ENGINE = INNODB;

CREATE TABLE Utilisateurs (
    user_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
    user_nom VARCHAR(30) NOT NULL,
    user_prenom VARCHAR(30) NOT NULL,
    user_statut VARCHAR(30) NOT NULL,
    user_pseudo VARCHAR(30) NOT NULL DEFAULT '',
    user_password VARCHAR(30) NOT NULL,
    
    PRIMARY KEY(user_id)
)
ENGINE = INNODB;


CREATE TABLE Classe (
    classe_id INT UNSIGNED NOT NULL,
    classe_nom VARCHAR(10) NOT NULL,
    
    PRIMARY KEY(classe_id)
)
ENGINE = INNODB;

-- quelle élève est dans quelle classe OU quel professeur enseigne dans quelle classe
CREATE TABLE Personne_Classe ( 
    id_personne INT UNSIGNED NOT NULL,
    classe_id INT UNSIGNED NOT NULL,
    PRIMARY KEY(id_personne),
    CONSTRAINT fk_classe_id FOREIGN KEY (classe_id) REFERENCES Classe(classe_id) ON DELETE CASCADE,
    CONSTRAINT fk_user_id FOREIGN KEY (id_personne) REFERENCES Utilisateurs(user_id) ON DELETE CASCADE 
)
ENGINE = INNODB;

CREATE TABLE Bulletin (
    bull_annee VARCHAR(10) NOT NULL, -- 2021-2022 p.ex
    bull_semestre INT NOT NULL
)
ENGINE = INNODB;
--
-- Insertion de valeurs dans les tables :
--

INSERT INTO Utilisateurs
VALUES 
    (1, 'Admin', 'Admin', 'Secretariat', 'admin', 'admin'),
    (2, 'Beyet', 'Thomas', 'Eleve', 'thomas.beyet', 'thomas');

INSERT INTO Classe
VALUES 
    (1, 'Mauve'),
    (2, 'Bleue'),
    (3, 'Ivoire'),
    (4, 'Verte');

INSERT INTO Personne_Classe
VALUES
    (2, 1);
    
INSERT INTO Matiere 
VALUES
    (1, 'Algebre'), 
    (2, 'Analyse'),
    (3, 'Electromagnetisme'),
    (4, 'Thermodynamique'), 
    (5, 'SI'), 
    (6, 'Informatique'), 
    (7, 'Algorithmique'), 
    (8, 'Anglais'), 
    (9, 'Communication'), 
    (10, 'Espagnol'), 
    (11, 'Allemand'), 
    (12, 'Francais'), 
    (13, 'Chinois'); 

	
