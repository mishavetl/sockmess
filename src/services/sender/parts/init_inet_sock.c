#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>

#include "../../../config.h"
#include "../senderutils.h"

/*
 * Initializes an inet socket
 *
 * @param argv (cmd line args sent to the sender)
 *
 * @return -1 if error else fd
 *
 */
fd_t init_inet_sock(char *argv[])
{
    struct sockaddr_in addr;
    fd_t fd;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("[e] (sender) can't create an inet socket");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[3]));

    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) < 0) {
        puts("[e] (sender) inet_pton error occured");
        return -1;
    }

    printf("[i] (sender) connecting to the partner's getter at port: %s\n", argv[3]);

    if (connect(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
       puts("[e] (sender) can't connect to inet socket");
       return -1;
    }

    puts("[i] (sender) connected");

    return fd;
}
