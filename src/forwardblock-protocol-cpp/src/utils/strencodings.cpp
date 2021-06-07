#include "strencodings.h"
#include <iostream>

const signed char p_util_hexdigit[256] =
{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,
  -1,0xa,0xb,0xc,0xd,0xe,0xf,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,0xa,0xb,0xc,0xd,0xe,0xf,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, };

signed char isHexDigit(char c)
{
    return p_util_hexdigit[(unsigned char)c];
}

bool isHex(const std::string &str)
{
    for(std::string::const_iterator it(str.begin()); it != str.end(); ++it)
    {
        if (isHexDigit(*it) < 0)
            return false;
    }
    return (str.size() > 0) && (str.size()%2 == 0);
}

std::string base16Encode(const std::string &input)
{
    static const char hex_digits[] = "0123456789abcdef";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

int hexValue(char hex_digit)
{
    switch (hex_digit) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        return hex_digit - '0';

    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        return hex_digit - 'A' + 10;

    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        return hex_digit - 'a' + 10;
    }
    throw EncodingException("Bad hex_digit");
}

std::string base16Decode(const std::string &input)
{
    const auto len = input.length();
    if (len & 1) {
        throw EncodingException{"base16Decode: Invalid length!"};
    }

    std::string output;
    output.reserve(len / 2);
    for (auto it = input.begin(); it != input.end(); )
    {
        try {
            int hi = hexValue(*it++);
            int lo = hexValue(*it++);
            output.push_back(hi << 4 | lo);
        } catch (const EncodingException &e) {
            throw e;
        }
    }
    return output;
}

std::string hexToBin(const std::string &input)
{
    try {
        return base16Decode(input);
    }  catch (const EncodingException &e) {
        throw e;
    }
}

std::string binToHex(const std::string &input)
{
    return base16Encode(input);
}

bool isBinary(const std::string &str)
{
    for(std::string::const_iterator it(str.begin()); it != str.end(); ++it)
    {
        if ((*it) != '0' || (*it) != '1')
            return false;
    }
    return true;
}
