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
#include <sys/un.h>

#include <stdlib.h>
#include <stdio.h>

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
#define TIMEOUT_BEFORE_EXIT 1
#define CONNECTION_TIMEOUT 5

char *hostname;

int inport;
int outport;

const int buff_len = BUFF_LEN;
char buff[BUFF_LEN];
struct sockaddr_un init_addr;

int main(int argc, char *argv[])
{
    fd_t initfd, senderfd, getterfd, viewfd, encrypterfd, decrypterfd;
    size_t len;

    int instad; // instance address
    int cont = 1, retval;

    struct timeval timeout;
    fd_set sockset;

    FD_ZERO(&sockset);

    /* generating instance address */

    if (argc != 4) {
        puts("[e] (kernel) not enough args");
        exit(1);
    }

    srand(time(0));
    instad = rand();

    inport = atoi(argv[2]);
    outport = atoi(argv[3]);
    hostname = argv[1];

    printf("using: %d\n", instad);

    /* initializing main domain socket for application */

    if ((initfd = init_main_sock(instad)) < 0) {
        cont = 0;
    }

    /* initializing subprocesses */

    /* NB: don't reorder them */
    if ((getterfd = spawnproc(argv[0], instad, initfd, "getter")) < 0) cont = 0;
    sleep(CONNECTION_TIMEOUT);
    if ((senderfd = spawnproc(argv[0], instad, initfd, "sender")) < 0) cont = 0;
    if ((encrypterfd = spawnproc(argv[0], instad, initfd, "encrypter")) < 0) cont = 0;
    if ((decrypterfd = spawnproc(argv[0], instad, initfd, "decrypter")) < 0) cont = 0;
    if ((viewfd = spawnproc(argv[0], instad, initfd, "view")) < 0) cont = 0;

    /* main loop */

    while (cont) {
        if (fcntl(senderfd, F_GETFD) != -1) FD_SET(senderfd, &sockset);
        if (fcntl(senderfd, F_GETFD) != -1) FD_SET(getterfd, &sockset);
        if (fcntl(viewfd, F_GETFD) != -1) FD_SET(viewfd, &sockset);
        if (fcntl(viewfd, F_GETFD) != -1) FD_SET(encrypterfd, &sockset);
        if (fcntl(viewfd, F_GETFD) != -1) FD_SET(decrypterfd, &sockset);

        timeout = (struct timeval) TIMEOUT_KERNEL;

        if ((retval = select_cust(&sockset, &timeout, "kernel")) < 0) break;

        memset(buff, 0, buff_len);

        if (FD_ISSET(senderfd, &sockset) && retval) { // mesg from the sender
            if ((len = recv(senderfd, buff, buff_len, 0)) == 0) {
                puts("[e] (kernel) lost connection to sender");
                cont = 0;
                break;
            }

            buff[len] = '\0';
            printf("[i] (kernel) from sender: %s\n", buff);
        } else if (FD_ISSET(getterfd, &sockset) && retval) { // mesg from the sender
            if ((len = recv(getterfd, buff, buff_len, 0)) == 0) {
                puts("[e] (kernel) lost connection to getter");
                cont = 0;
                break;
            }

            buff[len] = '\0';
            printf("[i] (kernel) from sender: %s\n", buff);
        } else if (FD_ISSET(viewfd, &sockset) && retval) { // mesg from the view
            if ((len = recv(viewfd, buff, buff_len, 0)) == 0) {
                puts("[e] (kernel) lost connection to view");
                cont = 0;
                break;
            }

            // buff[len] = '\0';
            printf("[i] (kernel) from view: %s\n", buff);

            if (buff[0] == 'a') {
                puts("[i] (kernel) sending message");
                send(senderfd, buff, strlen(buff), 0);
            }
        } else if (FD_ISSET(encrypterfd, &sockset) && retval) { // mesg from the encrypter
            if ((len = recv(encrypterfd, buff, buff_len, 0)) == 0) {
                puts("[e] (kernel) lost connection to encrypter");
                cont = 0;
                break;
            }

            // buff[len] = '\0';
            printf("[i] (kernel) from encrypter: %s\n", buff);
        } else if (FD_ISSET(decrypterfd, &sockset) && retval) { // mesg from the decrypter
            if ((len = recv(decrypterfd, buff, buff_len, 0)) == 0) {
                puts("[e] (kernel) lost connection to decrypter");
                cont = 0;
                break;
            }

            // buff[len] = '\0';
            printf("[i] (kernel) from decrypter: %s\n", buff);
        }

        if (strcmp(buff, "close") == 0) break;

        sleep(SLEEP_TIMEOUT);
    }

    puts("[i] shutting down all services");

    snprintf(buff, buff_len, "close");
    sendio(senderfd, buff, strlen(buff), 0);

    snprintf(buff, buff_len, "close");
    sendio(viewfd, buff, strlen(buff), 0);

    snprintf(buff, buff_len, "close");
    sendio(getterfd, buff, strlen(buff), 0);

    snprintf(buff, buff_len, "close");
    sendio(encrypterfd, buff, strlen(buff), 0);

    snprintf(buff, buff_len, "close");
    sendio(decrypterfd, buff, strlen(buff), 0);

    puts("[i] (kernel) exiting");

    /* cleanup */

    close(initfd);
    close(senderfd);
    close(viewfd);

    unlink(init_addr.sun_path);

    snprintf(buff, PATH_MAX + 6, "rm -f %s", init_addr.sun_path);
    system(buff);

    sleep(TIMEOUT_BEFORE_EXIT);

    if (cont) puts("ok");
    else puts("err");

    return !cont;
}
