#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
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

void crearSocketServidorSinConexion(int sockfdPrincipal, int puerto);



int main( int argc, char *argv[] ) {
	int sockfd, newsockfd, puerto, clilen, pid;
	char buffer[TAM];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	//Archivo de registros
	char linea[40];
	FILE *archivo_registros;

	//Comprueba cantidad de argumentos
	if ( argc < 2 ) {
		color("rojo");
        fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
        color("blanco");
		exit( 1 );
	}

	//Control de errores por verificacion de numero de puerto
	if( atoi(argv[1])<6020 || atoi(argv[1])>6044){
		color("rojo");
		printf("    SRV> Error de puerto. Puertos válidos: [6020,6044]\n");
		color("blanco");
		exit( 1 );
	}


	//Creacion del socket principal
	sockfd = socket( AF_INET, SOCK_STREAM, 0);
	if ( sockfd < 0 ) { 
		color("rojo");
		perror( "    SRV> Error al realizar la apertura del socket servidor. \nPrograma finalizado.");
		color("blanco");
		exit( 1 );
	}
	
	//Creacion y definicion de la estructura serv_addr
	memset( (char *) &serv_addr, 0, sizeof(serv_addr) ); //Llena de ceros todo el espacio de serv_addr
	puerto = atoi( argv[1] );	//convierte el puerto a un entero
	serv_addr.sin_family = AF_INET; //Define como tipo de direccion almacenada en sa_data, a la familia de protocolo internet
	serv_addr.sin_addr.s_addr = INADDR_ANY; //Se asocia un lugar para almacenar cualquier direccion IP
	serv_addr.sin_port = htons( puerto );	//Se asocia el numero de puerto, htons convierte el entero sin signo: puerto, al "orden de bytes de la red" ¿?

	//Asociacion del socket con la estructura
	if ( bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) { //Asocia el socket sockaddr con el puerto de la maquina, dev -1 si error
		color("rojo");
		perror( strcat(strcat("    SRV> Error al asociar el socket del servidor al puerto ","-puerto-")," de la máquina. \nPrograma finalizado.") );
		color("blanco");
		exit( 1 );
	}

	//Imprimir el socket disponible principal
	color("magenta");
    printf( "   SRV> El Servidor %d espera nuevos clientes en el puerto %d\n", getpid(), ntohs(serv_addr.sin_port) );
    color("blanco");

	//Indicar con listen que el socket principal está listo para aceptar conexiones entrantes
	listen( sockfd, 5 );
	clilen = sizeof( cli_addr );

	while( 1 ) { //Iteración del proceso padre: acepta conexiones y delega a un hijo

		color("blanco");
		//Aceptar la primer conexion entrante en la cola de entrada o esperar por una (se bloquea hasta que llegue una) : accept()
		newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );
		if ( newsockfd < 0 ) {
			perror( "    SRV> Error al aceptar la conexión, desde el servidor. \nPrograma finalizado." );
			exit( 1 );
		}

		//Crear un proceso hijo para que atienda a este cliente recien aceptado
		pid = fork(); 
		if ( pid < 0 ) {
			perror( "    SRV> Error al crear proceso hijo en el servidor" );
			exit( 1 );
		}

		// Se crea el proceso hijo
		// Se ejecuta primero el padre, que imprime un mensaje avisando qué hijo va a atender al cliente aceptado, y se desliga del socket cliente.
		// Y como el padre termina su ejecución estando adentro de un while(1), se vuelve a ejecutar y se bloquea en el accept() a esperar un nuevo hijo
		// Luego se ejecuta el hijo, que atiende al cliente recien aceptado.
		if ( pid == 0 ) {  // Proceso hijo
			//El hijo libera el socket principal
			close( sockfd );

			//Abrir archivo en modo lectura para este hijo
			char comandoAnterior;

			while ( 1 ) {	//Iteración del proceso hijo: recibe y responde
				//Limpiar buffer			
				memset( buffer, 0, TAM );

				//Leer el mensaje enviado por el cliente o esperar por uno
				n = read( newsockfd, buffer, TAM-1 );			// 3 ESPERA y RECIBE / 5 ESPERA
				if ( n < 0 ) {
					perror( "    SRV> Error en la lectura del socket cliente, desde el servidor.\nPrograma finalizado." );
					exit(1);
				}

				//Imprimir mensaje recibido
				printf( "\n   SRV> Proceso %d, recibió: ", getpid() );

				color("amarillo");
				printf( "%s", buffer);
				color("blanco");

				//Identificar Comandos y Responder a este cliente
				buffer[strlen(buffer)-1] = '\0';
				color("blanco");
				if( !strcmp( "disconnect", buffer ) )
					n = write( newsockfd, "Desconectado.", 14 );	//4 RESPONDE
				else if( !strcmp( "get_telemetry", buffer ) ){

					//Abrir archivo en modo lectura para este hijo
					archivo_registros = fopen("registro.csv","r");
					//Si el archivo no existe, crearlo
					if(archivo_registros == NULL){
						archivo_registros = fopen("registro.csv","a+");
						fclose(archivo_registros);
						printf("     SRV> Error al abrir archivo registro.csv. El archivo no existe. Fue creado.");
						n = write( newsockfd, "Archivo vacío.\n", 17 );	//4 RESPONDE
						printf( "     SRV> Se avisó que el archivo esta vacío.\n");
					//Si el archivo existe
					}else{
						//Leer la ultima linea
						while(fgets(linea,40,archivo_registros)){}
						if( !strcmp( "", linea ) ){
							n = write( newsockfd,"archivo vacío\n", 17 );	//4 RESPONDE
							printf( "     SRV> Se avisó que el archivo esta vacío.\n");
						}else{
							//Enviar ultima linea
							n = write( newsockfd, linea, 40 );	//4 RESPONDE
							printf( "     SRV> La última línea del registro fue enviada correctamente.\n");
						}
					}

					//Cerrar archivo
					fclose(archivo_registros);
				}else if( !strcmp( "get_datta", buffer ) ){
					//printf( "   SRV>  Comando get_datta\n");					
					crearSocketServidorSinConexion(newsockfd, /*argv[1],*/(puerto+25));
					//n = write( newsockfd, "Archivo transmitido.", 21 );	//4 RESPONDE
				}else if( !strcmp( "erase_datta", buffer ) ){
					printf( "     SRV> Borrando archivo de registro...\n");
					color("rojo");
					remove("registro.csv");
					archivo_registros = fopen("registro.csv","a+");
					fclose(archivo_registros);
					printf( "     SRV> Archivo de registro eliminado.\n");
					color("blanco");
					n = write( newsockfd, "Archivo de registros eliminado.\n", 32 );	//4 RESPONDE
				}else if( !strcmp( "shutdown", buffer ) ){
					color("amarillo");
					printf( "     SRV> Apagando servidor...\n");
					color("blanco");
					n = write( newsockfd, "Apagando servidor.\n", 19 );	//4 RESPONDE
					break;
				}
				else
					n = write( newsockfd, "Comando invalido.", 18 );	//4 RESPONDE
				
				//Ver que pasa con el n en el ultimo else
				if ( n < 0 ) {
					color("rojo");
					perror( "     SRV> Error al responder al cliente.\nPrograma finalizado." );
					color("blanco");
					exit( 1 );
				}

				// Verificación de si hay que terminar
				if( !strcmp( "disconnect", buffer ) ) {
					color("azul");
					printf( "    SRV> El Cliente atendido por el proceso %d se desconecta del sistema.\n\n", getpid() );
					color("blanco");
					exit(0);
				}
				color("blanco");
			} // end while Hijo
			break;
		}
		else { //Proceso padre
			color("azul");
			printf( "   ------------------------------------------------------------------------------\n" );
			printf( "   SRV> El Nuevo Cliente conectado, será atendido por el proceso " );
			color("magenta");
			printf( "%d\n", pid );
			color("azul");
			printf( "   ------------------------------------------------------------------------------\n" );
			color("blanco");
			close( newsockfd );
		}

	}
	color("rojo");
	printf( "     SRV> Servidor apagado\n" );
	color("blanco");
	close(sockfd);
	close(newsockfd);
	return 0;
} 



void crearSocketServidorSinConexion(int sockfdPrincipal, int puerto/*, int argc, char *argv[] */) {
	int sockfd, /*puerto,*/ tamano_direccion;
	char buffer[ TAM ];
	struct sockaddr_in serv_addr;
	int n;
	
	color("cyan");

	//Crear el socket Internet, sin conexion(permanente) DGRAM UDP
	sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
	if (sockfd < 0) { 
		color("rojo");
		perror("ERROR en apertura de socket");
		color("blanco");
		exit( 1 );
	}

	//Crear y asignar valores a la estructura serv_addr, guardar el puerto
	memset( &serv_addr, 0, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( puerto );
	memset( &(serv_addr.sin_zero), '\0', 8 );

	//Asociar el socket con la estructura
	if( bind( sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr) ) < 0 ) {
		color("rojo");
		perror( "ERROR en binding" );
		color("blanco");
		exit( 1 );
	}

	//Imprimir socket disponible
   	printf( "     SRV UDP> Socket disponible: %d\n", ntohs(serv_addr.sin_port) );

	//Iterar recibiendo y respondiendo
	tamano_direccion = sizeof( struct sockaddr );
		memset( buffer, 0, TAM );

		//Enviar comando get_datta al servidor
		n = write( sockfdPrincipal, "Socket Sin Conexion Creado", 27 );		//2 ENVIA
		if ( n < 0 ) {
			perror( "escritura de socket" );
			exit( 1 );
		}

		//Bloquearse a esperar que cree su socket y le envíe el archivo
		n = recvfrom( sockfd, buffer, TAM-1, 0, (struct sockaddr *)&serv_addr, &tamano_direccion );
		if ( n < 0 ) {
			color("rojo");
			perror( "lectura de socket" );
			color("blanco");
			exit( 1 );
		}
		printf("     SRV UDP> Recibí: %s\n", buffer );

		//Si el cliente esta listo para recibir los datos del archivo, cargar los datos del registro en buffer
		if( !strcmp( "Enviame los datos", buffer ) ){
			//Leer archivo y enviar
			printf( "     SRV UDP> Abriendo archivo a transmitir...\n" );
			FILE *datta;
			datta = fopen("registro.csv", "r");
			if(datta == NULL){
				datta = fopen("registro.csv", "a+");
				close(datta);
				if(datta == NULL)
					printf("El archivo no existe.");
			}

			//Concatenar datos
			char linea[TAM];
			memset( buffer, 0, TAM );
			while(fgets(linea, TAM, datta) != NULL){
				strcat(buffer,linea);
			}
			fclose(datta);
		}
		printf("     SRV UDP> Enviando datos...   ");

		//Enviar datos del registro al cliente sin conexion
		n = sendto( sockfd, (void *)buffer, sizeof(buffer), 0, (struct sockaddr *)&serv_addr, tamano_direccion  );
		if ( n < 0 ) {
			color("rojo");
			perror( "escritura en socket" );
			color("blanco");
			exit( 1 );
		}
		printf("Datos enviados..   ");

		//Cerrar socket servidor sin conexion UDP, para que prosiga el principal: TCP
		close(sockfd);
		printf("Socket Cerrado.\n");
		color("blanco");
	return;
}



