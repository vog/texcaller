--TEST--
run example.php
--FILE--
<?php
require 'example.php';
--EXPECT--
PDF size:     5.5 KB
PDF content:  %PDF- ... %%EOF

Original:  Téxt → "with" $peciäl <characters>
Escaped:   Téxt → {''}with{''} \$peciäl \textless{}characters\textgreater{}
