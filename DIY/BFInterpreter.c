/*
------------------------------------------------------------------------------------
Brainfuck-to-C Interpreter

Last edited on 2019-01-05
Axel Ekman © 2018
------------------------------------------------------------------------------------
DESCRIPTION

This interpreter reads a speficied Brainfuck infile and outputs an
interpreted c-file, which, when executed, runs the C equivalent
of the original Brainfuck code.

The optional parameters for this interpreter are:
    -i infile   specifies the input file name (by default 'brainfuck.in')
    -o outfile  specifies the output file name (by default 'brainfuck.out.c')
    -h          shows a list of available input parameters

If no input arguments are specified,
the program runs with default parameter values.
------------------------------------------------------------------------------------
BRAINFUCK RECAP

Brainfuck is an esoteric, Turing-complete programming language based on
simple memory array manipulation, using only 8 selected operators. Those
operators are the following:
    > = increases memory pointer, or moves the pointer to the right 1 block
    < = decreases memory pointer, or moves the pointer to the left 1 block
    + = increases value stored at the block pointed to by the memory pointer
    - = decreases value stored at the block pointed to by the memory pointer
    [ = like c while(cur_block_value != 0) loop
    ] = if block currently pointed to value is not zero, jump back to [
    , = like c getchar() - input 1 character
    . = like c putchar() - print 1 character to the console

The default memory array size is 30 000 elements.
------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Forward declaration */
void generate ();
void options ();

/* Global variables */
FILE *outfile;
FILE *infile;


int main(int argc, char **argv)
{
    char c;
    char *c_output_name;
    char *bf_input_name;
    int input_was_specified = 0;
    int output_was_specified = 0;

    if(argc < 2) /* I.e. only the program name was called */
    {
        printf("The default values have been chosen. Call the program with -h for help.\n\n");
    }
    else
    {
        while((c = getopt(argc, argv, "i:o:h")) != -1)
        {
            switch(c)
            {
                case 'i':
                    bf_input_name = optarg;
                    input_was_specified++;
                    break;
                case 'o':
                    c_output_name = optarg;
                    output_was_specified++;
                    break;
                case 'h':
                    options();
                    break;
                default:
                    printf("Invalid arguments!\n");
                    exit(1);
            }
        }
    }

    if(!input_was_specified)
    {
        bf_input_name = "brainfuck.in";
    }
    if(!output_was_specified)
    {
        c_output_name = "brainfuck.out.c";
    }

    infile = fopen(bf_input_name, "r");
    outfile = fopen(c_output_name, "w");

    generate();

    fclose(infile);
    fclose(outfile);

    return 0;
}

void generate()
{
    char ch;

    fprintf(outfile, "/* This C code was automatically generated from Brainfuck source code by the Brainfuck-to-C Interpreter\n");
    fprintf(outfile, "Axel Ekman © 2018 */\n");
    fprintf(outfile, "\n#include <stdio.h>\n#include <stdlib.h>\n\n");
    fprintf(outfile, "int main (){\n char a[30000], *ptr = a;\n");

    /* Interpret Brainfuck code */
    while((ch = fgetc(infile)) != EOF)
    {
        switch(ch){
            case '>':
                fprintf(outfile, "ptr++;\n");
                break;
            case '<':
                fprintf(outfile, "ptr--;\n");
                break;
            case '+':
                fprintf(outfile, "++*ptr;\n");
                break;
            case '-':
                fprintf(outfile, "--*ptr;\n");
                break;
            case '[':
                fprintf(outfile, "while(*ptr){\n");
                break;
            case ']':
                fprintf(outfile, "}\n");
                break;
            case ',':
                fprintf(outfile, "*ptr = getchar();\n");
                break;
            case '.':
                fprintf(outfile, "putchar(*ptr);\n");
                break;
            default:
                /* All other chars shall be interpreted as comments */
                break;
        }
    }

    fprintf(outfile, "return 0;\n}");
}

void options()
{
    printf("Interpreter specifications can be made with the following options:\n");
    printf("\t-i <infile>\tspecifies the infile name\n");
    printf("\t\t\t(by default 'brainfuck.in')\n");
    printf("\t-o <outfile>\tspecifies the outfile name\n");
    printf("\t\t\t(by default 'brainfuck.out.c')\n");
    printf("\t-h\t\tshows a list of available input parameters\n");
}
