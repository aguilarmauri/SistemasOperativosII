#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define TAM 25600
#include <time.h>
//Para semaforos
#include <sys/ipc.h>
#include <sys/sem.h>
//Para HTML
#include "funcionesHTML.h"

// Cada 1 segundo, leer la información desde los sensores
// y escribirla en el registro, mientras los sensores entreguen
// informacion

int main( int argc, char *argv[] ) {
	
	char linea[40];
	FILE *sensores, *registro;
	// Para semáforo
	key_t Llave;
	int semaforo;
	struct sembuf Proceso;
	int i, intentos;
	i=0;
	Llave = ftok("registro.csv",33);
	//par: llave, cant sem, permisos y crear sem si no Ex
	semaforo = semget(Llave,1,0600 | IPC_CREAT);
	//par: sem a inicializar, id del semaforo en el arreglo, SETVAL para inicializar, 0 valor inicial del sem.
	semctl(semaforo, 0, SETVAL, 1);
	Proceso.sem_num = 0; // Número de semáforo a modificar
	Proceso.sem_flg = 0; //

	//De manera indefinida
	intentos = 0;
	while(1){

		//Abrir archivo de los sensores en modo lentura
		sensores = fopen("sensores.csv","r");
		if(sensores == NULL){
			printf("Error al abrir el archivo de sensores\n");
			intentos++;
			if(intentos > 10){
				printf("Demasiados intentos de lectura de sensores.\nSe requiere mantenimiento urgente.");
				break;
			}
		}
		else{
			printf( "Los sensores se conectaron correctamente para lectura\n");

			// Mientras encuentre lineas en el archivo de sensores
			while(fgets(linea,40,sensores)!=NULL){

				// Solicitar semáforo para registro.csv
				Proceso.sem_op = -1; // Accion que va a tomar. -1 decrementa
				semop(semaforo, &Proceso, 1); //Crea las aciones del sem. 3par: número de estructura a enviar

				// Abrir archivo de los registros en modo escritura
				registro = fopen("registro.csv", "a");

				// Si no se pudo abrir o no existia, mostrar mensaje de advertencia
				if(registro == NULL){
					printf("Error al abrir el archivo de registro\n");
				}

				// Si la apertura fue exitosa, escribir una linea en el registro
				else{
					printf("%s\n",linea);
					fprintf(registro, "%s", linea);

					// Cerrar archivo registro para que lo pueda usar el servidor
					close( registro );

					// Liberar buffer de registro
					fflush(registro);
				}

				// Liberar semáforo
				Proceso.sem_op = 1; // Accion que va a tomar. +1 decrementa
				semop(semaforo, &Proceso, 1); //Crea las aciones del sem. 3par: número de estructura a enviar

				// Esperar 1 segundo antes de escribir la siguiente linea
				sleep(1);

			}
		}

		// Cerrar sensores
		close( sensores );

	printf("Reiniciando sensores...\n");
	}

	return 0; 
}