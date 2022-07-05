#include "logger.h"
#include "utils.h"

void
log_error(const char *log)
{
	printf("%c[0;37;41m Error %c[0;37m %s\n", 27, 27, log);
}

void
log_warn(const char *log)
{
	printf("%c[0;30;43m Warn %c[0;37m %s\n", 27, 27, log);
}

void
log_and_exit(const char *log)
{
	log_error(log);
	exit(1);
}
