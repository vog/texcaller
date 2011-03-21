/*! \mainpage Texcaller
 *
 *  \subpage download | \subpage license | \subpage links
 *
 *  Texcaller is a convenient interface to the
 *  <a href="http://www.tug.org/">TeX</a>
 *  command line tools
 *  that handles all kinds of errors without much fuzz.
 *  It is written in plain C,
 *  is fairly portable,
 *  and has no external dependencies
 *  besides TeX.
 *
 *  - \ref shell
 *  \dontinclude example.sh
 *  \skipline texcaller
 *
 *  - \ref c
 *  \dontinclude example.c
 *  \skip texcaller_convert
 *  \until ;
 *
 *  - \ref cxx
 *  \dontinclude example.cxx
 *  \skip texcaller::convert
 *  \until ;
 *
 *  - \ref postgresql
 *  \dontinclude example.sql
 *  \skip select
 *  \until ;
 *
 *  - \ref python
 *  \dontinclude example.py
 *  \skipline texcaller.convert
 *
 *  \page download Download
 *
 *  \section development Development version
 *
 *  <a href="https://github.com/vog/texcaller">Texcaller on GitHub</a>
 *
 *  \verbatim
git clone https://github.com/vog/texcaller.git
\endverbatim
 *
 *  \section release-0_4 Release 0.4
 *
 *  <a href="http://www.profv.de/texcaller/texcaller-0.4.tar.gz">Download</a> |
 *  <a href="https://github.com/vog/texcaller/commits/0.4">ChangeLog</a>
 *
 *  This release adds a Python interface.
 *  The C++ interface now uses standard exceptions.
 *
 *  \section release-0_3 Release 0.3
 *
 *  <a href="http://www.profv.de/texcaller/texcaller-0.3.tar.gz">Download</a> |
 *  <a href="https://github.com/vog/texcaller/commits/0.3">ChangeLog</a>
 *
 *  This release provides a C++ interface.
 *
 *  \section release-0_2 Release 0.2
 *
 *  <a href="http://www.profv.de/texcaller/texcaller-0.2.tar.gz">Download</a> |
 *  <a href="https://github.com/vog/texcaller/commits/0.2">ChangeLog</a>
 *
 *  This release provides an interface to PostgreSQL
 *  and an improved build system.
 *
 *  \section release-0_1 Release 0.1
 *
 *  <a href="http://www.profv.de/texcaller/texcaller-0.1.tar.gz">Download</a> |
 *  <a href="https://github.com/vog/texcaller/commits/0.1">ChangeLog</a>
 *
 *  This is the first release.
 *  It started as a reimplementation of
 *  <a href="http://pypi.python.org/pypi/tex">python-tex</a> 1.7.
 *
 *  \page license License
 *
 *  Copyright © 2010, 2011
 *  <a href="http://www.profv.de/">Volker Grabsch</a>
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
 *  \page links Links
 *
 *  \section website Project website
 *
 *  - <a href="http://www.profv.de/texcaller/">http://www.profv.de/texcaller/</a>
 *
 *  \section similar Similar projects
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
 */

#ifndef texcaller_3c67c51f1b9e45dea90acf18eb639723
#define texcaller_3c67c51f1b9e45dea90acf18eb639723

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/*! \defgroup c Texcaller C interface
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
 *  \param result
 *      will be set to a newly allocated buffer that contains
 *      the generated document,
 *      or \c NULL if an error occured.
 *
 *  \param result_size
 *      will be set to the size of \c result, or 0 if \c result is \c NULL.
 *
 *  \param info
 *      will be set to a newly allocated string that contains
 *      additional information such as
 *      an error message or TeX warnings.
 *      When out of memory,
 *      \c info will be set to \c NULL
 *      (and \c result will be set to \c NULL, too).
 *
 *  \param source
 *      the source to convert
 *
 *  \param source_size
 *      size of \c source
 *
 *  \param source_format
 *      must be one of:
 *      - \c "TeX"
 *      - \c "LaTeX"
 *
 *  \param result_format
 *      must be one of:
 *      - \c "DVI"
 *      - \c "PDF"
 *
 *  \param max_runs
 *      maximum number of TeX runs,
 *      must be ≥ 2.
 *      If the output hasn't stabilized after \c max_runs runs,
 *      the function will fail and \c result will be set to \c NULL.
 */
void texcaller_convert(char **result, size_t *result_size, char **info, const char *source, size_t source_size, const char *source_format, const char *result_format, int max_runs);

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

/*! @} */

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <string>
#include <stdexcept>

namespace texcaller
{

/*! \defgroup cxx Texcaller C++ interface
 *
 *  The \c texcaller.h header does not only provide the C functions,
 *  but also the corresponding C++ wrappers.
 *
 *  The following example program demonstrates
 *  how these are meant to be used:
 *
 *  \include example.cxx
 *
 *  Since the C++ wrappers are defined completely inline,
 *  there is no need for an extra C++ library.
 *  The compiler and linker options of the \ref c
 *  also work perfectly for C++:
 *
 *  \code
c++ -o example example.cxx `pkg-config texcaller --cflags --libs`
 *  \endcode
 *
 *  @{
 */

/*! Convert a TeX or LaTeX source to DVI or PDF.
 *
 *  This is a simple wrapper around \ref texcaller_convert.
 *
 *  \param result
 *      will contain the generated document.
 *
 *  \param info
 *      will contain additional information such as TeX warnings.
 *
 *  \param source
 *      the source to convert
 *
 *  \param source_format
 *      must be one of:
 *      - \c "TeX"
 *      - \c "LaTeX"
 *
 *  \param result_format
 *      must be one of:
 *      - \c "DVI"
 *      - \c "PDF"
 *
 *  \param max_runs
 *      maximum number of TeX runs,
 *      must be ≥ 2.
 *
 *  \exception std::domain_error
 *      the TeX source was invalid.
 *      That is,
 *      the TeX interpreter exited with an error,
 *      or the output hasn't stabilized after \c max_runs runs.
 */
inline void convert(std::string &result, std::string &info, const std::string &source, const std::string &source_format, const std::string &result_format, int max_runs) throw(std::domain_error, std::runtime_error)
{
    char *c_result;
    size_t c_result_size;
    char *c_info;
    ::texcaller_convert(&c_result, &c_result_size, &c_info,
                        source.data(), source.size(), source_format.c_str(), result_format.c_str(), max_runs);
    if (c_info == NULL) {
        throw std::runtime_error("Out of memory.");
    }
    if (c_result == NULL) {
        const std::string error_info(c_info);
        free(c_info);
        throw std::domain_error(error_info);
    }
    info.assign(c_info);
    free(c_info);
    result.assign(c_result, c_result_size);
    free(c_result);
}

/*! Escape a string for direct use in LaTeX.
 *
 *  This is a simple wrapper around \ref texcaller_escape_latex.
 *
 *  \param s
 *      the string to escape
 *
 *  \return
 *      the escaped value
 */
inline std::string escape_latex(const std::string &s) throw(std::runtime_error)
{
    char *c_result = ::texcaller_escape_latex(s.c_str());
    if (c_result == NULL) {
        throw std::runtime_error("Out of memory.");
    }
    const std::string result(c_result);
    free(c_result);
    return result;
}

/*! @} */

}

#endif

#endif
