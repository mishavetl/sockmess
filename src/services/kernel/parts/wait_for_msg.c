#include <sys/socket.h>

#include <string.h>

#include <stdio.h>
#include <stdlib.h>

#include "../../../config.h"
#include "../kernelutils.h"

/*
 * Waits while fd will send the msg
 *
 */
void wait_for_msg(fd_t fd, const char *msg)
{
    size_t len;

    const int buff_len = BUFF_LEN;
    char buff[BUFF_LEN];

    memset(buff, 0, buff_len);

    while (1) {
        len = recv(fd, buff, buff_len, 0);
        buff[len] = '\0';

        if (strcmp(buff, msg) == 0 || len == 0) break;
    }
}
