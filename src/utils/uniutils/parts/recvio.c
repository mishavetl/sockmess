#include <sys/socket.h>
#include <fcntl.h>

#include "../inc.h"
#include "../../../config.h"

/*
 * Recieves message if a file descriptor is open and has no errors
 *
 * @param fd (file descriptor to send to)
 * @param buff (buff to write to)
 * @param buff_len (max length of the buff)
 * @param flag (flag to recv with)
 *
 * @return -1 if error else length of the buff
 *
 */
int recvio(fd_t fd, char *buff, size_t buff_len, int flag)
{
    size_t len;

    if (fcntl(fd, F_GETFD) != -1) {
        if (flag == 0) flag = MSG_NOSIGNAL;

        len = recv(fd, buff, buff_len, flag);
        buff[len] = '\0';

        return len;
    }

    return -1;
}
