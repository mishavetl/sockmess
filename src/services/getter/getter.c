/*
** getter.c -- gets messages from the sender and sends it to the kernel
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
#include <sys/select.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <time.h>

#include "../../config.h"
#include "getterutils.h"
#include "../../utils/subprocutils/inc.h"
#include "../../utils/uniutils/inc.h"

#define SLEEP_TIMEOUT 1

fd_t listenfd;

int main(int argc, char *argv[])
{
    size_t len;
    fd_t inetfd, kernelfd;

    int cont = 1; // continue or not (boolean)
    int retval;

    char buff[BUFF_LEN];
    const int buff_len = BUFF_LEN;

    struct timeval timeout = TIMEOUT_GETTER;
    fd_set sockset;

    FD_ZERO(&sockset);

    /* initialization */

    if ((kernelfd = initproc(argc, argv, "getter")) < 0) {
        cont = 0;
    }

    /* confirmation */

    if (confirm(kernelfd, "getter") < 0) {
        cont = 0;
    }

    /* connect to the partner */

    if ((inetfd = init_inet_sock(argv)) < 0) {
        cont = 0;
    }

    snprintf(buff, buff_len, "ready");

    send(kernelfd, buff, strlen(buff), 0);

    /* main loop */

    while (cont) {
        FD_SET(kernelfd, &sockset);
        FD_SET(inetfd, &sockset);

        timeout = (struct timeval) TIMEOUT_GETTER;

        if ((retval = select_cust(&sockset, &timeout, "getter")) < 0) break;

        if (FD_ISSET(kernelfd, &sockset) && retval) {
            if ((len = recv(kernelfd, buff, buff_len, 0)) == 0) {
                puts("[i] (getter) kernel closed connection");
                break;
            }

            printf("[i] (getter) from kernel: %s\n", buff);
            if (strcmp(buff, "close") == 0) break;
        } else if (FD_ISSET(inetfd, &sockset) && retval) {
            memset(buff, 0, buff_len);
            if ((len = recv(inetfd, buff, buff_len, 0)) == 0) { // len is not working currently
                puts("[i] (getter) partner closed connection");
                strcpy(buff, "close");
                sendio(kernelfd, buff, strlen(buff), 0);
                break;
            }
            // printf("len = %d\n", len);

            // buff[len] = '\0';

            printf("[i] (getter) from partner: %s\n", buff);
        }

        sleep(SLEEP_TIMEOUT);
     }

     puts("[i] (getter) exiting");

     close(inetfd);
     close(listenfd);
     close(kernelfd);

     return -!cont;
}
