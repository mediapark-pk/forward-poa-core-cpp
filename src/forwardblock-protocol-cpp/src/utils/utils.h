//
// Created by umar on 23/12/20.
//

#ifndef DATA_TYPES_UTILS_H
#define DATA_TYPES_UTILS_H

#include "../3rdparty/bcmath/bcmath.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace Utils
{
const std::string CHARSET_BINARY = "01";
const std::string CHARSET_OCTAL = "01234567";
const std::string CHARSET_BASE16 = "0123456789abcdef";
const std::string CHARSET_BASE36 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string CHARSET_BASE58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
const std::string CHARSET_BASE62 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

/** IT will work if ltrim don't have whitespaces will fix it later **/
static std::string rtrim(std::string s, const std::string& delimiters = " \f\n\r\t\v")
{
   return s.erase(s.find_last_not_of(delimiters) + 1);
}

static std::string ltrim(std::string s, const std::string& delimiters = " \f\n\r\t\v")
{
   return s.erase(0, s.find_first_not_of(delimiters));
}

static std::string trim(std::string s, const std::string& delimiters = " \f\n\r\t\v")
{
   return s.erase(s.find_last_not_of(delimiters) + 1)
      .erase(0, s.erase(s.find_last_not_of(delimiters) + 1).find_first_not_of(delimiters));
}

static std::vector<std::string> explode(const char delimiter, const std::string& string)
{
   std::vector<std::string> splits;
   std::string buffer = "";
   std::stringstream stream(string);
   while (std::getline(stream, buffer, delimiter)) {
      splits.push_back(buffer);
   }
   return splits;
}    // namespace Utils

/*static std::string deciToBin(const std::string& string, int numOfBits)
{
   int iterator = 0;
   std::vector<int> asciValue(string.length());
   for (const auto ptr : string) {
      asciValue[iterator] = int(ptr);
      ++iterator;
   }
   std::vector<std::string> binaryFromAsci(asciValue.size());
}*/
// todo
/*static std::string base64_encode(std::string data) {
    static constexpr char sEncodingTable[] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

    size_t in_len = data.size();
    size_t out_len = 4 * ((in_len + 2) / 3);
    std::string ret(out_len, '\0');
    size_t i;
    char *p = const_cast<char *>(ret.c_str());

    for (i = 0; i < in_len - 2; i += 3) {
        *p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
        *p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int) (data[i + 1] & 0xF0) >> 4)];
        *p++ = sEncodingTable[((data[i + 1] & 0xF) << 2) | ((int) (data[i + 2] & 0xC0) >> 6)];
        *p++ = sEncodingTable[data[i + 2] & 0x3F];
    }
    if (i < in_len) {
        *p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
        if (i == (in_len - 1)) {
            *p++ = sEncodingTable[((data[i] & 0x3) << 4)];
            *p++ = '=';
        } else {
            *p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int) (data[i + 1] & 0xF0) >> 4)];
            *p++ = sEncodingTable[((data[i + 1] & 0xF) << 2)];
        }
        *p++ = '=';
    }

    return ret;
}*/

static std::string base64_encode(const std::string& in)
{
   std::string out;
   static const std::string b = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

   int val = 0, valb = -6;
   for (char c : in) {
      val = (val << 8) + c;
      valb += 8;
      while (valb >= 0) {
         out.push_back(b[(val >> valb) & 0x3F]);
         valb -= 6;
      }
   }
   if (valb > -6)
      out.push_back(b[((val << 8) >> (valb + 8)) & 0x3F]);
   while (out.size() % 4)
      out.push_back('=');
   return out;
}

/*static std::string base64_decode(const std::string &input) {
    std::string out;
    static constexpr unsigned char kDecodingTable[] = {
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62,
            64, 64, 64, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64, 64, 0,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
            23, 24, 25, 64, 64, 64, 64, 64, 64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
            39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};

    size_t in_len = input.size();
    if (in_len % 4 != 0)
        return "Input data size is not a multiple of 4";

    size_t out_len = in_len / 4 * 3;
    if (input[in_len - 1] == '=')
        out_len--;
    if (input[in_len - 2] == '=')
        out_len--;
    out.resize(out_len);
    for (size_t i = 0, j = 0; i < in_len;) {
        uint32_t a = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
        uint32_t b = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
        uint32_t c = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
        uint32_t d = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];

        uint32_t triple = (a << 3 * 6) + (b << 2 * 6) + (c << 1 * 6) + (d << 0 * 6);

        if (j < out_len)
            out[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < out_len)
            out[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < out_len)
            out[j++] = (triple >> 0 * 8) & 0xFF;
    }
    return out;
}*/

static std::string base64_decode(const std::string& in)
{
   std::string out;
   static const std::string b = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

   std::vector<int> T(256, -1);
   for (int i = 0; i < 64; i++)
      T[b[i]] = i;

   int val = 0, valb = -8;
   for (char c : in) {
      if (T[c] == -1)
         break;
      val = (val << 6) + T[c];
      valb += 6;
      if (valb >= 0) {
         out.push_back(char((val >> valb) & 0xFF));
         valb -= 8;
      }
   }
   return out;
}

enum PAD_TYPE { STR_PAD_RIGHT, STR_PAD_LEFT };

static std::string str_pad(
   const std::string& input, const int pad_length, const std::string& pad_string, PAD_TYPE padType)
{
   std::string tempString;
   switch (padType) {
   case PAD_TYPE::STR_PAD_LEFT:
      if (pad_string.length() > input.length())
         return input;
      while (tempString.length() <= pad_length) {
         if (tempString.length() + input.length() + pad_string.length() <= pad_length) {
            tempString.append(pad_string);
         } else {
            for (int i = 0;; ++i) {
               if (input.length() + tempString.length() < pad_length) {
                  tempString.push_back(pad_string[i]);
               } else {
                  return tempString.append(input);
               }
            }
         }
      }
      return tempString.append(input);

   case PAD_TYPE::STR_PAD_RIGHT:
      if (pad_string.length() > input.length())
         return input;
      while (tempString.length() <= pad_length) {
         if (tempString.length() + input.length() + pad_string.length() <= pad_length) {
            tempString.append(pad_string);
         } else {
            for (int i = 0;; ++i) {
               if (input.length() + tempString.length() < pad_length) {
                  tempString.push_back(pad_string[i]);
               } else {
                  return std::string(input).append(tempString);
               }
            }
         }
      }
      return std::string(input).append(tempString);
   default:
      return input;
   }
}

template <typename Base, typename T> inline bool instanceof (const T* ptr)
{
   return dynamic_cast<const Base*>(ptr) != nullptr;
}

static std::string decToHex(const int decimalValue)
{
   std::stringstream ss;
   ss << std::hex << decimalValue;    // int decimal_value
   std::string res(ss.str());
   return res;
}

static int hexToDec(const std::string& hexValue)
{
   std::stringstream ss;
   int decimalValue = 0;
   ss << hexValue;                    // std::string hex_value
   ss >> std::hex >> decimalValue;    // int decimal_value
   return decimalValue;
}

static std::string chunk_split(std::string& sString, int length, char seperator)
{
   std::string outstring = "";
   outstring = sString.substr(0, length);
   for (int loc = length; loc < sString.size(); loc += length) {
      outstring = outstring + seperator + sString.substr(loc, length);
   }
   return outstring;
}

static std::string number_format(float num, int decimals, char deci_sep = '.', char thous_sep = ',')
{
   std::stringstream stream;
   stream << std::fixed << std::setprecision(decimals) << num;
   std::string s = stream.str();
   return s;
}

static const char* hex_char_to_bin(char c)
{
   // TODO handle default / error
   switch (toupper(c)) {
   case '0':
      return "0000";
   case '1':
      return "0001";
   case '2':
      return "0010";
   case '3':
      return "0011";
   case '4':
      return "0100";
   case '5':
      return "0101";
   case '6':
      return "0110";
   case '7':
      return "0111";
   case '8':
      return "1000";
   case '9':
      return "1001";
   case 'A':
      return "1010";
   case 'B':
      return "1011";
   case 'C':
      return "1100";
   case 'D':
      return "1101";
   case 'E':
      return "1110";
   case 'F':
      return "1111";
   default:
      return "";
   }
}

/**
 * Calculates bits from hex string
 * @param string of hex
 * @return string of bits
 * */

static std::string hex2bits(const std::string& hex)
{
   std::string bits;
   int len = hex.length();
   bits.reserve(len * 4);
   for (int i = 0; i < len; ++i) {
      bits += hex_char_to_bin(hex.at(i));
   }
   return bits;
}
static char bin_str_to_hex(const uint32_t j)
{
   return j > 9 ? (char)j + 55 : (char)j + '0';
}

/**Calculates hex from bits string
 * @param string of bits
 * @return string of hex
 * */
static std::string bits2hex(const std::string& bits) noexcept
{
   std::string hex;
   int len = bits.size();
   hex.reserve(len / 4);
   for (int i = 0; i < len; i += 4) {
      const char* final = bits.substr(i, 4).c_str();
      uint32_t j = 0;
      j = ((uint32_t)(final[0] - '0') << 3) | ((uint32_t)(final[1] - '0') << 2) |
         ((uint32_t)(final[2] - '0') << 1) | ((uint32_t) final[3] - '0');
      hex += bin_str_to_hex(j);
   }
   return hex;
}


static bool hashEquals(const std::string& known, const std::string& user)
{
   int result = 0;
   for (size_t i = 0; i < known.length(); ++i) {
      result |= known.at(i) ^ user.at(i);
   }
   return (result == 0);
}

template <typename Range, typename Value = typename Range::value_type>
static std::string Join(Range const& elements, const char* const delimiter)
{
   std::ostringstream os;
   auto b = begin(elements), e = end(elements);

   if (b != e) {
      std::copy(b, prev(e), std::ostream_iterator<Value>(os, delimiter));
      b = prev(e);
   }
   if (b != e) {
      os << *b;
   }

   return os.str();
}

static std::string toBase10String(std::string encoded, std::string charset, bool isCaseSensitive = true)
{
   if (!isCaseSensitive)    // If case in-sensitive, convert all to lowercase first
      std::transform(encoded.begin(), encoded.end(), encoded.begin(), ::tolower);

   int len = encoded.length();
   std::string charsetLen = std::to_string(charset.length());
   std::string decs = "0", multiplier = "1";

   for (int i = len - 1; i >= 0; i--) {
      decs = BCMath::bcadd(
         decs, BCMath::bcmul(multiplier, std::to_string(charset.find_first_of(encoded[i]))));
      multiplier = BCMath::bcmul(multiplier, charsetLen);
   }

   return decs;
}

static std::string toBase10(std::string encoded, std::string charset, bool isCaseSensitive = true)
{
   std::string val = toBase10String(encoded, charset, isCaseSensitive);
   val = val.substr(0, val.find_first_of('.'));    // to make it integer and remove fractional part
   return val;
}

static std::string fromBase10(const char* data, const std::string& charSet)
{
   if (charSet.empty())
      throw std::runtime_error(" Invalid Charset");
   std::string num = data, encoded = "", charsetLen = std::to_string(charSet.length());

   while (true) {
      if (BCMath::bccomp(num, charsetLen, 0) == -1) {
         break;
      }
      std::string div = BCMath::bcdiv(num, charsetLen, 0);
      std::string mod = BCMath::bcmod(num, charsetLen);
      char char1 = charSet[std::stoi(mod)];
      encoded = char1 + encoded;
      num = div;
   }

   if (BCMath::bccomp(num, "0", 0) != -1) {
      encoded = charSet[std::stoi(num)] + encoded;
   }

   return encoded;
}

};    // namespace Utils

#endif    // DATA_TYPES_UTILS_H
