/*! \mainpage Texcaller
 *
 *  Texcaller is a convenient interface to the
 *  <a href="http://www.tug.org/">TeX</a>
 *  command line tools
 *  that handles all kinds of errors without much fuzz.
 *  It is written in plain C,
 *  is fairly portable
 *  and has no external dependencies
 *  besides TeX.
 *
 *  - \ref texcaller
 *  \include example.sh
 *
 *  - \ref libtexcaller
 *  \dontinclude example.c
 *  \skipline texcaller
 *  ...
 *  \skip texcaller
 *  \until ;
 *
 *  - \subpage license
 *
 *  - \subpage similar
 *
 *  \page license License
 *
 *  Copyright © 2010 <a href="http://www.profv.de/">Volker Grabsch</a>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject
 *  to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *  \page similar Similar projects
 *
 *  Texcaller doesn't suit your needs?
 *  Perhaps one of the following projects does.
 *
 *  - <a href="http://pypi.python.org/pypi/tex">python-tex</a> (predecessor of Texcaller)
 *  - <a href="http://www.profv.de/texpipeline/">Texpipeline</a> (predecessor of python-tex)
 *  - <a href="http://tug.ctan.org/pkg/autolatex">AutoLaTeX</a> (CTAN)
 *  - <a href="http://tug.ctan.org/pkg/latexmk">latexmk</a> (CTAN)
 *  - <a href="http://tug.ctan.org/pkg/mk">mk</a> (CTAN)
 *  - <a href="http://latex-mk.sourceforge.net/">LaTeX-Mk</a>
 *  - <a href="http://obrecht.fr/texwrapper/">TeXwrapper</a>
 *  - <a href="http://pyx.sourceforge.net/manual/node24.html">PyX / texrunner</a>
 *  - <a href="https://bitbucket.org/nmandery/python-pdflatex/">python-pdflatex</a>
 *
 *  \defgroup libtexcaller Texcaller library
 *
 *  The Texcaller library is a convenient C interface to the
 *  <a href="http://www.tug.org/">TeX</a>
 *  command line tools.
 *
 *  The following example program demonstrates
 *  how this library is meant to be used:
 *
 *  \include example.c
 *
 *  Since Texcaller supports
 *  <a href="http://pkg-config.freedesktop.org/">pkg-config</a>,
 *  compiling and linking in a portable way is as simple as:
 *
 *  \code
cc -o example example.c `pkg-config texcaller --cflags --libs`
 *  \endcode
 *
 *  Assuming that everything is installed
 *  in default locations on your system,
 *  the following simpler command will also work.
 *  However, don't hard code that into your build system!
 *
 *  \code
cc -o example example.c -ltexcaller
 *  \endcode
 *
 *  @{
 */

#ifndef texcaller_3c67c51f1b9e45dea90acf18eb639723
#define texcaller_3c67c51f1b9e45dea90acf18eb639723

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/*! Convert a TeX or LaTeX source to DVI or PDF.
 *
 *  This function is reentrant.
 *  Temporary files are always cleaned up.
 *  The TeX interpreter is automatically re-run as often as necessary
 *  until the output becomes stable.
 *  The interpreter is
 *  always run in batch mode
 *  and is disconnected from stdin, stdout and stderr.
 *  That way, it won't ever get in your way
 *  even if there are issues with your input source.
 *  Instead, all important information is simply collected
 *  in the \c info string.
 *
 *  \param dest
 *      will be set to a newly allocated buffer that contains
 *      the generated document,
 *      or \c NULL if an error occured.
 *
 *  \param dest_size
 *      will be set to the size of \c dest, or 0 if \c dest is \c NULL.
 *
 *  \param info
 *      will be set to a newly allocated string that contains
 *      additional information such as
 *      an error message or TeX warnings.
 *      When out of memory,
 *      \c info will be set to \c NULL
 *      (and \c dest will be set to \c NULL, too).
 *
 *  \param src
 *      source to be converted
 *
 *  \param src_size
 *      size of \c src
 *
 *  \param src_format
 *      source format, must be one of:
 *      - \c "TeX"
 *      - \c "LaTeX"
 *
 *  \param dest_format
 *      destination format, must be one of:
 *      - \c "DVI"
 *      - \c "PDF"
 *
 *  \param max_runs
 *      maximum number of runs of the command \c cmd,
 *      must be ≥ 2.
 *      If the output hasn't stabilized after \c max_runs runs,
 *      the function will fail and \c dest will be set to \c NULL.
 */
void texcaller_convert(char **dest, size_t *dest_size, char **info, const char *src, size_t src_size, const char *src_format, const char *dest_format, int max_runs);

/*! Escape a string for direct use in LaTeX.
 *
 *  That is, all LaTeX special characters are replaced
 *  with proper LaTeX elements.
 *  Note that all other unicode characters remain as they are,
 *  so be sure to use the \c inputenc package
 *  in your LaTeX document:
 *
 *  \code
\usepackage[utf8x]{inputenc}
 *  \endcode
 *
 *  For example, the following string:
 *
 *  \verbatim
Téxt → "with" $peciäl <characters>
\endverbatim
 *
 *  is escaped to:
 *
 *  \verbatim
Téxt → {''}with{''} \$peciäl \textless{}characters\textgreater{}
\endverbatim
 *
 *  This function is reentrant.
 *
 *  \param s
 *      the string to escape
 *
 *  \return
 *      a newly allocated string containing the escaped value,
 *      or \c NULL when out of memory.
 */
char *texcaller_escape_latex(const char *s);

/*!  @} */

#ifdef __cplusplus
}
#endif

#endif
