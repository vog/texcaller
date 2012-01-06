/* See doc/index.html for copyright information and documentation. */

/*! \defgroup python Texcaller Python interface
 *
 *  \par Synopsis
 *
 *  \code
import texcaller
texcaller.convert(source, source_format, result_format, max_runs)  # returns a pair (result, info)
texcaller.escape_latex(s)
 *  \endcode
 *
 *  \par Description
 *
 *  These
 *  <a href="http://www.postgresql.org/">Python</a>
 *  functions are simple wrappers
 *  around the \ref c library functions,
 *  making
 *  <a href="http://www.tug.org/">TeX</a>
 *  typesetting
 *  easily accessible from Python.
 *
 *  \par Example
 *
 *  \include example.py
 *
 *  \par Beware of \c \\u
 *
 *  Unfortunately,
 *  Python always interprets \c \\uXXXX sequences in unicode strings
 *  (even in raw unicode strings),
 *  which interferes badly with common LaTeX commands such as \c \\usepackage{}.
 *  This means every backslash \c \\ which is followed by the character \c u
 *  needs to be escaped via \c \\u005c,
 *  which looks really strange:
 *
 *  \include example_ur_1.py
 *
 *  Fortunately,
 *  this problem is easily solved by
 *  using raw byte strings, and
 *  converting those to unicode strings via the \c decode() method:
 *
 *  \include example_ur_2.py
 */

/*! \cond */
#ifdef SWIGPYTHON

%pythonprepend convert %{
    (source, source_format, result_format, max_runs) = args
    args = (source.encode('UTF-8'), source_format.encode('UTF-8'), result_format.encode('UTF-8'), max_runs)
%}
%pythonappend convert %{
    (result, info) = val
    val = (result, info.decode('UTF-8'))
%}

%pythonprepend escape_latex %{
    (s,) = args
    args = (s.encode('UTF-8'),)
%}
%pythonappend escape_latex %{
    val = val.decode('UTF-8')
%}

#endif
/*! \endcond */

/*! \defgroup ruby Texcaller Ruby interface
 *
 *  \par Synopsis
 *
 *  \code
require 'texcaller'
Texcaller.convert(source, source_format, result_format, max_runs)  # returns a pair [result, info]
Texcaller.escape_latex(s)
 *  \endcode
 *
 *  \par Description
 *
 *  These
 *  <a href="http://www.ruby-lang.org/">Ruby</a>
 *  functions are simple wrappers
 *  around the \ref c library functions,
 *  making
 *  <a href="http://www.tug.org/">TeX</a>
 *  typesetting
 *  easily accessible from Ruby.
 *
 *  \par Example
 *
 *  \include example.rb
 */

/*! \cond */
#ifdef SWIGRUBY
#endif
/*! \endcond */

/*! \defgroup php Texcaller PHP interface
 *
 *  \par Synopsis
 *
 *  \code
texcaller_convert(&$result, &$info, $source, $source_format, $result_format, $max_runs)
texcaller_escape_latex($s)
 *  \endcode
 *
 *  \par Description
 *
 *  These
 *  <a href="http://www.php.net/">PHP</a>
 *  functions are simple wrappers
 *  around the \ref c library functions,
 *  making
 *  <a href="http://www.tug.org/">TeX</a>
 *  typesetting
 *  easily accessible from PHP.
 *
 *  \par Example
 *
 *  \include example.php
 */

/*! \cond */
#ifdef SWIGPHP

%rename(texcaller_convert) texcaller::convert;
%rename(texcaller_escape_latex) texcaller::escape_latex;

#endif
/*! \endcond */

/*
 *  Declarations for all languages
 */

%{
#include "../c/texcaller.h"
%}

%include "typemaps.i"
%include "stl.i"

%module texcaller

namespace texcaller {

void convert(std::string &OUTPUT, std::string &OUTPUT, const std::string &source, const std::string &source_format, const std::string &result_format, int max_runs) throw(std::domain_error, std::runtime_error);
std::string escape_latex(const std::string &s) throw(std::runtime_error);

}

%{
#include "../c/texcaller.c"
%}
