#include "print_msg.h"

int
main(int argc, char **argv)
{
    print_msg("Hello world");

#if DEBUG
    print_msg("Debugging");
#endif

    return 0;
}
