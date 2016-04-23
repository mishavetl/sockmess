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

#include "viewutils.h"

#define CMD_PROMPT "#> "

int main(int argc, char *argv[])
{
    int retval, cont = 1;
    fd_t kernelfd = 0;

    size_t len;

    const int buff_len = BUFF_LEN;
    char buff[BUFF_LEN];
    char buff_copy[BUFF_LEN];

    // char *msg;

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

        printf(CMD_PROMPT);

        fgets(buff, buff_len, stdin);
        buff[strlen(buff) - 1] = '\0';

        /* check reserved commands */

        if (check_cmd(buff, buff_copy, buff_len, kernelfd)) {
            break;
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
    }

    puts("[i] (view) exiting");

    snprintf(buff, buff_len, "close");
    send(kernelfd, buff, strlen(buff), 0);

    close(kernelfd);

    return -!cont;
}
