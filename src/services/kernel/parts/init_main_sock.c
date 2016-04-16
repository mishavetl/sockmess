#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <linux/limits.h>
#include <errno.h>

#include "../../../config.h"
#include "../kernelutils.h"

extern struct sockaddr_un init_addr;

/*
 * Initializes main socket
 *
 * @param instad (instance address)
 *
 * @return -1 if error else fd
 *
 */
fd_t init_main_sock(int instad)
{
    fd_t fd = 0;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("error initializing socket: %s\n", strerror(errno));
        return -1;
    }

    memset(&init_addr, 0, sizeof(init_addr));

    init_addr.sun_family = AF_UNIX;
    snprintf(init_addr.sun_path, PATH_MAX, "%s_%d.sock", KERNEL_SOCKPATH, instad);

    if (bind(
            fd,
            (struct sockaddr *) &init_addr,
            sizeof(struct sockaddr_un)
        ) < 0
    ) {
        printf("error binding address: %s\n", strerror(errno));
        return -1;
    }

    if (listen(fd, 5) != 0) {
        printf("error listening port: %s\n", strerror(errno));
        return -1;
    }

    return fd;
}
