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
        system("/sbin/insmod /var/www/cgi-bin/hello.ko 2> ErrorInstalacion");

        char *seccion = "Instalar Modulo     ";
        imprimirHEADER(seccion);
        printf("<b>"); imprimirHOSTNAME(); printf("</b><br>");
        printf("Modulo Instalado.<br>");
        system("lsmod | head > modulos.txt");
        remove("/var/www/mensaje.txt");
        system("dmesg | tail -n 1 > mensaje.txt");
        imprimirMensaje();
        printf("<br>");
        imprimirModulos();
        imprimirFOOTER();
}

