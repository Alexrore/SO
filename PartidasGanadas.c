//Programa en C para crear una base de datos
	//Incluir esta libreri\uffada para poder hacer las llamadas en shiva2.upc.es
//#include <my_global.h> 
#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
	int main(int argc, char **argv)
{
	//Conector para acceder al servidor de bases de datos 
		MYSQL *conn;
	int err;
	//Creamos una conexion al servidor MYSQL
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion, indicando nuestras claves de acceso
	// al servidor de bases de datos (user,pass)
	conn = mysql_real_connect (conn, "localhost","root", "mysql", NULL, 0, NULL, 0);
	if (conn==NULL)
	{
		printf ("Error al inicializar la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	// Creamos la base de datos del juego
	mysql_query(conn, "drop database if exists Juego;"); 
	err=mysql_query(conn, "create database Juego;");
	if (err!=0) {
		printf ("Error al crear la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//indicamos la base de datos con la que queremos trabajar 
	err=mysql_query(conn, "use Juego;");
	if (err!=0)
	{
		printf ("Error al crear la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	// creamos la tabla que define la entidad PartidasGanadas que incluye nombre y victorias
		err=mysql_query(conn,"CREATE TABLE PartidasGanadas (nombre VARCHAR(25), victorias int)");
		if (err!=0)
	{
			printf ("Error al definir la tabla %u %s\n", mysql_errno(conn), mysql_error(conn));
			exit (1);
	}
		// ahora tenemos la base de datos lista en el servidor de MySQL
		// cerrar la conexion con el servidor MYSQL 
		mysql_close (conn);
		exit(0);
}
