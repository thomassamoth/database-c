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
    user_pseudo VARCHAR(30) NOT NULL DEFAULT '' UNIQUE, 
    user_password VARCHAR(30) NOT NULL,
    user_promo INT DEFAULT NULL,
    
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
    -- PRIMARY KEY(id_personne)
    CONSTRAINT fk_classe_id FOREIGN KEY (classe_id) REFERENCES Classe(classe_id) ON DELETE CASCADE,
    CONSTRAINT fk_user_id FOREIGN KEY (id_personne) REFERENCES Utilisateurs(user_id) ON DELETE CASCADE,

    CONSTRAINT pk_personne_classe PRIMARY KEY(id_personne, classe_id) 
)
ENGINE = INNODB;

-- ALTER TABLE Personne_Classe ADD CONSTRAINT pk_personne_classe PRIMARY KEY(id_personne, classe_id);

CREATE TABLE Personne_Matiere ( 
    id_personne INT UNSIGNED NOT NULL,
    matiere_id INT UNSIGNED NOT NULL,
    PRIMARY KEY(id_personne),
    CONSTRAINT fk_matiere_id FOREIGN KEY (matiere_id) REFERENCES Matiere(mat_id) ON DELETE CASCADE
    -- CONSTRAINT fk_user_id FOREIGN KEY (id_personne) REFERENCES Utilisateurs(user_id) ON DELETE CASCADE 
)
ENGINE = INNODB;



CREATE TABLE Bulletin (
    bull_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
    bull_eleve INT UNSIGNED NOT NULL, -- id_eleve
    bull_annee VARCHAR(10) NOT NULL, -- 2021-2022 p.ex
    bull_semestre INT NOT NULL,
    bull_matiere INT UNSIGNED NOT NULL, -- id_matière
    bull_note DECIMAL(15,2),
    bull_appreciation VARCHAR(1000),
    bull_locked BOOLEAN DEFAULT False,
    
    CONSTRAINT fk_bull_id FOREIGN KEY (bull_eleve) REFERENCES Utilisateurs(user_id) ON DELETE CASCADE,
    CONSTRAINT fk_bull_matiere FOREIGN KEY (bull_matiere) REFERENCES Matiere(mat_id) ON DELETE CASCADE,
    
    PRIMARY KEY(bull_id)
)
ENGINE = INNODB;
--
-- Insertion de valeurs dans les tables :
--

INSERT INTO Utilisateurs
VALUES
    (1, 'Admin', 'Admin', 'Secretariat', 'admin', 'admin', NULL),
    (2, 'Martin', 'Jean', 'Eleve', 'jean.martin', 'jean', 2025),
    (3, 'Prof', 'Prof', 'Enseignant', 'prof', 'prof', NULL);

INSERT INTO Classe
VALUES 
    (1, 'Mauve'),
    (2, 'Bleue'),
    (3, 'Ivoire'),
    (4, 'Verte');

INSERT INTO Personne_Classe
VALUES
    (2, 1),
    (3, 1),
    (3, 2),
    (3, 3),
    (3, 4);
    
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

INSERT INTO Personne_Matiere
VALUES
    (3, 6);

INSERT INTO Bulletin
VALUES
    (1, 2, '2019-2020', 1, 1, 17.08, "Excellent travail",  False),
    (2, 2, '2020-2021', 1, 2, 19.73, "Travail remarquable",  False);

-- VERROUILLER BULLETINS
-- UPDATE Bulletin SET bull_locked = False WHERE bull_id = 1;


-- AFFICHER BULLETINS
-- SELECT user_nom, user_prenom, mat_nom, bull_note, bull_appreciation  FROM Bulletin INNER JOIN Utilisateurs ON bull_eleve = user_id INNER JOIN Matiere on mat_id = bull_matiere;

-- AFFICHAGE DES CLASSES DONT LE PROF EST REPONSABLE : 
-- SELECT  user_nom, user_prenom, classe_nom  FROM Utilisateurs  INNER JOIN Personne_Classe as persCla ON id_personne = user_id INNER JOIN Classe as cla on cla.classe_id = persCla.classe_id WHERE user_statut = "Enseignant" ORDER BY user_nom;
