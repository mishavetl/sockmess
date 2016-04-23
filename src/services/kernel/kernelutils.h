#ifndef KERNELUTILS_H
#define KERNELUTILS_H

#define MAX_PENDING_CONNECTIONS 10

fd_t init_subprocess(const char *name, fd_t initfd);
fd_t spawnproc(const char *filename, int instad, fd_t initfd, const char *procname);
fd_t init_main_sock(int instad);

void wait_for_msg(fd_t fd, const char *msg);

#endif
