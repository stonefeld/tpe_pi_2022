#ifndef __CONFIG_H__
#define __CONFIG_H__

/*
 * Espacio a reservar cuando se desea alocar un bloque estandar para no
 * calcular espacios necesarios.
 */
#define BLOCK 20

/*
 * La cantidad de columnas que tendran los resultados de la query1.
 */
#define QUERY1_COLS 2

/*
 * La cantidad de columnas que tendran los resultados de la query2.
 */
#define QUERY2_COLS 2

/*
 * La cantidad de columnas que tendran los resultados de la query3.
 */
#define QUERY3_COLS 4

/*
 * El largo maximo de los anios al momento de crear la matriz.
 */
#define YEAR_LEN 5

/*
 * El largo maximo de los dias.
 */
#define DAYS_LEN 10

/*
 * Al descomentar, y compilar con DBG=1 se agrega informacion al log extra que
 * muestra el estado del programa mientras funciona.
 */
// #define TRACE

#endif // __CONFIG_H__
