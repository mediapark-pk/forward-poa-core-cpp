#ifndef UTILS_STRENCODINGS_H
#define UTILS_STRENCODINGS_H

#include <stdexcept>
#include <string>

class EncodingException : public std::runtime_error
{
public:
    EncodingException(std::string&& s) : std::runtime_error{std::move(s)} {}
    const char* what() const noexcept
    {
        return std::runtime_error::what();
    }

private:
    std::string m_desc;
};

std::string base16Encode(const std::string &input);
std::string base16Decode(const std::string &input);

bool isHex(const std::string &str);
bool isBinary(const std::string &str);

std::string hexToBin(const std::string &input);
std::string binToHex(const std::string &input);

#endif
