#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFFSIZE 256
#include "funcionesHTML.h"
#include "funcionesC.h"

int main(int argc, char *argv[])
{
	char *seccion = "Info Recursos       ";
	imprimirHEADER(seccion);

			printf("<br><b>Fecha y Hora actuales: </b>");
				imprimirFECHAYHORA();
			printf("<br><b>Maquina: </b>");
				imprimirHOSTNAME();
			printf("<br><b>CPU:</b>");
				imprimirCPU();
			printf("<br><b>Memoria Libre/Total:</b>");
				imprimirMemoria();
			printf("<br><b>Uptime: </b>");
				imprimirUptime();
			//printf("<br><b>Kernel: </b>");
			//	imprimirKernel();
	imprimirFOOTER();
}

