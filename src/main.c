#include "logger.h"
#include "parser.h"
#include "readings.h"
#include "sensors.h"
#include "utils.h"

#define STREAM_LEN 1024
#define SENSORS_KEYS 3
#define READINGS_KEYS 7

int read_files(Sensors s, Readings r, const char *sensors_path, const char *readings_path);

int
read_files(Sensors s, Readings r, const char *sensors_path, const char *readings_path)
{
	char stream[STREAM_LEN];
	char *tokens_sensors[SENSORS_KEYS], *tokens_readings[READINGS_KEYS];
	int status = 0;
	unsigned int keys;
	FILE *f;

	if (!(f = fopen(sensors_path, "r"))) {
		return E_NOFILE;
	}

	// nos salteamos la primer linea con los headers
	fgets(stream, STREAM_LEN, f);
	while (fgets(stream, STREAM_LEN, f) != NULL) {
		parser_get(stream, &keys, tokens_sensors, SENSORS_KEYS);

		// verifico que la cantidad de columnas sea la correcta y que el status del
		// sensor sea activo, sino no lo agrego.
		if (keys == SENSORS_KEYS && tokens_sensors[2][0] == 'A') {
			unsigned int id = atoi(tokens_sensors[0]);
			char *name = tokens_sensors[1];
			status = sensors_add(s, id, name);
			status = log_code(status);
		}
	}
	fclose(f);

	if (!(f = fopen(readings_path, "r")))
		return E_NOFILE;

	// nos salteamos la primer linea con los headers
	fgets(stream, STREAM_LEN, f);
	while (fgets(stream, STREAM_LEN, f) != NULL) {
		parser_get(stream, &keys, tokens_readings, READINGS_KEYS);

		// verifico que la cantidad de columnas sea la correcta y que el status del
		// sensor sea activo, sino no lo agrego.
		if (keys == READINGS_KEYS) {
			unsigned int id = atoi(tokens_readings[4]);
			char *name;

			if (sensors_get_name(s, id, &name)) {
				unsigned int year = atoi(tokens_readings[0]);
				unsigned int count = atoi(tokens_readings[6]);
				status = readings_add(r, id, name, year, count);
				status = log_code(status);
			}
		}
	}
	fclose(f);

	return status;
}

int
main(int argc, char **argv)
{
	ErrorCodes code = 0;
	Sensors s = sensors_new();
	Readings r = readings_new((unsigned int[]){ 1, 2 }, 2);

	code = read_files(s, r, argv[2], argv[1]);
	code = log_code(code);
	//sensors_print(s);
	//readings_print(r);

	/*
	unsigned int rows, cols;
	Matrix q1 = NULL;
	readings_get_matrix(r, &q1, 1, &rows, &cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			printf("%20.20s\t", q1[i][j]);
		putchar('\n');
	}
	readings_free_matrix(q1, rows, cols);
	*/

	sensors_free(s);
	readings_free(r);

    return code;
}
