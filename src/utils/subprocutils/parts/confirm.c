#include <sys/socket.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <linux/limits.h>

#include "../../../config.h"
#include "../inc.h"

/*
 * Confirms a connection to the kernel
 *
 * @param kernelfd (file descriptor of the kernel)
 * @param name (name of the service)
 *
 * @return -1 if error else 0
 *
 */
int confirm(fd_t kernelfd, const char *name)
{
    char buff[BUFF_LEN];
    const int buff_len = BUFF_LEN;

    snprintf(buff, PATH_MAX, "connected");
    send(kernelfd, buff, strlen(buff), 0);

    memset(&buff, 0, sizeof(buff));

    recv(kernelfd, buff, buff_len, 0);
    if (strcmp(buff, "connected") != 0) {
        printf("[e] (%s) error connect confirmation connection to kernel: %s\n", name, buff);
        return -1;
    }

    return 0;
}
