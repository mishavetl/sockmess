/*
** sender.c -- sends messages over internet sockets
*/

/*
 * Copyleft 2016 Michael Vietluzhskih
 *
 * You are free to copy, modify and publish this code
 *
 * All rights reserved
 *
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <netinet/in.h>
#include <netdb.h>

#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <unistd.h>
#include <errno.h>

#include <linux/limits.h>

#include "../../config.h"
#include "../../utils/subprocutils/inc.h"
#include "senderutils.h"

int main(int argc, char *argv[])
{
    fd_t kernelfd, inetfd;
    int cont = 1;

    size_t len;

    const int buff_len = BUFF_LEN;
    char buff[BUFF_LEN];
    char buff_copy[BUFF_LEN];

    /* initialization */

    if ((kernelfd = initproc(argc, argv, "sender")) < 0) {
        cont = 0;
    }

    /* initialize an inet connection */

    if ((inetfd = init_inet_sock(argv)) < 0) {
        cont = 0;
    }

    /* confirmation */

    if (confirm(kernelfd, "sender") < 0) {
        cont = 0;
    }

    /* main loop */

    while (cont) {
        len = recv(kernelfd, buff, buff_len, 0);
        buff[len] = '\0';

        printf("[i] (sender) from kernel: %s\n", buff);

        if (strcmp(buff, "close") == 0) break;

        strncpy(buff_copy, buff + 1, strlen(buff) - 1);
        buff_copy[strlen(buff) - 1] = '\0';

        send(inetfd, buff_copy, strlen(buff_copy), 0);

        sleep(1);
    }

    puts("[i] (sender) exiting");

    close(kernelfd);

    return -!cont;
}
