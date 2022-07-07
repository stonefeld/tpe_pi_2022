#include "logger.h"
#include "utils.h"

static char* _get_time(void);

static char*
_get_time()
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	return asctime(timeinfo);
}

void
log_error(const char *log)
{
	printf("%c[0;37;41m Error %c[0;37m %s - %c[1;30m%s%c[;37m", 27, 27, log, 27, _get_time(), 27);
}

void
log_warn(const char *log)
{
	printf("%c[0;30;43m Warn %c[0;37m %s - %c[1;30m%s%c[;37m", 27, 27, log, 27, _get_time(), 27);
}

void
log_trace(const char *log)
{
	printf("%c[0;30;42m Trace %c[0;37m %s - %c[1;30m%s%c[;37m", 27, 27, log, 27, _get_time(), 27);
}

int
log_code(ErrorCodes code)
{
	char *logs[] = {
		"",
		"Los argumentos recibidos son incorrectos",
		"El archivo especificado no existe",
		"No hay memoria suficiente",
		"La lista esta vacia",
		"El nodo no fue agregado",
		"Mala cantidad de keys del csv",
		"Nodo actualizado",
		"Nodo agregado",
	};

	switch (code) {
		case NOE:
		case I_UPDATED:
		case I_ADDED: {
#if DEBUG && TRACE
			log_trace(logs[code]);
#endif
			code = NOE;
		} break;

		case E_BADARGS:
		case E_NOFILE:
		case E_NOMEM:
		case E_EMPLIST: {
			log_error(logs[code]);
		} break;

		case W_NOTADD:
		case W_BADKEYS: {
			log_warn(logs[code]);
			code = NOE;
		} break;
	}

	return code;
}

void
log_and_exit(const char *log)
{
	log_error(log);
	exit(1);
}
