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
	char *seccion = "Get_Datta           ";
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


	//Abrir archivo en modo lectura
	archivo_registros = fopen("registro.csv","r");

	//Si el archivo no existe, crearlo
	if(archivo_registros == NULL){
		archivo_registros = fopen("registro.csv","a+");
		fclose(archivo_registros); //TIRA ERROR

		// Liberar semáforo de registro.csv, Aparición 1/2
		Proceso.sem_op = 1; // Accion que va a tomar. +1 incrementa
		semop(semaforo, &Proceso, 1); //Crea las aciones del sem. 3par: número de estructura a enviar

		printf("El archivo no existe. Fue creado.");
	
	//Si el archivo existe
	}else{
		//Imprimir linea por linea
		while(fgets(linea,40,archivo_registros)){
			if( strcmp( "", linea ) )
				printf("%s<br>", linea);
		}

		//Cerrar archivo
		fclose(archivo_registros);

		// Liberar semáforo de registro.csv, Aparición 2/2
		Proceso.sem_op = 1; // Accion que va a tomar. +1 incrementa
		semop(semaforo, &Proceso, 1); //Crea las aciones del sem. 3par: número de estructura a enviar

	}
	
	imprimirFOOTER();
	return 0;
}