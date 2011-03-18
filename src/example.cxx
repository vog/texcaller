#include <texcaller.h>
#include <iostream>

int main()
{
    //
    //  Generate a PDF document
    //

    std::string latex =
        "\\documentclass{article}"
        "\\begin{document}"
        "Hello world!"
        "\\end{document}";

    try {
        std::string pdf;
        std::string info;

        texcaller::convert(pdf, info, latex, "LaTeX", "PDF", 5);

        std::cout << "Generated PDF of " << pdf.size() << " bytes.";
        std::cout << " Details:" << std::endl << std::endl << info;
    } catch (std::domain_error &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }


    //
    //  Escape a string for LaTeX
    //

    const std::string s =
        "Téxt → \"with\" $peciäl <characters>";

    std::cout << std::endl;
    std::cout << "Original:  " << s << std::endl;
    std::cout << "Escaped:   " << texcaller::escape_latex(s) << std::endl;

    return 0;
}
