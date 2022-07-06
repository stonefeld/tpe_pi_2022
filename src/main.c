#include "sensors.h"
#include "readings.h"
#include "utils.h"

#define STREAM_LEN 1024

int
main(int argc, char **argv)
{
	FILE *f;
	char stream[STREAM_LEN];
	int c = 0;
	Sensors s = sensors_new();
	Readings r = readings_new((unsigned int[]){ 1, 2 }, 2);

	if (!(f = fopen(argv[2], "r")))
		return 1;

	fgets(stream, STREAM_LEN, f);
	while (c == 0 && fgets(stream, STREAM_LEN, f) != NULL)
		c = sensors_add(s, stream);
	fclose(f);

	// sensors_print(s);

	if (!(f = fopen(argv[1], "r")))
		return 1;

	fgets(stream, STREAM_LEN, f);
	while (c == 0 && fgets(stream, STREAM_LEN, f) != NULL)
		c = readings_add(r, s, stream);
	fclose(f);

	// readings_print(r);
	unsigned int rows, cols;
	Matrix q1 = readings_get_matrix(r, 1, &rows, &cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			printf("%20.20s\t", q1[i][j]);
		putchar('\n');
	}
	readings_free_matrix(q1, rows, cols);

	sensors_free(s);
	readings_free(r);

    return c;
}
