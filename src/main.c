#include "pedestrians.h"

/*
 * La cantidad de argumentos que se espera recibir por linea de comandos.
 */
#define EXPECTED_ARGS 3

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
 * Posicion de las correspondientes columnas en los archivos de sensors
 * y readings
 */
enum sensor_keys_ind { S_ID = 0, NAME, STATUS };
enum readings_keys_ind { YEAR = 0, MONTH, MDATE, DAY, R_ID, TIME, COUNTS };

/*
 * Traspasa los datos de cada celda de la matriz a un archivo de tipo csv,
 * donde cada columna esta separada por un ';'.
 * @param: filename: nombre del archivo donde se guardan los datos
 * @param: query: la matriz de strings con los datos a meter en el archivo
 * @param: rows: la cantidad de filas que tiene la matriz
 * @param: cols: la cantidad de columnas que tiene la matriz
 * @param: header: el titulo de las columnas del csv
 */
void dump_to_csv(char *filename, Matrix query, unsigned int rows, unsigned int cols, char *header);

/*
 * Recibe el nombre del dia la semana (en ingles pues son recibidos del .csv en
 * ese idioma) y devuelve el numero del dia de la semana corresponiente.
 * @param: day: el nombre del dia
 * @return: el numero al que le corresponde el dia en la semana
 */
short get_nday(char *day);

/*
 * Recibe un stream de datos, un delimitador especifico y un vector de strings
 * donde se almacenaran los punteros a cada una de los 'tokens' o 'keys' del
 * stream separados por el delimitador especificado.
 * @param: stream: el stream de datos obtenidos del archivo
 * @param: delim: el delimitador de cada token del stream
 * @param: keys: la cantidad de tokens encontrados
 * @param: tokens: donde se guardan los punteros a los tokens
 * @param: dim: la cantidad de tokens que se espera encontrar
 */
void parser_get(char *stream, const char *delim, unsigned int *keys, char **tokens, unsigned int dim);

/*
 * Lee los archivos de readings.csv y sensors.csv, procesando cada linea del
 * mismo y enviandolo al backend para ser procesado por el motor de queries
 * especificado.
 * @param: s: el puntero al TAD de sensores
 * @param: r: el puntero al TAD de readings
 * @param: sensors_path: la ubicacion del archivos de sensors.csv
 * @param: readings_path: la ubicacion del archivos de readings.csv
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

short
get_nday(char *day)
{
	char *days[WEEKDAYS] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
	int nday = -1;
	for (int i = 0; i < WEEKDAYS && nday == -1; i++)
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
	short nday;
	unsigned short time;
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
			if (tokens_sensors[STATUS][0] == 'A') {
				name = tokens_sensors[NAME];
				id = atoi(tokens_sensors[S_ID]);

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
			id = atoi(tokens_readings[R_ID]);

			if (sensors_get_name(s, id, &name)) {
				day = tokens_readings[DAY];
				nday = get_nday(day);

				if (nday != -1) {
					time = atoi(tokens_readings[TIME]);
					year = atoi(tokens_readings[YEAR]);
					count = atoi(tokens_readings[COUNTS]);

					status = readings_add(r, id, name, year, day, nday, time, count);
					status = log_code(status);
				}
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
	if (argc != EXPECTED_ARGS) {
		log_code(E_BADARGS);
		return -1;
	}

	// Inicializamos los sensores y los readings que contiene a los queries
	ErrorCodes code = 0;
	Sensors s = sensors_new();
	Readings r = readings_new((unsigned int[]){ 1, 2, 3 }, 3);

	// Agregamos de ambos archivos cada uno de los datos correspondientes
	code = read_files(s, r, argv[2], argv[1]);

	// En caso de algun error o warning relevalte que se imprima e indique
	// si es un ERROR, un WARN o un NOE (no error)
	code = log_code(code);

	// Creamos las matrices de las correspondientes queries solicitadas y
	// traspasamos los datos de las mismas a archivos csv
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

	// Liberamos toda la memoria utilizada
	sensors_free(s);
	readings_free(r);

	// Y finalizamos el programa con el codigo final. Si no hubo errores
	// retorna 0, caso contrario -1
	if (code != NOE)
		return -1;
	return 0;
}
