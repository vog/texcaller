# encoding: utf-8

$LOAD_PATH.unshift '.'
require 'texcaller'

latex = <<'EOF'
\documentclass{article}
\begin{document}
Hello world!
\end{document}
EOF

pdf, info = Texcaller.convert(latex, 'LaTeX', 'PDF', 5)
puts 'PDF size:     %.1f KB' % (pdf.size / 1024.0)
puts 'PDF content:  %s ... %s' % [pdf[0..4], pdf[-6..-1]]

s = 'Téxt → "with" $peciäl <characters>'

puts "Original:  #{s}"
puts "Escaped:   #{Texcaller.escape_latex(s)}"
