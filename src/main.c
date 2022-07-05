#include "sensors.h"
#include "utils.h"

#define STREAM_LEN 1024

int
main(int argc, char **argv)
{
	FILE *f;
	char stream[STREAM_LEN];
	Sensors s = NULL;

	if (!(f = fopen(argv[1], "r")))
		return 1;

	fgets(stream, STREAM_LEN, f);
	while (fgets(stream, STREAM_LEN, f) != NULL)
		s = sensors_add(s, stream);
	fclose(f);

	sensors_print(s);
	puts(sensors_get_name(s, 79));
	puts(sensors_get_name(s, 42));

	sensors_free(s);

    return 0;
}
