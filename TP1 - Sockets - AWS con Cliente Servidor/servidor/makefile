object: aws.o sim.o
	gcc -o aws aws.o
	gcc -o sim simulador.o

sim.o: simulador.c
	gcc -c simulador.c

aws.o:
	gcc -c aws.c

clean:
	rm aws.o
	rm simulador.o