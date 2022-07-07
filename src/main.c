#include "logger.h"
#include "parser.h"
#include "readings.h"
#include "sensors.h"
#include "utils.h"

#define STREAM_LEN 1024
#define SENSORS_KEYS 3
#define READINGS_KEYS 7
#define WEEKDAYS 7

ErrorCodes read_files(Sensors s, Readings r, const char *sensors_path, const char *readings_path);
unsigned short get_nday(char *day);

unsigned short
get_nday(char *day)
{
    char *days[WEEKDAYS] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
	int nday;
	for (int i = 0; i < WEEKDAYS; i++)
		if (strcmp(day, days[i]) == 0)
			nday = i;
	return nday;
}

ErrorCodes
read_files(Sensors s, Readings r, const char *sensors_path, const char *readings_path)
{
	char stream[STREAM_LEN];
	char *tokens_sensors[SENSORS_KEYS], *tokens_readings[READINGS_KEYS];
	ErrorCodes status = 0;
	unsigned int keys;
	FILE *f;

	if (!(f = fopen(sensors_path, "r")))
		return E_NOFILE;

	// nos salteamos la primer linea con los headers
	fgets(stream, STREAM_LEN, f);
	while (fgets(stream, STREAM_LEN, f) != NULL) {
		parser_get(stream, &keys, tokens_sensors, SENSORS_KEYS);

		// verifico que la cantidad de columnas sea la correcta y que el status del
		// sensor sea activo, sino no lo agrego.
		if (keys == SENSORS_KEYS) {
			if (tokens_sensors[2][0] == 'A') {
				char *name = tokens_sensors[1];
				unsigned int id = atoi(tokens_sensors[0]);

				status = sensors_add(s, id, name);
				status = log_code(status);
			}
		} else {
			log_code(W_BADKEYS);
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
			char *name;
			unsigned int id = atoi(tokens_readings[4]);

			if (sensors_get_name(s, id, &name)) {
				char *day = tokens_readings[3];
				unsigned short time = atoi(tokens_readings[5]);
				unsigned short nday = get_nday(day);
				unsigned int year = atoi(tokens_readings[0]);
				unsigned int count = atoi(tokens_readings[6]);

				status = readings_add(r, id, name, year, day, nday, time, count);
				status = log_code(status);
			}
		} else {
			log_code(W_BADKEYS);
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
	Readings r = readings_new((unsigned int[]){ 1, 2, 3 }, 3);

	code = read_files(s, r, argv[2], argv[1]);
	code = log_code(code);
	// sensors_print(s);
	// readings_print(r);

#if 0
	unsigned int rows, cols;
	Matrix q1 = NULL;
	if (!readings_get_matrix(r, &q1, 1, &rows, &cols)) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++)
				printf("%20.20s\t", q1[i][j]);
			putchar('\n');
		}
	}
	readings_free_matrix(q1, rows, cols);
#endif
	readings_print(r);

	sensors_free(s);
	readings_free(r);

    return code;
}

