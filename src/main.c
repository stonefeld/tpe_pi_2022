#include "sensors.h"
#include "utils.h"

#define MAX_CHARS 1024

int
main(int argc, char **argv)
{
	FILE *f;
	char stream[MAX_CHARS];
	Sensors s = NULL;

	if (!(f = fopen(argv[1], "r")))
		return 1;

	fgets(stream, MAX_CHARS, f);
	while (fgets(stream, MAX_CHARS, f) != NULL)
		s = sensors_add(s, stream);
	fclose(f);

	//sensors_print(s);
	puts(sensors_get_name(s, 79));
	puts(sensors_get_name(s, 42));

	sensors_free(s);

    return 0;
}
