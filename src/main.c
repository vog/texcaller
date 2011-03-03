/* See texcaller.h for copyright information and documentation. */

/*! \defgroup shell Texcaller Shell interface
 *
 *  \par Synopsis
 *
 *  \code
texcaller SRC_FORMAT DEST_FORMAT MAX_RUNS <SRC >DEST
 *  \endcode
 *
 *  \par Example
 *
 *  \include example.sh
 *
 *  \par Description
 *
 *  The \c texcaller binary is a simple command line tool
 *  around the texcaller_convert() library function.
 *
 *  It is an alternative, simpler command line interface for
 *  <a href="http://www.tug.org/">TeX</a>
 *  to be used in shell scripts.
 *
 *  It reads the source document from standard input
 *  and writes the result document to standard output.
 *  No temporary files are left behind.
 *  Information and error messages are reported to standard error.
 *  The exit code is 0 on success and 1 on failure.
 */

#include "texcaller.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    const char *src_format;
    const char *dest_format;
    int max_runs;

    char *src;
    size_t src_size;
    size_t read_size;
    const size_t src_size_increment = 4096;

    char *dest;
    size_t dest_size;
    char *info;

    /* command line arguments */
    if (argc != 4) {
        fprintf(stderr, "Usage: texcaller SRC_FORMAT DEST_FORMAT MAX_RUNS <SRC >DEST\n");
        return 1;
    }
    src_format = argv[1];
    dest_format = argv[2];
    max_runs = atoi(argv[3]);

    /* stdin -> src */
    src_size = 0;
    src = NULL;
    do {
        char *new_src = realloc(src, src_size + src_size_increment);
        if (new_src == NULL) {
            free(src);
            fprintf(stderr, "Out of memory.\n");
            return 1;
        }
        src = new_src;
        read_size = fread(src + src_size, 1, src_size_increment, stdin);
        src_size += read_size;
    } while (read_size == src_size_increment);
    if (ferror(stdin)) {
        free(src);
        fprintf(stderr, "Unable to read from stdin: %s.\n", strerror(errno));
        return 1;
    }

    /* run tex */
    texcaller_convert(&dest, &dest_size, &info,
                      src, src_size, src_format, dest_format, max_runs);

    /* cleanup */
    free(src);

    /* info -> stderr */
    fprintf(stderr, "%s\n", info == NULL ? "Out of memory." : info);
    free(info);

    /* output -> stdout */
    if (dest == NULL) {
        return 1;
    }
    if (fwrite(dest, 1, dest_size, stdout) != dest_size) {
        fprintf(stderr, "Unable to output %lu bytes: %s.\n",
                (unsigned long)dest_size, strerror(errno));
        free(dest);
        return 1;
    }
    free(dest);
    return 0;
}
