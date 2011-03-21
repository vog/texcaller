#include <texcaller.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char *latex =
        "\\documentclass{article}"
        "\\begin{document}"
        "Hello world!"
        "\\end{document}";

    char *pdf;
    size_t pdf_size;
    char *info;

    texcaller_convert(&pdf, &pdf_size, &info,
                      latex, strlen(latex), "LaTeX", "PDF", 5);

    if (pdf == NULL) {
        printf("Error: %s\n", info == NULL ? "Out of memory." : info);
    } else {
        printf("Generated PDF of %i bytes. Details:\n\n%s", pdf_size, info);
    }

    free(pdf);
    free(info);
    return 0;
}
