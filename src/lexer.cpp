//----------------------------------------------------------------------
// FILE: lexer.cpp
// DATE: CPSC 326, Spring 2023
// NAME: Brandon Huyck
// DESC: Implementation of Lexer Class
//----------------------------------------------------------------------

#include "lexer.h"

using namespace std;

Lexer::Lexer(istream &input_stream)
    : input{input_stream}, column{0}, line{1}
{
}

char Lexer::read()
{
    ++column;
    return input.get();
}

char Lexer::peek()
{
    return input.peek();
}

void Lexer::error(const string &msg, int line, int column) const
{
    throw cRPNException::LexerError(msg + " at line " + to_string(line) +
                                    ", column " + to_string(column));
}

Token Lexer::next_token()
{
    char ch = read();

    // ignore whitespace and comments
    while (isspace(ch) || ch == '#')
    {
        if (ch == '#')
        {
            do
            {
                ch = read();
            } while (ch != '\n' && ch != EOF);
        }
        if (ch == EOF) // figure out a more elegant solution
            break;
        if (ch == '\n')
        {
            line++;
            column = 0;
        }
        ch = read();
    }

    if (ch == EOF)
        return Token(TokenType::EOS, "EOS", line, column);

    // single char tokens
    if (ch == '.')
        return Token(TokenType::DOT, ".", line, column);
    if (ch == ',')
        return Token(TokenType::COMMA, ",", line, column);
    if (ch == ';')
        return Token(TokenType::SEMICOLON, ";", line, column);

    if (ch == '(')
        return Token(TokenType::LPAREN, "(", line, column);
    if (ch == ')')
        return Token(TokenType::RPAREN, ")", line, column);
    if (ch == '[')
        return Token(TokenType::LBRACKET, "[", line, column);
    if (ch == ']')
        return Token(TokenType::RBRACKET, "]", line, column);
    if (ch == '{')
        return Token(TokenType::LBRACE, "{", line, column);
    if (ch == '}')
        return Token(TokenType::RBRACE, "}", line, column);

    if (ch == '+')
        return Token(TokenType::PLUS, "+", line, column);
    if (ch == '-')
        return Token(TokenType::MINUS, "-", line, column);
    if (ch == '*')
        return Token(TokenType::TIMES, "*", line, column);
    if (ch == '/')
        return Token(TokenType::DIVIDE, "/", line, column);

    // check second/next char
    if (ch == '<')
    {
        if (peek() == '=')
        {
            read();
            return Token(TokenType::LESS_EQ, "<=", line, column - 1);
        }
        return Token(TokenType::LESS, "<", line, column);
    }
    if (ch == '>')
    {
        if (peek() == '=')
        {
            read();
            return Token(TokenType::GREATER_EQ, ">=", line, column - 1);
        }
        return Token(TokenType::GREATER, ">", line, column);
    }
    if (ch == '=')
    {
        if (peek() == '=')
        {
            read();
            return Token(TokenType::EQUAL, "==", line, column - 1);
        }
        return Token(TokenType::ASSIGN, "=", line, column);
    }
    if (ch == '!')
    {
        if (peek() == '=')
        {
            read();
            return Token(TokenType::NOT_EQUAL, "!=", line, column - 1);
        }
        error("expecting '!=' found '!>'", line, column); // problem; need to fix ??
    }

    // char value tokens
    if (ch == '\'')
    {
        int col = column;
        string str = "";

        if ((ch = read()) == '\'')
            error("empty character", line, column);

        if (ch == EOF)
            error("found end-of-file in character", line, column);
        if (ch == '\n')
            error("found end-of-line in character", line, column);
        if (ch == '\t')
            error("Cannot have tab inside char", line, column);

        if (ch != '\\' && peek() != '\'') // read()?
        {
            string msg = "expecting ' found ";
            msg += peek();
            error(msg, line, ++column);
        }
        if (ch != '\\' && peek() == '\'')
        {
            read();
            return Token(TokenType::CHAR_VAL, str + ch, line, col);
        }
        ch = read();
        if (peek() != '\'')
            error("Char can only be one character", line, column);
        else
        {
            read();
            return Token(TokenType::CHAR_VAL, str + '\\' + ch, line, col);
        }
        error("IDK what happened, pal...", line, col);
    }

    // string value tokens
    if (ch == '"')
    {
        int ln = line;
        int col = column;
        string str = "";

        ch = read();

        while (ch != '"')
        {
            if (ch == '\\' && peek() == '"')
                ch = read();
            if (ch == '\n')
                error("found end-of-line in string", ln, column);
            if (ch == EOF)
                error("found end-of-file in string", ln, column);

            str += ch;
            ch = read();
        }
        return Token(TokenType::STRING_VAL, str, ln, col);
    }

    // check digits
    if (isdigit(ch))
    {
        int col = column;
        string str = "";
        bool dec = false;

        if (ch == '0' && isdigit(peek()))
            error("leading zero in number", line, column);

        str += ch;

        while (isdigit(peek()) || (peek() == '.' && !dec))
        {
            ch = read(); // see if more eleg
            str += ch;
            if (ch == '.') // see if more eleg
                dec = true;
        }

        if (ch == '.')
            error("missing digit in '" + str + "'", line, ++column);

        if (!dec)
            return Token(TokenType::INT_VAL, str, line, col);
        else
            return Token(TokenType::DOUBLE_VAL, str, line, col);
    }

    // check reserved words
    if (isalpha(ch))
    {
        int col = column;
        string str = "";
        bool alphas = true;

        str += ch; // revisit to make more elegant

        while (isalpha(peek()) || isdigit(peek()) || peek() == '_')
        {
            ch = read();
            str += ch;

            if (alphas && (isdigit(ch) || ch == '_'))
                alphas = false;
        }

        if (alphas)
        {
            if (str == "true")
                return Token(TokenType::BOOL_VAL, "true", line, col);
            if (str == "false")
                return Token(TokenType::BOOL_VAL, "false", line, col);
            if (str == "null")
                return Token(TokenType::NULL_VAL, "null", line, col);
            if (str == "while")
                return Token(TokenType::WHILE, "while", line, col);
            if (str == "for")
                return Token(TokenType::FOR, "for", line, col);
            if (str == "int")
                return Token(TokenType::INT_TYPE, "int", line, col);
            if (str == "double")
                return Token(TokenType::DOUBLE_TYPE, "double", line, col);
            if (str == "string")
                return Token(TokenType::STRING_TYPE, "string", line, col);
            if (str == "bool")
                return Token(TokenType::BOOL_TYPE, "bool", line, col);
            if (str == "char")
                return Token(TokenType::CHAR_TYPE, "char", line, col);
            if (str == "void")
                return Token(TokenType::VOID_TYPE, "void", line, col);
            if (str == "struct")
                return Token(TokenType::STRUCT, "struct", line, col);
            if (str == "array")
                return Token(TokenType::ARRAY, "array", line, col);
            if (str == "if")
                return Token(TokenType::IF, "if", line, col);
            if (str == "elseif")
                return Token(TokenType::ELSEIF, "elseif", line, col);
            if (str == "else")
                return Token(TokenType::ELSE, "else", line, col);
            if (str == "and")
                return Token(TokenType::AND, "and", line, col);
            if (str == "or")
                return Token(TokenType::OR, "or", line, col);
            if (str == "not")
                return Token(TokenType::NOT, "not", line, col);
            if (str == "new")
                return Token(TokenType::NEW, "new", line, col);
            if (str == "return")
                return Token(TokenType::RETURN, "return", line, col);
        }

        return Token(TokenType::ID, str, line, col);
    }

    string str = "unexpected character '";
    str += ch;
    str += '\'';
    error(str, line, column);
    return Token(); // prevent complier warning; never hits, errors out before
}
