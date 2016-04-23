/*
** view.c -- an user interface
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

#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>

#include "../../config.h"
#include "../../utils/subprocutils/inc.h"
#include "../../utils/uniutils/inc.h"

#define MSGLEN 256
#define MSGARLEN 5

#define CLEAR_CURSOR_CMD "\e[2J\e[0;0H"

int msglen = 0;
char msg[MSGARLEN][MSGLEN];

int main(int argc, char *argv[])
{
    int retval, cont = 1;
    fd_t kernelfd = 0;

    const int buff_len = BUFF_LEN;
    char buff[BUFF_LEN];
    char buff_copy[BUFF_LEN];

    struct timeval timeout;

    fd_set sockset;
    FD_ZERO(&sockset);

    memset(buff, 0, buff_len);
    memset(buff_copy, 0, buff_len);

    /* initialization */

    if ((kernelfd = initproc(argc, argv, "view")) < 0) {
        cont = 0;
    }

    /* confirmation */

    if (confirm(kernelfd, "view") < 0) {
        cont = 0;
    }

    /* main loop */

    while (cont) {
        FD_SET(kernelfd, &sockset);
        timeout = (struct timeval) TIMEOUT_VIEW;

        printf(">> ");
        scanf("%s", buff);

        /* check reserved commands */

        if (buff[0] == 'q') {
            break;
        } else if (buff[0] == 's') {
            scanf("%s", buff);
            printf("[i] (view) sending '%s'\n", buff);
            send(kernelfd, buff, strlen(buff), 0);
        } else if (buff[0] == 'c') {
            printf(CLEAR_CURSOR_CMD);
        } else if (buff[0] == 'a') { // send a message to partner
            send(kernelfd, buff, strlen(buff), 0);
        } else {
            printf("[e] (view) '%c' is not known\n", buff[0]);
        }

        /* select fd */

        if ((retval = select_cust(&sockset, &timeout, "view")) < 0) break;

        if (FD_ISSET(kernelfd, &sockset) && retval) {
            memset(buff, 0, sizeof(buff));
            recv(kernelfd, buff, buff_len, 0);
            printf("[i] (view) from kernel: %s\n", buff);

            if (strcmp(buff, "close") == 0) {
                break;
            }
        }

        // draw(buff);
        // sleep(1);
    }

    puts("[i] (view) exiting");

    snprintf(buff, buff_len, "close");
    send(kernelfd, buff, strlen(buff), 0);

    close(kernelfd);

    return -!cont;
}
