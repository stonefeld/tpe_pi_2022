#ifndef __LOGGER_H__
#define __LOGGER_H__

// #define TRACE

typedef enum {
	NOE = 0,   E_BADARGS, E_NOFILE,
	E_NOMEM,   E_EMPLIST, W_NOTADD,
	W_BADKEYS, I_UPDATED, I_ADDED
} ErrorCodes;

void log_error(const char *log);
void log_warn(const char *log);
int log_code(ErrorCodes code);
void log_and_exit(const char *log);

#endif // __LOGGER_H__
