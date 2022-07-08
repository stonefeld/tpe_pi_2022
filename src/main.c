#include "pedestrians.h"

#define STREAM_LEN 1024
#define SENSORS_KEYS 3
#define READINGS_KEYS 7
#define WEEKDAYS 7

void dump_to_csv(char *filename, Matrix query, unsigned int rows, unsigned int cols, char *header);
ErrorCodes read_files(Sensors s, Readings r, const char *sensors_path, const char *readings_path);
unsigned short get_nday(char *day);

void
dump_to_csv(char *filename, Matrix query, unsigned int rows, unsigned int cols, char *header)
{
	int j;
    FILE *f;

    f = fopen(filename, "w+");
	fprintf(f, "%s\n", header);

    for (int i = 0; i < rows; i++) {
        for (j = 0; j < cols - 1; j++)
            fprintf(f, "%s;", query[i][j]);
        fprintf(f, "%s\n", query[i][j]);
    }
    fclose(f);
}

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
	unsigned int keys;
	ErrorCodes status = 0;
	FILE *f;

	char *name, *day, delim[2] = ";";
	unsigned short time, nday;
	unsigned int id, year, count;

	if (!(f = fopen(sensors_path, "r")))
		return E_NOFILE;

	// nos salteamos la primer linea con los headers
	fgets(stream, STREAM_LEN, f);
	while (status != ERROR && fgets(stream, STREAM_LEN, f) != NULL) {
		parser_get(stream, delim, &keys, tokens_sensors, SENSORS_KEYS);

		// verifico que la cantidad de columnas sea la correcta y que el status del
		// sensor sea activo, sino no lo agrego.
		if (keys == SENSORS_KEYS) {
			if (tokens_sensors[2][0] == 'A') {
				name = tokens_sensors[1];
				id = atoi(tokens_sensors[0]);

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
	while (status != ERROR && fgets(stream, STREAM_LEN, f) != NULL) {
		parser_get(stream, delim, &keys, tokens_readings, READINGS_KEYS);

		if (keys == READINGS_KEYS) {
			id = atoi(tokens_readings[4]);

			if (sensors_get_name(s, id, &name)) {
				day = tokens_readings[3];
				time = atoi(tokens_readings[5]);
				nday = get_nday(day);
				year = atoi(tokens_readings[0]);
				count = atoi(tokens_readings[6]);

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
	if (argc != 3) {
		log_code(E_BADARGS);
		return -1;
	}

	ErrorCodes code = 0;
	Sensors s = sensors_new();
	Readings r = readings_new((unsigned int[]){ 1, 2, 3 }, 2);

	code = read_files(s, r, argv[2], argv[1]);
	code = log_code(code);
	// sensors_print(s);
	// readings_print(r);

	unsigned int rows, cols;
	Matrix q1 = NULL;
	code = readings_get_matrix(r, &q1, 1, &rows, &cols);
	code = log_code(code);
	if (code == NOE)
		dump_to_csv("query1.csv", q1, rows, cols, "sensors;counts");
	readings_free_matrix(q1, rows, cols);

	Matrix q2 = NULL;
	code = readings_get_matrix(r, &q2, 2, &rows, &cols);
	code = log_code(code);
	if (code == NOE)
		dump_to_csv("query2.csv", q2, rows, cols, "year;counts");
	readings_free_matrix(q2, rows, cols);

	Matrix q3 = NULL;
	code = readings_get_matrix(r, &q3, 3, &rows, &cols);
	code = log_code(code);
	if (code == NOE)
		dump_to_csv("query3.csv", q3, rows, cols, "day;day_counts;night_counts;total_counts");
	readings_free_matrix(q3, rows, cols);

	// readings_print(r);

	sensors_free(s);
	readings_free(r);

	if (code != NOE)
		return -1;
	return 0;
}
