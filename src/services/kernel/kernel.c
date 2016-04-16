/*
** kernel.c -- it is a center of the app
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
#include <sys/types.h>
#include <sys/un.h>

#include <stdio.h>
#include <stdlib.h>

#include <linux/limits.h>

#include <unistd.h>
#include <errno.h>

#include <string.h>
#include <time.h>

#include <fcntl.h>

#include "../../config.h"
#include "../../utils/subprocutils/inc.h"

#include "kernelutils.h"

#define SLEEP_TIMEOUT 1
#define TIMEOUT_BEFORE_EXIT 2


const int buff_len = BUFF_LEN;
char buff[BUFF_LEN];
struct sockaddr_un init_addr;

int main(int argc, char *argv[])
{
    fd_t initfd, senderfd, viewfd;
    size_t len;

    int instad; // instance address
    int cont = 1, retval;

    struct timeval timeout;
    fd_set sockset;

    FD_ZERO(&sockset);

    /* generating instance address */

    if (argc > 2) {
        instad = atoi(argv[2]);
    } else {
        srand(time(0));
        instad = rand();
    }

    printf("using: %d\n", instad);

    /* initializing main domain socket for application */

    if ((initfd = init_main_sock(instad)) < 0) {
        cont = 0;
    }

    /* initializing subprocesses */

    senderfd = spawnproc(argv[0], instad, initfd, "sender");
    viewfd = spawnproc(argv[0], instad, initfd, "view");

    /* main loop */

    while (cont) {
        if (fcntl(senderfd, F_GETFD) != -1) FD_SET(senderfd, &sockset);
        if (fcntl(viewfd, F_GETFD) != -1) FD_SET(viewfd, &sockset);

        timeout = (struct timeval) TIMEOUT_KERNEL;

        if ((retval = select_cust(&sockset, &timeout, "kernel")) < 0) break;

        memset(buff, 0, buff_len);

        if (FD_ISSET(senderfd, &sockset) && retval) { // mesg from the sender
            len = recvio(senderfd, buff, buff_len, 0);
            buff[len] = '\0';
            printf("[i] (kernel) from sender: %s\n", buff);
        } else if (FD_ISSET(viewfd, &sockset) && retval) { // mesg from the view
            len = recvio(viewfd, buff, buff_len, 0);
            buff[len] = '\0';
            printf("[i] (kernel) from view: %s\n", buff);
        }

        if (strcmp(buff, "close") == 0) break;

        if (!retval) {
            sendio(viewfd, "timeout", 7, 0);
        }

        sleep(SLEEP_TIMEOUT);
    }

    snprintf(buff, buff_len, "close");
    sendio(senderfd, buff, strlen(buff), 0);

    snprintf(buff, buff_len, "close");
    sendio(viewfd, buff, strlen(buff), 0);

    puts("[i] (kernel) exiting");

    /* cleanup */

    close(initfd);
    close(senderfd);
    close(viewfd);

    unlink(init_addr.sun_path);

    snprintf(buff, PATH_MAX + 6, "rm -f %s", init_addr.sun_path);
    system(buff);

    if (cont) puts("ok");
    else puts("err");

    sleep(TIMEOUT_BEFORE_EXIT);

    return !cont;
}
