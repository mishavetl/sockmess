/*
** getter.c -- gets message from sender and sends it to the kernel
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

#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>

#include <string.h>

#include "config.h"

#include "subproutils.h"


int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, kernelfd = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_un kernel_addr;

    char buff[1024];

    if (argc < 2) {
        puts("not enough args. second argument is an address of the kernel");
        exit(1);
    }

    if ((kernelfd = connect_to_kernel(kernel_addr, argv[1])) < 0) {
        printf("error when connecting to kernel in getter\n");
        exit(1);
    }

    // connect to the partner

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buff, '0', sizeof(buff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SOCKETMESS_PORT);

    bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);

    snprintf(buff, 1024, "%s", "connected");

    send(kernelfd, buff, strlen(buff));

    while (1) {
        recv(connfd, buff, sizeof(buff) - 1);

        // snprintf(sendBuff, sizeof(sendBuff), "%s\r\n", sendBuff);
        send(kernelfd, buff, strlen(buff));

        close(connfd);
        sleep(1);
     }
}
