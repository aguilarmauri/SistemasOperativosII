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
	char *seccion = "Modulos Instalados  ";
	imprimirHEADER(seccion);
	printf("<meta http-equiv='refresh' content='1' />");
	printf("<b>"); imprimirHOSTNAME(); printf("</b><br>");
	system("lsmod | head > modulos.txt");
	imprimirModulos();	
	imprimirFOOTER();
}