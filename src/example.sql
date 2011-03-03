set standard_conforming_strings = on;

-- simple table to store the LaTeX sources of various documents
create table documents (
    name varchar(32) primary key,
    latex_source text
);

-- insert a sample document
insert into documents (name, latex_source) values (
    'hello',
    '\documentclass{article}\begin{document}Hello world!\end{document}'
);

-- generate PDF from the sample document
select
    texcaller_convert(latex_source, 'LaTeX', 'PDF', 5)
from
    documents
where
    name = 'hello';

-- generate a document on the fly, demonstrating how to escape user input
select
    texcaller_convert(
        '\documentclass{article}'
        || '\usepackage[utf8x]{inputenc}'
        || '\begin{document}'
        || texcaller_escape_latex('Téxt → "with" $peciäl <characters>')
        || '\end{document}',
        'LaTeX', 'PDF', 5
    );
