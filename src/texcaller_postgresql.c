/* See texcaller.h for copyright information and documentation. */

/*! \defgroup postgresql Texcaller PostgreSQL interface
 *
 *  \par Synopsis
 *
 *  \dontinclude texcaller.sql
 *  \skipline (
 *  \skipline (
 *
 *  \par Description
 *
 *  These
 *  <a href="http://www.postgresql.org/">PostgreSQL</a>
 *  functions are simple wrappers
 *  around the \ref c library functions,
 *  bringing
 *  <a href="http://www.tug.org/">TeX</a>
 *  typesetting
 *  into the world of relational databases.
 *
 *  Invalid TeX documents are handled gracefully
 *  by simply returning \c NULL
 *  rather than aborting with an error.
 *  On failure as well as on success,
 *  additional processing information is provided via
 *  <a href="http://www.postgresql.org/docs/current/static/plpgsql-errors-and-messages.html">NOTICE</a>s.
 *
 *  \par Example
 *
 *  \include example.sql
 */

#include <postgres.h>
#include <executor/executor.h>
#include <utils/builtins.h>

#include <texcaller.h>

PG_MODULE_MAGIC;

Datum postgresql_texcaller_convert(PG_FUNCTION_ARGS);
Datum postgresql_texcaller_escape_latex(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(postgresql_texcaller_convert);
Datum postgresql_texcaller_convert(PG_FUNCTION_ARGS)
{
    char *dest;
    size_t dest_size;
    char *info;
    text *src;
    char *src_format;
    char *dest_format;
    int max_runs;
    bytea *result;
    /* load arguments */
    src = PG_GETARG_TEXT_P(0);
    src_format = text_to_cstring(PG_GETARG_TEXT_P(1));
    dest_format = text_to_cstring(PG_GETARG_TEXT_P(2));
    max_runs = PG_GETARG_INT32(3);
    /* call function */
    texcaller_convert(&dest, &dest_size, &info,
                      VARDATA(src), VARSIZE(src) - VARHDRSZ,
                      src_format, dest_format, max_runs);
    /* free arguments */
    pfree(src_format);
    pfree(dest_format);
    /* show info as NOTICE */
    if (info == NULL) {
        ereport(ERROR,
                (errcode(ERRCODE_OUT_OF_MEMORY),
                 errmsg("Out of memory.")));
        PG_RETURN_NULL();
    }
    ereport(NOTICE,
            (errmsg_internal(info)));
    free(info);
    /* return dest */
    if (dest == NULL) {
        PG_RETURN_NULL();
    }
    result = palloc(VARHDRSZ + dest_size);
    SET_VARSIZE(result, VARHDRSZ + dest_size);
    memcpy(VARDATA(result), dest, dest_size);
    free(dest);
    PG_RETURN_BYTEA_P(result);
}

PG_FUNCTION_INFO_V1(postgresql_texcaller_escape_latex);
Datum postgresql_texcaller_escape_latex(PG_FUNCTION_ARGS)
{
    char *s;
    char *native_result;
    text *result;
    /* load arguments */
    s = text_to_cstring(PG_GETARG_TEXT_P(0));
    /* call function */
    native_result = texcaller_escape_latex(s);
    /* free arguments */
    pfree(s);
    /* return result */
    if (native_result == NULL) {
        ereport(ERROR,
                (errcode(ERRCODE_OUT_OF_MEMORY),
                 errmsg("Out of memory.")));
        PG_RETURN_NULL();
    }
    result = cstring_to_text(native_result);
    free(native_result);
    PG_RETURN_TEXT_P(result);
}
