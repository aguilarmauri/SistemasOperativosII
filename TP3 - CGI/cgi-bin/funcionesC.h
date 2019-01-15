void imprimirHOSTNAME(){
		//printf("\n");
		FILE *f1;
		char buffer1[BUFFSIZE+1];
		f1=fopen("/proc/sys/kernel/hostname","r");
		fgets(buffer1, BUFFSIZE+1, f1);
		printf("%s",buffer1);
		fclose(f1);
}

void imprimirFECHAYHORA(){
		printf("\n");
		FILE *f;
		char buffer[BUFFSIZE+1];
		int hora, min, seg, ano, mes, dia;
		
		f=fopen("/proc/driver/rtc","r");
		//rtc_date       : 2014-08-22
		fgets(buffer, BUFFSIZE+1, f);
		sscanf(buffer, "%*s       %*c %d%*c%d%*c%d",&hora,&min,&seg);
		fgets(buffer, BUFFSIZE+1, f);
		sscanf(buffer, "%*s       %*c %d%*c%d%*c%d",&ano,&mes,&dia);
		
		printf("    %d:%d:%d", hora, min, seg); //hora
		printf("hs, del %d/%d/%d\n", dia, mes, ano); //fecha
		
		fclose(f);
}

void imprimirEncabezado(){
		printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
		printf("\n>Maquina: ");
		imprimirHOSTNAME();
		printf("\n>Fecha y Hora actuales: ");
		imprimirFECHAYHORA();
}

void imprimirCPU(){
		//printf("<br>Procesador<br>"); 
		FILE *f3; 
		char buffer3[BUFFSIZE+1]; 
		f3 = fopen("/proc/cpuinfo","r"); 
		fgets(buffer3, BUFFSIZE+1, f3); 
		fgets(buffer3, BUFFSIZE+1, f3); 
		printf("	%s",buffer3); 
		fgets(buffer3, BUFFSIZE+1, f3); 
		fgets(buffer3, BUFFSIZE+1, f3); 
		fgets(buffer3, BUFFSIZE+1, f3); 
		printf("	%s\n",buffer3); 
		fclose(f3); 
}

void imprimirKernel(){
		FILE *f4; 
		char buffer4[BUFFSIZE+1]; 
		f4 = fopen("/proc/version_signature","r"); 
		fgets(buffer4, 20, f4); 
		printf("%s\n",buffer4); 
		fclose(f4);
}

void imprimirUptime(){
		FILE *f5; 
		char buffer5[BUFFSIZE+1]; 
		f5 = fopen("/proc/uptime","r"); 
		fgets(buffer5, BUFFSIZE+1, f5); 
		int a=1;
		int i=0;
		int cantidad=0;	
		double punto=0; 						
		while(a==1)
		{
		if(buffer5[i]==46)
			punto=i;
			
		else if(buffer5[i]==32)
			{	
				cantidad=i;
				a=-1;
			}
			i++;	
		}
		i=0;	
		double aux=0;

		while(i<cantidad)
		{
			if(buffer5[i]!=46 && punto>0)
				aux+=(buffer5[i]-48)*pow(10.0,punto-1);	
			if(buffer5[i]!=46 && punto<0)
				aux+=(buffer5[i]-48)*pow(10.0,punto);
			punto--;
			i++;
		}
		int horas=aux/3600;
		int minutos=(aux-horas*3600)/60;
		double segundos=(aux-horas*3600-minutos*60);

		printf("Uptime-HH/MM/SS-: %d:%d:%.2f",horas, minutos, segundos); 
		fclose(f5);
}


void imprimirFileSystem(){
		FILE *f; 
		char buffer[BUFFSIZE+1]; 
		char c;
		int n=0;
		f = fopen("/proc/filesystems","r"); 
		do{
			c=fgetc(f);
			if(c=='\n')
			n++;
		}while(c!=EOF);
		fgets(buffer, BUFFSIZE+1, f); 
		printf("\n\nSistemas Soportados: %d\n",n); 
		fclose(f);
		//system("wc -l /proc/filesystems");
}

void imprimirMemoria(){
		//printf("\nMemoria Libre/Total: ");
		//Mem:          970M       841M       129M        11M        30M       327M
		//int memTotal, memLibre;		
		char *memTotal, *memLibre;		
		memTotal = (char *) malloc(15);
		memLibre = (char *) malloc(15);	
		FILE *fp; 
		char buffer[BUFFSIZE+1];
		fp = popen( "free -h", "r");
		fgets(buffer, BUFFSIZE+1, fp);
		fgets(buffer, BUFFSIZE+1, fp);
		//sscanf(buffer,"%*s%*c          %d%*c       %*d%*c       %d%*c",&memTotal,&memLibre);
		//Mem:          3,8G       3,2G       595M 
		int i;
		for(i=14; i<19; i++){
			//printf(">>>%c<<<\n",buffer[i] );
			//if(buffer[i]!=' ')
				memTotal[i-14] = buffer[i];
			}
		for(i=35; i<40; i++){
			//printf(">>>%c<<<\n",buffer[i] );
			//if(buffer[i]!=' ')
				memLibre[i-35] = buffer[i];
		}
		//printf(" %dM/%dM\n",memLibre,memTotal);
		printf(" %s",memLibre);
		printf("/%s",memTotal);
		
		//printf("	Memoria Disponible/Total: %s\n",mem); 
}

void imprimirModulos(){
		//printf("\n");
		FILE *archivo;
		char buffer[BUFFSIZE+1];
		archivo = fopen("modulos.txt","r");
		if(archivo != NULL){
			while( fgets(buffer, BUFFSIZE+1, archivo) )
				printf("%s<br>",buffer);
			fclose(archivo);
		}
}

void imprimirMensaje(){
		//printf("\n");
		FILE *archivo;
		char buffer[BUFFSIZE+1];
		archivo = fopen("mensaje.txt","r");
		if(archivo != NULL){
			while( fgets(buffer, BUFFSIZE+1, archivo) )
				printf("%s<br>",buffer);
			fclose(archivo);
		}
}