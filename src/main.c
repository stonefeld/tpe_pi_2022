#include "sensors.h"
#include "utils.h"

int
main(int argc, char **argv)
{
	FILE *f;
	char stream[1024];
	Sensors s = NULL;

	if (!(f = fopen(argv[1], "r")))
		return 1;

	fgets(stream, 1024, f);
	while (1) {
		if (fgets(stream, 1024, f) == NULL)
			break;
		s = sensors_add(s, stream);
	}
	fclose(f);
	sensors_print(s);

	sensors_free(s);

    return 0;
}
