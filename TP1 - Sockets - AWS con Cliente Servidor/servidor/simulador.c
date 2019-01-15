#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define TAM 25600
#include <time.h>

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


// Cada 1 segundo, leer la información desde los sensores
// y escribirla en el registro, mientras los sensores entreguen
// informacion

int main( int argc, char *argv[] ) {
	char linea[40];
	FILE *sensores, *registro;
	color("blanco");

	//De manera indefinida
	while(1){
		//Abrir archivo de los sensores en modo lentura
		sensores = fopen("sensores.csv","r");
		if(sensores == NULL){
			color("rojo");
			printf("    SIM> Error al abrir el archivo de sensores\n");
			color("blanco");
		}
		else{
			printf( "    SIM> Los sensores se conectaron correctamente para lectura\n");

			// Mientras encuentre lineas en el archivo de sensores
			while(fgets(linea,40,sensores)!=NULL){

				// Abrir archivo de los registros en modo escritura
				registro = fopen("registro.csv", "a");

				// Si no se pudo abrir o no existia, mostrar mensaje de advertencia
				if(registro == NULL){
					color("rojo");
					printf("    SIM> Error al abrir el archivo de registro\n");
					color("blanco");
				}

				// Si la apertura fue exitosa, escribir una linea en el registro
				else{
					color("verde");
					printf("    SIM> Escribiendo linea: %s",linea);
					color("blanco");
					fprintf(registro, "%s", linea);

					// Cerrar archivo registro para que lo pueda usar el servidor
					close( registro );

					// Liberar buffer de registro
					fflush(registro);
				}

				// Esperar 1 segundo antes de escribir la siguiente linea
				sleep(1);
			}
		}

		// Cerrar sensores
		close( sensores );

	printf("    SIM> Reiniciando sensores...\n");
	}

	return 0; 
} 
