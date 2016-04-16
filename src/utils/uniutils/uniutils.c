#include <math.h>
#include <string.h>
#include <fcntl.h>

#include "uniutils.h"

/*
 * Returns an int representation of source in int
 *
 * @param: source (value to get from)
 * @param: start (index to start from)
 * @param: base (base of int)
 *
 * @return: int
 *
 */
int strtoi(char *source, int start, int base)
{
    int acc = 0,
        d = pow(strlen(source) - start, 2);

    for (; start < strlen(source); start++) {
        acc += d * (source[start] - '0');
        d /= base;
    }

    return acc;
}
