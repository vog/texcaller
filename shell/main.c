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
    const char *source_format;
    const char *result_format;
    int max_runs;

    char *source;
    size_t source_size;
    size_t read_size;
    const size_t source_size_increment = 4096;

    char *result;
    size_t result_size;
    char *info;

    /* command line arguments */
    if (argc != 4) {
        fprintf(stderr, "Usage: texcaller SRC_FORMAT DEST_FORMAT MAX_RUNS <SRC >DEST\n");
        return 1;
    }
    source_format = argv[1];
    result_format = argv[2];
    max_runs = atoi(argv[3]);

    /* stdin -> source */
    source_size = 0;
    source = NULL;
    do {
        char *new_source = realloc(source, source_size + source_size_increment);
        if (new_source == NULL) {
            free(source);
            fprintf(stderr, "Out of memory.\n");
            return 1;
        }
        source = new_source;
        read_size = fread(source + source_size, 1, source_size_increment, stdin);
        source_size += read_size;
    } while (read_size == source_size_increment);
    if (ferror(stdin)) {
        free(source);
        fprintf(stderr, "Unable to read from stdin: %s.\n", strerror(errno));
        return 1;
    }

    /* run tex */
    texcaller_convert(&result, &result_size, &info,
                      source, source_size, source_format, result_format, max_runs);

    /* cleanup */
    free(source);

    /* info -> stderr */
    fprintf(stderr, "%s\n", info == NULL ? "Out of memory." : info);
    free(info);

    /* output -> stdout */
    if (result == NULL) {
        return 1;
    }
    if (fwrite(result, 1, result_size, stdout) != result_size) {
        fprintf(stderr, "Unable to output %lu bytes: %s.\n",
                (unsigned long)result_size, strerror(errno));
        free(result);
        return 1;
    }
    free(result);
    return 0;
}
