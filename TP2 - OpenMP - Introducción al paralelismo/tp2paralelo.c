#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "signal.h" //float x[1250000]={ 0.513741518,
#include "sample.h" //float y[500]={-0.363739013,
#include <string.h>
#define CLOCKS_PER_SEC  1000.0

int main (int argc, char *argv[]) {

	// Declaraciones
	int PUNTOS_X = (1250000);
	int PUNTOS_H = (500);
	int PUNTOS_Y = ((PUNTOS_X) + (PUNTOS_H) - 1);
	int PUNTOS_Z = ((PUNTOS_X) + (PUNTOS_H) - 2);
	int M = (25000);
	int x1[4]={3,2,1,4};
	int h1[3]={2,1,3};
	//Y=[6 7 13 15 7 12 ]
	int x2[26] = {2,3,6,2,32,6,23,6,2,3,6,3,2,3,5,6,4,2,23,5,3,6,4,76,2,3};
	int h2[13] = {2,3,56,3,3,6,2,7,64,34,123,25,23};
    //Y=[4 12 133 196 421 259 1902 575 1597 933 1066 1341 3463 2329 6022 3016 4419 2147 1763 1224 2483 1295 1221 1736 1769 5683 2735 2045 3854 1923 2037 5948 3444 9846 2340 2167 121 69 ]
	int n,i;
	float y[PUNTOS_Y];
	float *z;
	char *Zi;
	clock_t tiempoMF, tiempoPPP;
	double timeMF, timePPP, tiempoTotalMF;
	FILE *correlacion;
	int nthreads, tid;//, cantHilos;
	char *cantHilos;
	cantHilos = (char*)malloc ( 5*sizeof(char) );
	float yCTE;
	printf(" Procesamiento Doppler:\n");

	printf("  Ingrese la cantidad de hilos deseada: ");
	fgets( cantHilos, 4, stdin );
	nthreads = atoi(cantHilos);
	omp_set_num_threads(nthreads);
	//printf("  >Número de hilos = %d\n", nthreads);
	
	// Match Filter
	printf("  ------------------------------------------\n");
	printf("  Filtrando por Match Filter...");fflush(stdout);

	//tiempoTotalMF = 0;
	timeMF = omp_get_wtime();
	#pragma omp parallel shared(y,nthreads) private(i,tid)
	  {

		//tid = omp_get_thread_num();
		/*if (tid == 0){
			nthreads = omp_get_num_threads();
			printf("  >Número de hilos = %d\n", nthreads);
		}
		printf("  Hilo %d iniciando...\n",tid);
		*/

		#pragma omp for
			// Convolucion
			for(n=0; n<PUNTOS_X; n++)//{
				for(i=0; i<PUNTOS_H; i++)
					y[n+i] += h[i]*x[n];
				/*if(n<10)
					printf("hilo %d: n %d i %d\t",tid,n,i);
				}*/
	  }  /* end of parallel section */

	timeMF = (omp_get_wtime() - timeMF)*1000;
	printf(" Filtrado.");
	printf("\t(%.0f ms)\n",timeMF);

	//Imprimir resultado de la convolucion
	printf("  Resultados del filtrado:\n");
	for(i=0; i<PUNTOS_Y; i++)
		if(i<3)
			printf("   y[%d]=%f\n",i,y[i]);
	

	//PPP
	//printf("  ------------------------------------------\n");
	printf("  Calculando PPP...");	fflush(stdout);
	//tiempoPPP = clock();
	z = (float*)malloc ( PUNTOS_Z*sizeof(float) );
	timePPP = omp_get_wtime();
	yCTE = 0;
	#pragma omp parallel shared(y,nthreads, yCTE) private(i,tid)
	  {
		//tid = omp_get_thread_num();
		/*if (tid == 0){
			nthreads = omp_get_num_threads();
			//printf("  >Número de hilos = %d\n", nthreads);
		}*/

		#pragma omp for reduction(+:yCTE)
			for(i=0; i<(M-2); i++)
				yCTE += y[i];

		#pragma omp barrier

		#pragma omp for //schedule(static) = es DEFAULT
			for(n=0; n<PUNTOS_Z; n++)
				z[n] = y[n+1]*yCTE;
			 //i <= 1250498;
			 //j <= 24998;
	}
	timePPP = (omp_get_wtime() - timePPP)*1000;
	printf("  Calculado.");
	//tiempoPPP = clock() - tiempoPPP;
	//printf("  %5.0fms\n",tiempoPPP/(double)CLOCKS_PER_SEC);

	printf("\t\t\t(%.0f ms)\n",timePPP);

	//Imprimir resultado de la convolucion
	printf("  Resultados del PPP:\n");
	correlacion = fopen("ppp.h","w+");
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
