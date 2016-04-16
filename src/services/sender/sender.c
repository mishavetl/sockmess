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

int main(int argc, char *argv[])
{
    fd_t kernelfd = 0;
    int cont = 1;

    const int buff_len = BUFF_LEN;
    char buff[BUFF_LEN];

    // struct sockaddr_in partner_addr;
    struct sockaddr_un kernel_addr;

    /* initialization */

    if ((kernelfd = initproc(argc, argv, "sender")) < 0) {
        cont = 0;
    }

    /* confirmation */

    if (confirm(kernelfd, "sender") < 0) {
        cont = 0;
    }

    /* main loop */

    while (cont) {
        recv(kernelfd, buff, buff_len, 0);

        printf("[i] (sender) from kernel: %s\n", buff);

        if(strcmp(buff, "close") == 0) break;

        sleep(1);
    }

    puts("[i] (sender) exiting");

    close(kernelfd);

    // if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    //     printf("\n Error : Could not create socket \n");
    //     return 1;
    // }

    // memset(&serv_addr, '0', sizeof(serv_addr));

    // serv_addr.sin_family = AF_INET;
    // serv_addr.sin_port = htons(SOCKETMESS_PORT);

    // if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
    //     printf("\n inet_pton error occured\n");
    //     return 1;
    // }

    // if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    //    printf("\n Error : Connect Failed \n");
    //    return 1;
    // }

    // write(sockfd, "hello", 5);
    //
    // while ((len = read(sockfd, buff, sizeof(buff) - 1)) > 0) {
    //     buff[len] = '\0';
    //
    //     if(fputs(buff, stdout) == EOF) {
    //         printf("\n Error : Fputs error\n");
    //     }
    // }
    //
    // if(len < 0) {
    //     printf("\n Read error \n");
    // }

    return -!cont;
}
