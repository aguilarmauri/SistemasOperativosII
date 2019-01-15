#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "signal.h" //float x[1250000]={ 0.513741518,
#include "sample.h" //float y[500]={-0.363739013,
#include <string.h>
#define CLOCKS_PER_SEC  1000.0

int main (int argc, char *argv[]) {

	printf(" Procesamiento Doppler:\n");
	// Declaraciones
	int PUNTOS_X = (1250000);
	int PUNTOS_H = (500);
	int PUNTOS_Y = ((PUNTOS_X) + (PUNTOS_H) - 1); //1250499
	int PUNTOS_Z = ((PUNTOS_X) + (PUNTOS_H) - 2); //1250498
	int M = (25000);
	int x1[4]={3,2,1,4};
	int h1[3]={2,1,3};
	//PUNTOS_X = 4;
	//PUNTOS_H = 3;
	//M = 5;
	//Y=[6 7 13 15 7 12 ]
	//Z=[429 780 900 420 720]
	int x2[26] = {2,3,6,2,32,6,23,6,2,3,6,3,2,3,5,6,4,2,23,5,3,6,4,76,2,3};
	int h2[13] = {2,3,56,3,3,6,2,7,64,34,123,25,23};
	//PUNTOS_X = 26;
	//PUNTOS_H = 13;
	//PUNTOS_Y = ((PUNTOS_X) + (PUNTOS_H) - 1);
	//PUNTOS_Z = ((PUNTOS_X) + (PUNTOS_H) - 2);
	//Y=[4 12 133 196 421 259 1902 575 1597 933 1066 1341 3463 2329 6022 3016 4419 2147 1763 1224 2483 1295 1221 1736 1769 5683 2735 2045 3854 1923 2037 5948 3444 9846 2340 2167 121 69 ]
	//Z=[]
	// Inicializaciones
	int n,i;
	float *y;
	y = (float*)malloc ( PUNTOS_Y*sizeof(float) );
	clock_t tiempoMF, tiempoPPP;
	double timeMF, timePPP;
	FILE *correlacion;
	//float z[PUNTOS_Z];
	float *z;
	z = (float*)malloc ( PUNTOS_Z*sizeof(float) );
	float yCTE;

	// Match Filter
	printf("  ------------------------------------------\n");
	printf("  Filtrando por Match Filter...");fflush(stdout);
	//tiempoMF = clock();
	timeMF = omp_get_wtime();
	// Convolucion
	for(n=0; n<PUNTOS_X; n++)
		for(i=0; i<PUNTOS_H; i++)
			y[n+i] += x[n]*h[i];
	//tiempoMF = clock() - tiempoMF;
	timeMF = (omp_get_wtime() - timeMF)*1000;
	printf(" Filtrado.");
	//printf("  tiempo %5.0fms\n",tiempoMF/(double)CLOCKS_PER_SEC);
	printf("\t(%.0f ms)\n",timeMF);
	
	//Imprimir resultado de la convolucion
	printf("  Resultados del filtrado:\n");
	for(i=0; i<PUNTOS_Y; i++)
		if(i<3)
			printf("   y[%d]=%f\n",i,y[i]);
	 
	
	//PPP
	printf("  Calculando PPP...");	fflush(stdout);
	//tiempoPPP = clock();
	timePPP = omp_get_wtime();

	yCTE = 0;
	//for(n=0; n<PUNTOS_Z; n++)
	//		z[n] = 0;
	for(i=0; i<(M-2); i++)
				yCTE += y[i];
	for(n=0; n<PUNTOS_Z; n++)
			z[n] = y[n+1]*yCTE;

	// FORMA TRIVIAL
/*	for(n=0; n<PUNTOS_Z; n++)
		for(i=0; i<(M-2); i++)
			z[n] += y[n+1]*y[i];
*/	
	printf("  Calculado.");
	//tiempoPPP = clock() - tiempoPPP;
	timePPP = (omp_get_wtime() - timePPP)*1000;
	//printf("  tiempo %5.0fms\n",tiempoPPP/(double)CLOCKS_PER_SEC);
	printf("\t\t\t(%.0f ms)\n",timePPP);

	//Imprimir resultado de la convolucion
	//printf("  Resultados del PPP:\n");
	correlacion = fopen("ppp.h","w+");
	char *Zi;
	Zi = (char*)malloc ( 50*sizeof(char) );
	for(i=0; i<PUNTOS_Z; i++){
		if(i<3)
			printf("   z[%d]=%f\n",i,z[i]);
		sprintf(Zi,"%f",z[i]); //convierte float a char
		strcat(Zi," ");
		fputs(Zi,correlacion);
	}
	fclose(correlacion);
	 
	printf("  ------------------------------------------\n");

}
