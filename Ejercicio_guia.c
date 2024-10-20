#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>

void *AtenderCliente (void *socket)
{
	int sock_conn;
	int *s;
	s = (int *) socket;
	sock_conn = *s;
	
	// Declaramos variables para manejar la petición y la respuesta
	char peticion[512];
	char respuesta[512];
	int ret;
	
	// Variable para controlar cuando finalizar la conexión con el cliente
	int terminar = 0;
	
	// Bucle para atender al cliente hasta que solicite desconexión
	while (terminar == 0)
	{
		// Recibimos la petición del cliente
		ret = read(sock_conn, peticion, sizeof(peticion));
		printf("Petición recibida\n");
		
		// Añadimos el carácter nulo al final de la cadena para evitar basura en el buffer
		peticion[ret] = '\0';
		
		printf("Contenido de la petición: %s\n", peticion);
		
		// Desglosamos la petición en diferentes partes
		char *p = strtok(peticion, "/");
		int codigo = atoi(p); // Obtenemos el código de la petición
		
		// Declaración de variable para las conversiones de temperatura
		float temperatura;
		
		if (codigo != 0)
		{
			// Si la petición es válida, obtenemos el valor de la temperatura
			p = strtok(NULL, "/");
			temperatura = atof(p);
			printf("Código: %d, Temperatura: %f\n", codigo, temperatura);
		}
		
		if (codigo == 0) // Si el código es 0, el cliente solicita desconectar
		{
			terminar = 1;
		}
		else if (codigo == 1) // Si el código es 1, el cliente pide convertir Celsius a Fahrenheit
		{
			temperatura = (temperatura * (9.0 / 5.0)) + 32;
			sprintf(respuesta, "%f/n", temperatura);
		}
		else if (codigo == 2) // Si el código es 2, el cliente pide convertir Fahrenheit a Celsius
		{
			temperatura = (temperatura - 32) * (5.0 / 9.0);
			sprintf(respuesta, "%f/n", temperatura);
		}
			
		if (codigo != 0) // Si no es desconexión, enviamos la respuesta al cliente
		{
			printf("Enviando respuesta: %s\n", respuesta);
			write(sock_conn, respuesta, strlen(respuesta));
		}
	}
	// Cerramos la conexión del cliente al terminar
	close(sock_conn); 
}

int main(int argc, char *argv[]) 
{
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	
	// INICIALIZACIONES
	// Creamos el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error al crear el socket");
	
	// Preparamos la estructura de la dirección del servidor
	memset(&serv_adr, 0, sizeof(serv_adr)); // Inicializamos a cero la estructura
	serv_adr.sin_family = AF_INET;
	
	// Asociamos el socket a cualquier IP disponible en la máquina
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// Definimos el puerto de escucha
	serv_adr.sin_port = htons(9050);
	
	// Hacemos el bind del socket para asociarlo al puerto
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf("Error en el bind");
	
	// Preparamos el socket para escuchar conexiones
	if (listen(sock_listen, 3) < 0)
		printf("Error en el listen");
	
	int i;
	int sockets[100];
	pthread_t thread;
	i = 0;
	
	// Bucle principal para aceptar y atender nuevas conexiones de clientes
	for (;;)
	{
		printf("Esperando conexiones\n");
		
		// Aceptamos la conexión entrante
		sock_conn = accept(sock_listen, NULL, NULL);
		printf("Conexión recibida\n");
		
		// Guardamos el socket del cliente
		sockets[i] = sock_conn;
		
		// Creamos un hilo para atender a este cliente
		pthread_create(&thread, NULL, AtenderCliente, &sockets[i]);
		i = i + 1;
	}
}
