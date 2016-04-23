#include <sys/ioctl.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/select.h>
#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>

#include "../../../config.h"
#include "../../../utils/subprocutils/inc.h"

extern char **msg;
extern int msglen;

/*
 * Draws info on the screen
 *
 * @param buff (message from kernel)
 *
 */
void draw(char *buff)
{
    struct winsize w;
    int i;
    char cmd[20];

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    printf("lines %d\n", w.ws_row);
    printf("columns %d\n", w.ws_col);

    printf("\e[1A\e[%dD\e[K", w.ws_col);
    printf("\e[1A\e[%dD\e[K", w.ws_col);

    getchar();

    for (i = strlen(buff) - 1; i >= 0 && buff[i] != ' '; i++) {
        cmd[i] = buff[i];
    }

    cmd[i] = '\0';
    buff[i] = '\0';

    printf("\e[2J"); // clear screen

    puts("#> ");

    if (strcmp(cmd, "msg")) {
        strcpy(msg[msglen++], buff);
    } else if (strcmp(cmd, "err")) {
        printf("\e[0;31m%s\e[0m\n", buff);
    }

    for (i = 0; i < msglen; i++) {
        puts(msg[i]);
    }

    printf("\e[0;4H"); // move cursor to the first line and 4th column
}
