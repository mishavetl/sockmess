#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>
#include <time.h>

#include "../../../config.h"
#include "../inc.h"

/*
 * Initializes connection to kernel
 *
 * @param kernel_addr (empty struct sockaddr_un)
 * @param addr (random number which is initialized in kernel and passed as argv[1])
 *
 * @return kernelfd ; if error -1
 */
fd_t connect_to_kernel(struct sockaddr_un *kernel_addr, const char *addr)
{
    fd_t kernelfd = 0;

    if ((kernelfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("error initializing socket kernel: %s\n", strerror(errno));
        return -1;
    }

    memset(kernel_addr, 0, sizeof(struct sockaddr_un));

    (*kernel_addr).sun_family = AF_UNIX;
    snprintf((*kernel_addr).sun_path, PATH_MAX, "%s_%s.sock", KERNEL_SOCKPATH, addr);

    if (
        connect(
            kernelfd,
            (struct sockaddr *) kernel_addr,
            sizeof(struct sockaddr_un)
        ) < 0
    ) {
        printf("error connecting to kernel: %s\n", strerror(errno));
        return -1;
    }

    return kernelfd;
}
