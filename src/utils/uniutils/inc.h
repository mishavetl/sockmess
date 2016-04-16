/*
** uniutils -- universal utils
*/

/*
 * Copyleft 2016 Michael Vietluzhskih
 *
 * You are free to copy, modify and publish this code
 *
 * All rights reserved
 *
 */

#ifndef UNIUTILS_H
#define UNIUTILS_H

#include <sys/select.h>
#include "../../config.h"

// int strtoi(char *source, int start, int base);
int sendio(fd_t fd, char *buff, size_t buff_len, int flag);
int recvio(fd_t fd, char *buff, size_t buff_len, int flag);
int select_cust(fd_set *sockset, struct timeval *timeout, const char *name);

#endif
