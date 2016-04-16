#include <sys/socket.h>
#include <fcntl.h>

#include "../inc.h"
#include "../../../config.h"

/*
 * Sends message if a file descriptor is open and has no errors
 *
 * @param fd (file descriptor to send to)
 * @param buff (message to send)
 * @param buff_len (length of the buff)
 * @param flag (flag to send with)
 *
 * @return -1 if error else length of the buff
 *
 */
int sendio(fd_t fd, char *buff, size_t buff_len, int flag)
{
    if (fcntl(fd, F_GETFL) != -1) {
        if (flag == 0) flag = MSG_NOSIGNAL;
        return send(fd, buff, buff_len, flag);
    }

    return -1;
}
