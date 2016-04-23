#ifndef SOCKETMESS_CONFIG_H
#define SOCKETMESS_CONFIG_H

#define PORT 5341

#define KERNEL_SOCKPATH "/tmp/sockmess_kernel_socket" // path to the file for the main socket
#define TMPFILEPATH "/tmp/sockmess_tmpfile"
#define PROCID_FILE "/tmp/sockmess_procidfile"

#define BUFF_LEN 1024

#define TIMEOUT_KERNEL {10, 0}
#define TIMEOUT_VIEW {0, 5}
#define TIMEOUT_GETTER {2, 0}
#define TIMEOUT_SENDER {2, 0}

/* types */

typedef int fd_t;

#endif
