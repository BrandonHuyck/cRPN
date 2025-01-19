//----------------------------------------------------------------------
// FILE: crpn_exception.cpp
// DATE: CPSC 326, Spring 2023
// NAME: S. Bowers
// DESC: cRPN exception class impelementation
//----------------------------------------------------------------------

#include "crpn_exception.h"

cRPNException::cRPNException(const std::string &msg)
    : message{msg}
{
}

cRPNException cRPNException::LexerError(const std::string &msg)
{
    return cRPNException("Lexer Error: " + msg);
}

cRPNException cRPNException::ParserError(const std::string &msg)
{
    return cRPNException("Parser Error: " + msg);
}

cRPNException cRPNException::StaticError(const std::string &msg)
{
    return cRPNException("Static Error: " + msg);
}

cRPNException cRPNException::VMError(const std::string &msg)
{
    return cRPNException("VM Error: " + msg);
}

const char *cRPNException::what() const noexcept
{
    return message.c_str();
}
