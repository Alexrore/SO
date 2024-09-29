drop database if exists Juego;
create database Juego;
use Juego;
CREATE TABLE PartidasGanadas (
nombre VARCHAR(25), victorias int);

INSERT INTO PartidasGanadas VALUES('Alejandro',3);
INSERT INTO PartidasGanadas VALUES('Aaron',2);
INSERT INTO PartidasGanadas VALUES('Arnau',0);
INSERT INTO PartidasGanadas VALUES('Carla',1);