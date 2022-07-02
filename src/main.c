#include <stdio.h>
#include <stdlib.h>

void check_args(int argc, char **argv);
int file_exists(char *name);

int
main(int argc, char **argv)
{	
	check_args(argc, argv);
	return 0;
}

//Realiza programacion defensiva chequeando que existan los archivos y que la cantidad de args recividos sea la correcta
void
check_args(int argc, char **argv)
{
	if (argc != 3) {
		printf("La cantidad de argumentos proporcionados es distinta de dos\n");
		exit(1);
	}
	
	if( file_exists(argv[1]) == 0 ){
		printf("El archivo %s no existe\n", argv[1]);
		exit(1);
	}
	
	if(file_exists(argv[2]) == 0){
		printf("El archivo %s no existe\n", argv[2]);
		exit(1);
	}

}

//Devuelve 0 si el archivo no existe
int
file_exists(char *name)
{
	FILE *f;
	if ((f = fopen(name, "r")) == NULL) {
		return 0;
	}
	fclose(f);
	return 1;
}