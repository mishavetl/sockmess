/*
** subprocutils -- utils for subprocesses
*/

/*
 * Copyleft 2016 Michael Vietluzhskih
 *
 * You are free to copy, modify and publish this code
 *
 * All rights reserved
 *
 */

#ifndef SUBPROUTILS_H
#define SUBPROUTILS_H

fd_t connect_to_kernel(struct sockaddr_un *kernel_addr, const char *name);
fd_t initproc(int argc, char *argv[], const char *name);
int confirm(fd_t kernelfd, const char *name);

#endif
