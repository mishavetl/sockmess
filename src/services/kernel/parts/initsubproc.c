#include <sys/socket.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include <string.h>

#include "../../../config.h"
#include "../kernelutils.h"
#include "../../../utils/uniutils/inc.h"

/*
 * Initializes subprocess
 *
 * @param name (name of the process)
 * @param initfd (fd of the kernel)
 *
 * @return sockfd ; if error return -1
 *
 */
int initsubproc(const char *name, fd_t initfd)
{
    fd_t sockfd = 0;
    int len;

    char buff[BUFF_LEN];
    const int buff_len = BUFF_LEN;

    if ((sockfd = accept(initfd, (struct sockaddr *) NULL, NULL)) < 0) {
        printf("error accepting connection to %s: %s\n", name, strerror(errno));
        return -1;
    }

    // memset(&buff, 0, sizeof(buff));

    len = recv(sockfd, buff, buff_len, 0);
    buff[len] = '\0';

    if (strcmp(buff, "connected") != 0) {
        printf("error confirmation connection to %s: %s\n", name, buff);
        return -1;
    }

    // memset(&buff, 0, sizeof(buff));

    snprintf(buff, buff_len, "connected");
    send(sockfd, buff, strlen(buff), 0);

    memset(buff, 0, buff_len);

    return sockfd;
}
