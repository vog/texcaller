<?php

$latex = '\documentclass{article}
\begin{document}
Hello world!
\end{document}';

$pdf = '';
$info = '';
texcaller_convert($pdf, $info, $latex, 'LaTeX', 'PDF', 5);
printf("PDF size:     %.1f KB\n", strlen($pdf) / 1024.0);
printf("PDF content:  %s ... %s\n", substr($pdf, 0, 5), substr($pdf, -6));

$s = 'Téxt → "with" $peciäl <characters>';

echo 'Original:  '.$s."\n";
echo 'Escaped:   '.texcaller_escape_latex($s)."\n";
