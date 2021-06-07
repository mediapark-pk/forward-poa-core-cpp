#include "mathencodings.h"
#include "../3rdparty/libpack/pack.h"
#include "strencodings.h"

#include <iostream>
#include <sstream>

std::string Math::Hex2BinInt(const std::string& str)
{
    if (!isHex(str)) {
        throw std::invalid_argument("Bad byte in base16 str: " + str);
    }

    return hexToBin(str);
}

std::string Math::Encode_UInt1LE(int dec)
{
    if (dec > 0xff) {
        return "";
    }
    std::stringstream ss;
    ss << std::hex << dec;

    if (ss.str().length() < 2) {
        std::string ret;
        ret.append("0");
        ret.append(std::to_string((Math::Decode_UInt1LE(ss.str())) - 48));
        return hexToBin(ret);
    } else {
        return hexToBin(ss.str());
    }
}

uint16_t Math::Decode_UInt1LE(const std::string& bin)
{
    std::stringstream ss;
    uint16_t decoded;
    ss << binToHex(bin);
    ss >> std::hex >> decoded;
    return decoded;
}

std::string Math::Encode_UInt2LE(const int val)
{
    if (val > 0xFFFF)
        return "";
    return PhPacker::pack('v', val);
}

int Math::Decode_UInt2LE(const std::string& bin)
{
    return PhPacker::unpack<unsigned short>('v', bin);
}

std::string Math::Encode_UInt4LE(int dec)
{
    if (dec > 0xffffffff) {
        return "";
        // throw new \OverflowException(sprintf('Cannot encode %d as UInt2LE', $dec));
    }

    return PhPacker::pack('V', dec);
}

int Math::Decode_UInt4LE(const std::string& bin)
{
    return PhPacker::unpack<unsigned int>('V', bin);
}

std::string Math::Encode_UInt8LE(const int val)
{
    return PhPacker::pack('P', val);
}

uint64_t Math::Decode_UInt8LE(const std::string& bin)
{
    return PhPacker::unpack<uint64_t>('P', bin);
}
bool Math::isValidUnit(const int n, const int& bytes)
{
    switch (bytes) {
    case 1:
        return n >= 0 && n <= 0xff;
    case 2:
        return n >= 0 && n <= 0xffff;
    case 3:
        return n >= 0 && n <= 0xffffffff;
    case 4:
        return n > 0 && n <= INT_MAX;
    default:
        return false;
    }
}
