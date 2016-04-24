#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include "../../../config.h"
#include "../getterutils.h"

extern fd_t listenfd;

/*
 * Initializes inet socket
 *
 * @return -1 if error else fd
 *
 */
fd_t init_inet_sock(char *argv[])
{
    char buff[BUFF_LEN];
    const int buff_len = BUFF_LEN;

    fd_t inetfd;
    struct sockaddr_in addr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("[e] (sender) can't create an inet socket");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    memset(buff, 0, sizeof(buff));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[3]));

    if (bind(
            listenfd,
            (struct sockaddr *) &addr,
            sizeof(struct sockaddr_in)
        ) < 0
    ) {
        printf("[e] (getter) error binding address: %s\n", strerror(errno));
        return -1;
    }

    if (listen(listenfd, MAX_PENDING_CONNECTIONS) != 0) {
        printf("[e] (getter) error listening port: %s\n", strerror(errno));
        return -1;
    }

    if ((inetfd = accept(listenfd, (struct sockaddr*) NULL, NULL)) < 0) {
        printf("[e] (getter) error accepting request: %s\n", strerror(errno));
        return -1;
    }

    snprintf(buff, buff_len, "connected");
    send(inetfd, buff, strlen(buff), 0);

    puts("[i] (getter) connected to partner");

    return inetfd;
}
