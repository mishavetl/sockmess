#include <sys/socket.h>

#include <string.h>

#include <stdio.h>
#include <stdlib.h>

#include "../../../config.h"
#include "../viewutils.h"

#define CLEAR_CURSOR_CMD "\e[2J\e[0;0H"

/*
 * Checks reserved cmds
 *
 * @return 1 if wants to break loop else 0
 *
 */
int check_cmd(char *buff, char *buff_copy, int buff_len, fd_t kernelfd)
{
    if (buff[0] == 'q') {
        return 1;
    } else if (buff[0] == 's') {
        if (strlen(buff) < 2) {
            puts("[e] (view) you can't send an empty message");
        } else {
            strncpy(buff_copy, buff + 1, strlen(buff));

            printf("[i] (view) sending '%s'\n", buff_copy);
            send(kernelfd, buff_copy, strlen(buff_copy), 0);
        }
    } else if (buff[0] == 'c') {
        printf(CLEAR_CURSOR_CMD);
    } else if (buff[0] == 'a') { // send a message to partner
        if (strlen(buff) < 2) {
            puts("[e] (view) you can't send an empty message");
        } else {
            send(kernelfd, buff, strlen(buff), 0);
        }
    } else if (strlen(buff) == 0) {
        //
    } else {
        printf("[e] (view) '%c' cmd is not known\n", buff[0]);
    }

    return 0;
}
