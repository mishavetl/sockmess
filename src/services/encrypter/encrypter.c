/*
** encrypter.c -- a service for encryption of messages
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
#include <math.h>

#include "../../config.h"
#include "../../utils/subprocutils/inc.h"
#include "../../utils/uniutils/inc.h"

#include "encrypterutils.h"

int main(int argc, char *argv[])
{
    int retval, cont = 1;
    fd_t kernelfd = 0;

    size_t len;

    const int buff_len = BUFF_LEN;
    char buff[BUFF_LEN];
    char buff_copy[BUFF_LEN];

    struct timeval timeout;

    fd_set sockset;
    FD_ZERO(&sockset);

    memset(buff, 0, buff_len);
    memset(buff_copy, 0, buff_len);

    /* initialization */

    if ((kernelfd = initproc(argc, argv, "encrypter")) < 0) {
        cont = 0;
    }

    /* confirmation */

    if (confirm(kernelfd, "encrypter") < 0) {
        cont = 0;
    }

    /* main loop */

    while (cont) {
        timeout = (struct timeval) TIMEOUT_ENCRYPTER;
        FD_SET(kernelfd, &sockset);

        if ((retval = select_cust(&sockset, &timeout, "encrypter")) < 0) {
            break;
        }

        if (FD_ISSET(kernelfd, &sockset) && retval) {
            if ((len = recv(kernelfd, buff, buff_len, 0)) < 0) {
                puts("[e] (encrypter) lost connection to kernel");
                break;
            }

            if (strcmp(buff, "close") == 0) {
                break;
            }
        }
    }

    puts("[i] (encrypter) exiting");

    close(kernelfd);

    return -!cont;
}
