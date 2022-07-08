#ifndef __LOGGER_H__
#define __LOGGER_H__

/*
 * Esta es una lista de los posibles codigos de estado que puede recibir el
 * frontend.
 */
typedef enum {
	NOE = 0,   WARN,      ERROR,
	E_BADARGS, E_NOFILE,  E_NOMEM,
	E_EMPLIST, W_NOTADD,  W_BADKEYS,
	W_NOQUERY, I_UPDATED, I_ADDED
} ErrorCodes;

/*
 * Permite escribir un mensaje en formato de error.
 */
void log_error(const char *log);

/*
 * Permite escribir un mensaje en formato de advertencia.
 */
void log_warn(const char *log);

/*
 * Imprime un mensaje correspondiente a cada uno de los disponibles codigos
 * de error.
 */
int log_code(ErrorCodes code);

#endif // __LOGGER_H__
