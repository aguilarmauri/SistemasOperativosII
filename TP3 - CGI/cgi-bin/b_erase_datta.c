#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#define TAM 25600
//Para semaforos
#include <sys/ipc.h>
#include <sys/sem.h>
//Para HTML
#include "funcionesHTML.h"

int main( int argc, char *argv[] ) {
	char *seccion = "Erase_Datta         ";
	imprimirHEADER(seccion);

	int pid;
	int n;

	// Para semáforo
	key_t Llave;
	int semaforo;
	struct sembuf Proceso;
	int i;
	i=0;
	Llave = ftok("registro.csv",33);
	//par: llave, cant sem, permisos y crear sem si no Ex
	semaforo = semget(Llave,1,0600 | IPC_CREAT);
	//par: sem a inicializar, id del semaforo en el arreglo, SETVAL para inicializar, 0 valor inicial del sem.
	semctl(semaforo, 0, SETVAL, 1);
	Proceso.sem_num = 0; // Número de semáforo a modificar
	Proceso.sem_flg = 0; //

	//Archivo de registros
	char linea[40];
	FILE *archivo_registros;

	// Pedir semaforo para abrir registro.csv
	Proceso.sem_op = -1; // Accion que va a tomar. -1 decrementa
	semop(semaforo, &Proceso, 1); //Crea las aciones del sem. 3par: número de estructura a enviar

	int eliminacion;
	eliminacion = remove("/var/www/cgi-bin/registro.csv" );
	//eliminacion = unlink("/var/www/registro.csv" );
	//eliminacion = system("/bin/rm /var/www/registro.csv");
	//system("/bin/rm /var/www/registro.csv 2> errorErase");
	if( eliminacion != 0 ){
		printf("Error borrando archivo.<br>");
		printf("Error: %d", eliminacion);
	}
	else{
		archivo_registros = fopen("registro.csv","a+");
		fclose(archivo_registros);
		printf( "Archivo de registro eliminado.<br>");
	}
	
	// Liberar semáforo de registro.csv, Aparición 2/2
	Proceso.sem_op = 1; // Accion que va a tomar. +1 incrementa
	semop(semaforo, &Proceso, 1); //Crea las aciones del sem. 3par: número de estructura a enviar
	
	imprimirFOOTER();
	fflush(stdout);
	return 0;
}