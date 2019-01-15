#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define TAM 25600

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void color(char color[]);

void color(char color[])
{
	if( !strcmp( "rojo", color ) ) {
		printf("%s", KRED);
	}else if( !strcmp( "verde", color ) ){
		printf("%s", KGRN);
	}else if( !strcmp( "amarillo", color ) ){
		printf("%s", KYEL);
	}else if( !strcmp( "azul", color ) ){
		printf("%s", KBLU);
	}else if( !strcmp( "magenta", color ) ){
		printf("%s", KMAG);
	}else if( !strcmp( "cyan", color ) ){
		printf("%s", KCYN);
	}else if( !strcmp( "blanco", color ) ){
		printf("%s", KWHT);
	}else if( !strcmp( "normal", color ) ){
       printf("%s", KNRM);
	}
}


void escribirDatta(char contenido[]);

void crearSocketClienteSinConexion(int newsockfdPrincipal, char *ip, int puerto );

int main( int argc, char *argv[] ) {
	int sockfd, puerto, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int terminar = 0;
	char buffer[TAM];

	//Control de errores por cantidad de argumentos
	if ( argc < 3 ) {
		fprintf( stderr, "Uso %s host puerto\n", argv[0]);
		exit( 0 );
	}
	
	puerto = atoi( argv[2] );

	//Abrir el socket
	sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sockfd < 0 ) {
		perror( "ERROR apertura de socket" );
		exit( 1 );
	}		


	//Asociar informacion en el socket
	server = gethostbyname( argv[1] );
	if (server == NULL) {
		fprintf( stderr,"Error, no existe el host\n" );
		exit( 0 );
	}
	memset( (char *) &serv_addr, '0', sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length );
	serv_addr.sin_port = htons( puerto );
	if ( connect( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr ) ) < 0 ) {
		perror( "conexion" );
		exit( 1 );
	}

	color("azul");
	printf("   -----------------------------------\n");
	printf("    Conectado a la AWS numero ");
	color("magenta");
	printf("%d\n",puerto-6020);
	color("azul");
	printf("   -----------------------------------\n");

	while(1) {
		color("rojo");
		printf( "   >" );
		color("blanco");
		memset( buffer, '\0', TAM );
		fgets( buffer, TAM-1, stdin );						//1 ESPERA comando de entrada por teclado.

		n = write( sockfd, buffer, strlen(buffer) );		//2 ENVIO comando a la aws.
		if ( n < 0 ) {
			perror( "escritura de socket" );
			exit( 1 );
		}

		// Si se escribió el comando disconnect, avisar que al final se debe terminar la conexion
		buffer[strlen(buffer)-1] = '\0';
		if( !strcmp( "disconnect", buffer ) ) {
			terminar = 1;
		}

		// Leer socket
		memset( buffer, '\0', TAM );
		n = read( sockfd, buffer, TAM );				// 6 ESPERA y RECIBE
		if ( n < 0 ) {
			perror( "lectura de socket" );
			exit( 1 );
		}

		// Imprimir respuesta de todos los comandos, excepto cuando esta respuesta es: Socket Sin Conexion Creado
		color("verde");
		if( strcmp( "Socket Sin Conexion Creado", buffer ) )
			printf( "    Respuesta: %s\n", buffer );

		// Si se creó el socket server UDP, crear socket cliente UDP para recibir datos del registro
		if( !strcmp( "Socket Sin Conexion Creado", buffer ) ) {
			crearSocketClienteSinConexion(sockfd, argv[1], puerto+25);
		//Sino, cualquier otra respuesta es interpretada como un comando invalido
		}else if( !strcmp( "Comando invalido.", buffer ) ){
			printf( "    Ud. puede utilizar los siguientes comandos:\n");
			printf( "			get_telemetry para obtener el ultimo registro.\n");
			printf( "			get_datta para obtener el archivo de registros.\n");
			printf( "			erase_datta para borrar el archivo de registros.\n");
			printf( "			disconnect para desconectarse de esta AWS.\n");
		}

		// Si se envió el comando disconnect, desconectarse del servidor
		if( terminar ) {
			exit(0);
		}
	}
	return 0;
} 






void crearSocketClienteSinConexion( int newsockfdPrincipal, char *ip, int puerto ) {
	int sockfd, n, tamano_direccion;
	struct sockaddr_in dest_addr;
	struct hostent *server;
	char buffer[TAM];

	// Obtener host
	server = gethostbyname( ip );
	if ( server == NULL ) {
		fprintf( stderr, "ERROR, no existe el host\n");
		exit(0);
	}

	// Creacion del socket
	sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
	if (sockfd < 0) {
		perror( "apertura de socket" );
		exit( 1 );
	}

	// Creacion de la estructura dest_addr Direccion de destino
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons( puerto );
	dest_addr.sin_addr = *( (struct in_addr *)server->h_addr );
	memset( &(dest_addr.sin_zero), '\0', 8 );
	
	//Avisar que ya se creó el socket cliente y que esta listo para recibir los datos del registro
	strcpy(buffer,"Enviame los datos\0");
	tamano_direccion = sizeof( dest_addr );
	n = sendto( sockfd, (void *)buffer, TAM, 0, (struct sockaddr *)&dest_addr, tamano_direccion );
	if ( n < 0 ) {
		perror( "Escritura en socket" );
		exit( 1 );
	}

	//Esperar a que el servidor sin conexion envíe los datos del registro
	memset( buffer, 0, TAM );
	n = recvfrom( sockfd, (void *)buffer, TAM, 0, (struct sockaddr *)&dest_addr, &tamano_direccion );
	if ( n < 0 ) {
		perror( "Lectura de socket" );
		exit( 1 );
	}

	//Imprimir los datos recibidos para debuggear
	//printf("Los datos recibidos son:<%s>",buffer);

	//Escribir datos del registro en archivo datta.csv
	escribirDatta(buffer);

	//Cerrar socket
	close(sockfd);
	printf("    Registro de datos, recibido y guardado en el archivo datta.csv\n\n");

	//return 0;
} 

void escribirDatta(char buffer[])
{
	remove("datta.csv");
    FILE *file;
    file = fopen("datta.csv", "w");
    if(file == NULL)
        printf("Error al abrir el archivo");
    fprintf(file, "%s", buffer);
    fclose(file);      
}
