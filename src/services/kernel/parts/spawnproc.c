#include <sys/socket.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <linux/limits.h>
#include <errno.h>

#include <string.h>

#include "../../../config.h"
#include "../kernelutils.h"
#include "../../../utils/uniutils/inc.h"

#define SUBPROC_ARGC 1

/*
 * Spawns processes
 *
 */
fd_t spawnproc(const char *filename, int instad, fd_t initfd, const char *procname)
{
    char _filename[PATH_MAX];
    char _argv[SUBPROC_ARGC + 2][BUFF_LEN];

    fd_t connfd;
    pid_t pid = fork();

    if (pid == 0) { // it is a child
        memset(_filename, 0, BUFF_LEN);
        memset(_argv, 0, SUBPROC_ARGC * BUFF_LEN);

        snprintf(_argv[0], PATH_MAX, "%s_%s", filename, procname);
        snprintf(_argv[1], BUFF_LEN, "%d", instad);

        // puts(_argv[0]);
        // puts(_argv[1]);

        if (execv(_argv[0], (char *[]) {_argv[0], _argv[1], NULL}) < 0) {
            printf("[e] error running subproc %s: %s", procname, strerror(errno));
            exit(1);
        }

        exit(0);
    }

    if ((connfd = initsubproc(procname, initfd)) < 0) {
        printf("[e] error initializing %s", procname);
    }

    return connfd;
}
