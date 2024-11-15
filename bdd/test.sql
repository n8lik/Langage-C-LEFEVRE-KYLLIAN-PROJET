-- Création de la table utilisateurs
CREATE TABLE utilisateurs (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nom VARCHAR(50) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    date_inscription DATE
);

CREATE TABLE test (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nom VARCHAR(50) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    date_inscription DATE
);

CREATE TABLE tost (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nom VARCHAR(50) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    date_inscription DATE
);
-- Insertion de quelques données
INSERT INTO utilisateurs (nom, email, date_inscription) VALUES
('Alice Dupont', 'alice@email.com', '2024-01-15'),
('Bob Martin', 'bob@email.com', '2024-02-20'),
('Charlie Brown', 'charlie@email.com', '2024-03-10');

