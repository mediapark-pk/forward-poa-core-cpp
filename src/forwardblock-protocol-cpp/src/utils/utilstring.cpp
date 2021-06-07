#include "utilstring.h"
#include <algorithm>

void UtilString::ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
void UtilString::rtrim(std::string& s)
{
    s.erase(
        std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

void UtilString::trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}
