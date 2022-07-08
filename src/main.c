#include "pedestrians.h"

/*
 * Tamanio del buffer de lectura para cada linea de los archivos leidos. En
 * caso de requerirse mayor tamanio se agranda STREAM_LEN.
 */
#define STREAM_LEN 1024

/*
 * La cantidad de 'keys', 'tokens' o columnas esperadas por el archivo de
 * sensores.
 */
#define SENSORS_KEYS 3

/*
 * La cantidad de 'keys', 'tokens' o columnas esperadas por el archivo de
 * readings.
 */
#define READINGS_KEYS 7

/*
 * La cantidad de dias que tiene la semana.
 */
#define WEEKDAYS 7

/*
 * Traspasa los datos de cada celda de la matriz a un archivo de tipo csv,
 * donde cada columna esta separada por un ';'
 */
void dump_to_csv(char *filename, Matrix query, unsigned int rows, unsigned int cols, char *header);

/*
 * Recibe el nombre del dia la semana (en ingles pues son recibidos del .csv en
 * ese idioma) y devuelve el numero del dia de la semana corresponiente.
 */
unsigned short get_nday(char *day);

/*
 * Recibe un stream de datos, un delimitador especifico y un vector de strings
 * donde se almacenaran los punteros a cada una de los 'tokens' o 'keys' del
 * stream separados por el delimitador especificado.
 */
void parser_get(char *stream, const char *delim, unsigned int *keys, char **tokens, unsigned int dim);

/*
 * Lee los archivos de readings.csv y sensors.csv, procesando cada linea del
 * mismo y enviandolo al backend para ser procesado por el motor de queries
 * especificado.
 */
ErrorCodes read_files(Sensors s, Readings r, const char *sensors_path, const char *readings_path);

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
	int nday = 0;
	for (int i = 0; i < WEEKDAYS; i++)
		if (strcmp(day, days[i]) == 0)
			nday = i;
	return nday;
}

void
parser_get(char *stream, const char *delim, unsigned int *keys, char **tokens, unsigned int dim)
{
	char *token;

	*keys = 0;
	token = strtok(stream, delim);

	while (dim > 0 && token != NULL) {
		tokens[(*keys)++] = token;
		token = strtok(NULL, delim);
		dim--;
	}
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
	if (strcmp(stream, "Sensor_ID;Name;Status\n") != 0)
		return E_NOFILE;

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
	if (strcmp(stream, "Year;Month;Mdate;Day;Sensor_ID;Time;Hourly_Counts\n") != 0)
		return E_NOFILE;

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
	Readings r = readings_new((unsigned int[]){ 1, 2, 3 }, 3);

	code = read_files(s, r, argv[2], argv[1]);
	code = log_code(code);

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

	sensors_free(s);
	readings_free(r);

	if (code != NOE)
		return -1;
	return 0;
}
