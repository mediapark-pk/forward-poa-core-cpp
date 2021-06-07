#ifndef MATHENCODINGS_H
#define MATHENCODINGS_H

#include <string>

class Math
{
public:
    static std::string Hex2BinInt(const std::string& str);

    static std::string Encode_UInt1LE(int dec);
    static uint16_t Decode_UInt1LE(const std::string& bin);

    static std::string Encode_UInt2LE(const int val);
    static int Decode_UInt2LE(const std::string& bin);

    static std::string Encode_UInt4LE(int dec);
    static int Decode_UInt4LE(const std::string& bin);
    static std::string Encode_UInt8LE(const int val);
    static uint64_t Decode_UInt8LE(const std::string& bin);
    static bool isValidUnit(const int n,const int &bytes);
};

#endif // MATHENCODINGS_H
