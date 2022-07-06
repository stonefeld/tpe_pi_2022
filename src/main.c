#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 1024

int
main(int argc, char **argv)
{	
	if (argc != 3) {
		printf("La cantidad de argumentos proporcionados es distinta de dos\n");
		return 1;
	}
	
	FILE *f;
	char datos[];
	Sensors s = NULL;
	int c = 0;
	
	if ( (f = fopen(argv[1], "r")) == NULL ) {
		printf("El archivo %s no existe\n", argv[1]);
		return 1;
	}
	
	fgets(datos, MAX_LENGTH, f);

	while( fgets(datos, MAX_LENGTH, f) != NULL && c == 0){
		c = sensors_add(s, datos);
	}
	
	fclose(f);
	
	if ((f = fopen(argv[2], "r")) == NULL) {
		printf("El archivo %s no existe\n", argv[2]);
		return 1;
	}
	
	fgets(datos, MAX_LENGTH, f);

	while( fgets(datos, MAX_LENGTH, f) != NULL && c == 0){
		c = query_add(s, datos);
	}
	
	fclose(f);

	return c;
}

