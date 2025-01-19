//----------------------------------------------------------------------
// FILE: crpn.cpp
// DATE: Spring 2023
// AUTH: Brandon Huyck
// DESC: Custom programming language, cRPN
//----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "lexer.h"
// #include "simple_parser.h"
// #include "ast_parser.h"
// #include "print_visitor.h"
// #include "semantic_checker.h"
// #include "vm.h"
// #include "code_generator.h"

using namespace std;

// Prints usage message
//
// @author Huyck, Brandon
// @date 25/1/23
int help()
{
    cout << "Usage: ./crpn [option] [script-file]" << endl
         << "Options:" << endl
         << "--help     prints this message" << endl
         << "--lex      displays token information" << endl
         << "--parse    checks for syntax errors" << endl
         << "--print    pretty prints program" << endl
         << "--check    statically checks program" << endl
         << "--ir       print intermediate (code) representation" << endl;

    return 0;
}

/*
 * Lexical Analysis.
 * Creates and outputs Token objects.
 * Errors and returns code 1 if cRPNException caused.

 * @param inputStream The stream to read the char from
 * @author Huyck, Brandon
 * @date 02/02/23
 */
int lex(istream *inputStream)
{
    Lexer lexer = Lexer(*inputStream);

    try
    {
        Token t = lexer.next_token();
        cout << to_string(t) << endl;
        while (t.type() != TokenType::EOS)
        {
            t = lexer.next_token();
            cout << to_string(t) << endl;
        }
    }
    catch (cRPNException &ex)
    {
        cerr << ex.what() << endl;
        return 1;
    }

    return 0;
}

/*
 * Parser.
 * Uses the Simple Parser to check syntax
 *
 * @param inputStream The stream to read the chars from
 * @author Huyck, Brandon
 * @date 13/4//23
 */
// int parse(istream *inputStream)
// {
//     Lexer lexer = Lexer(*inputStream);

//     try
//     {
//         SimpleParser parser(lexer);
//         parser.parse();
//     }
//     catch (cRPNException &ex)
//     {
//         cerr << ex.what() << endl;
//     }

//     return 0;
// }

/*
 * Pretty Printer.
 * Outputs formatted cRPN file
 *
 * @param inputStream The stream to read the word from
 * @author Huyck, Brandon
 * @date 28/2/23
 */
// int print(istream *inputStream)
// {
//     Lexer lexer = Lexer(*inputStream);

//     try
//     {
//         ASTParser parser(lexer);
//         Program p = parser.parse();
//         PrintVisitor v(cout);
//         p.accept(v);
//     }
//     catch (cRPNException &ex)
//     {
//         cerr << ex.what() << endl;
//     }

//     return 0;
// }

/*
 * Checker; calls parser and semantic checker to verify syntax
 *
 * @param inputStream The stream to read the code from
 * @author Huyck, Brandon
 * @date 2/3/23
 */
// int check(istream *inputStream)
// {

//     Lexer lexer = Lexer(*inputStream);

//     try
//     {
//         ASTParser parser(lexer);
//         Program p = parser.parse();
//         SemanticChecker v;
//         p.accept(v);
//     }
//     catch (cRPNException &ex)
//     {
//         cerr << ex.what() << endl;
//     }

//     return 0;
// }

/*
 * IR.
 * Give the intermediate representation aka virtual machine instructions
 *
 * @param inputStream The stream to read the code from
 * @author Huyck, Brandon
 * @date 13/4/23
 */
// int ir(istream *inputStream)
// {
//     Lexer lexer = Lexer(*inputStream);

//     try
//     {
//         ASTParser parser(lexer);
//         Program p = parser.parse();
//         SemanticChecker t;
//         p.accept(t);
//         VM vm;
//         CodeGenerator g(vm);
//         p.accept(g);
//         cout << to_string(vm) << endl;
//     }
//     catch (cRPNException &ex)
//     {
//         cerr << ex.what() << endl;
//     }

//     return 0;
// }

/*
 * Interpreter.
 * Executes/interprets the code .
 *
 * @param inputStream The stream to read the word from
 * @author Huyck, Brandon
 * @date 13/4/23
 */
// int noOptionGiven(istream *inputStream)
// {
//     Lexer lexer = Lexer(*inputStream);

//     try
//     {
//         ASTParser parser(lexer);
//         Program p = parser.parse();
//         SemanticChecker t;
//         p.accept(t);
//         VM vm;
//         CodeGenerator g(vm);
//         p.accept(g);
//         vm.run();
//     }
//     catch (cRPNException &ex)
//     {
//         cerr << ex.what() << endl;
//     }

//     return 0;
// }

int main(int argc, char *argv[])
{
    // more arguments passed than supported
    // print usage information and abort
    if (argc > 3)
    {
        help();
        return 1;
    }

    istream *input;
    string args[argc];
    int exitCode = 0;

    // convert cstrings to strings
    for (int i = 0; i < argc; ++i)
        args[i] = string(argv[i]);

    // see if file is passed; checks last argument
    if (argc > 1 && args[argc - 1][0] != '-')
    {
        input = new ifstream(args[argc - 1]);

        if (input->fail())
        {
            cout << "ERROR: No such file " << args[argc - 1] << " exists in current directory." << endl;
            delete input;
            return 1;
        }
    }
    else
        input = &cin;

    // call appropriate option, if applicable
    if (args[1][0] == '-')
    {
        if (args[1] == "--help")
            exitCode = help();
        else if (args[1] == "--lex")
            exitCode = lex(input);
        // else if (args[1] == "--parse")
        //     exitCode = parse(input);
        // else if (args[1] == "--print")
        //     exitCode = print(input);
        // else if (args[1] == "--check")
        //     exitCode = check(input);
        // else if (args[1] == "--ir")
        //     exitCode = ir(input);
        else
        {
            cout << "ERROR: \"" << args[1] << "\" is not a valid options; use \"--help\" to list valid options." << endl;
            exitCode = 1;
        }
    }
    else
    {
        // exitCode = noOptionGiven(input);
        cout << "Only --lex supported right now; calling --lex..." << endl;
        exitCode = lex(input);
    }

    // if file was used
    if (argc > 1 && args[argc - 1][0] != '-')
    {
        delete input;
    }

    return exitCode;
}
