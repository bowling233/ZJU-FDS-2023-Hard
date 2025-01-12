Maximum Submatrix Project Source Code
=====================================

This is the source code for the Maximum Submatrix project. The project is
written in C and uses the GNU Make build system. Here is a list of the
source files:

main.c - The driver program. It parses the command line arguments and
         calls the appropriate functions to run the program.

mss.h - The header file for the Maximum Submatrix project. It contains
        the function prototypes for the functions in mss.c.

mss.c - The implementation file for the Maximum Submatrix project. It
        contains the implementation of the functions in mss.h.

gen.c - Data generator. It generates random matrices and writes them to
        a file.

Makefile - The GNU Make build system file. It contains the rules for
           building the project.

In order to build and run the project, simply run the following command:

    make

If you just want to build the project, run the following command:

    make build
