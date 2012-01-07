cat >hello.tex <<'EOF'
\documentclass{article}
\begin{document}
Hello world!
\end{document}
EOF

texcaller LaTeX PDF 5 <hello.tex >hello.pdf
