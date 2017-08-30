# coding: UTF-8

from __future__ import print_function, unicode_literals

import texcaller

latex = r'''\documentclass{article}
\begin{document}
Hello world!
\end{document}'''

pdf, info = texcaller.convert(latex, 'LaTeX', 'PDF', 5)
print('PDF size:     %.1f KB' % (len(pdf) / 1024.0))
print('PDF content:  %s ... %s' % (pdf[:5], pdf[-6:]))

s = 'Téxt → "with" $peciäl <characters>'

print('Original:  %r' % s)
print('Escaped:   %r' % texcaller.escape_latex(s))
