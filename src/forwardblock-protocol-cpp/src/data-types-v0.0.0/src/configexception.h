#ifndef CONFIGEXCEPTION_H
#define CONFIGEXCEPTION_H

#include <stdexcept>
#include <string>

class Exception : public std::runtime_error
{
public:
   Exception(std::string s) : std::runtime_error{std::move(s)} {}
};

#endif    // CONFIGEXCEPTION_H
