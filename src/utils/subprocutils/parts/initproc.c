#include <stdio.h>
#include <sys/un.h>

#include "../../../config.h"
#include "../inc.h"

/*
 * Initializes process and connects it to the kernel
 *
 * @param argc (amount of argc elements)
 * @param name (name of the name)
 *
 * @return -1 if error else fd
 *
 */
fd_t initproc(int argc, char *argv[], const char *name)
{
    struct sockaddr_un addr;
    fd_t fd;

    printf("initializing %s\n", name);

    if (argc < 2) {
        printf("[e] (%s) not enough arguments\n", name);
        printf("[e] (%s) first argument is an address of the kernel\n", name);
        return -1;
    }

    if ((fd = connect_to_kernel(&addr, argv[1])) < 0) {
        printf("[e] (%s) error connecting to kernel in sender\n", name);
        return -1;
    }

    return fd;
}
