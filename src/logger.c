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
log_and_exit(const char *log)
{
	log_error(log);
	exit(1);
}
