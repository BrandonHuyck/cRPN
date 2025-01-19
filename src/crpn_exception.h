//----------------------------------------------------------------------
// FILE: crpn_exception.h
// DATE: CPSC 326, Spring 2023
// NAME: S. Bowers
// DESC: Interface for crpn exceptions
//----------------------------------------------------------------------

#ifndef CRPN_EXCEPTION
#define CRPN_EXCEPTION

#include <string>

// specialized exception for crpn implementation
class cRPNException : public std::exception
{
public:
    // construct a "normal" error exception
    cRPNException(const std::string &msg);

    // helpers to create correct type of error
    static cRPNException LexerError(const std::string &msg);
    static cRPNException ParserError(const std::string &msg);
    static cRPNException StaticError(const std::string &msg);
    static cRPNException VMError(const std::string &msg);

    // return a string representation for printing
    const char *what() const noexcept;

private:
    // the exeception message
    std::string message;
};

#endif
