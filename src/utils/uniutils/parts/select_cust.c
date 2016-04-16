#include <stdio.h>

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "../../../config.h"
#include "../inc.h"

/*
 * Just a snippet to the select function with error handling (custom select)
 *
 * @param sockset (fd_set of sockets)
 * @param timeout
 * @param name (name of the service)
 *
 * @return -1 if error else if msg 1 else 0
 *
 */
int select_cust(fd_set *sockset, struct timeval *timeout, const char *name) {
     int retval;

     if ((retval = select(FD_SETSIZE, sockset, NULL, NULL, timeout)) < 0) {
          printf("[e] (%s) error selecting fd: %s\n", name, strerror(errno));
     }

     return retval;
}
