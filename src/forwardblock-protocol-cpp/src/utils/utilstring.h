#ifndef UTILSTRING_H
#define UTILSTRING_H

#include <string>

class UtilString
{
public:
    static void ltrim(std::string& s);
    static void rtrim(std::string& s);
    static void trim(std::string& s);
};

#endif // UTILSTRING_H
