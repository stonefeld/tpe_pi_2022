#include "sensors.h"
#include "utils.h"

#define STREAM_LEN 1024

int
main(int argc, char **argv)
{
	FILE *f;
	char stream[STREAM_LEN];
	int c = 0;
	Sensors s = sensors_new();

	if (!(f = fopen(argv[1], "r")))
		return 1;

	fgets(stream, STREAM_LEN, f);
	while (c == 0 && fgets(stream, STREAM_LEN, f) != NULL)
		c = sensors_add(s, stream);
	fclose(f);

	sensors_print(s);

	sensors_free(s);

    return c;
}
